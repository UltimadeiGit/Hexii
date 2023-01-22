#include "CompoundLabel.h"

USING_NS_CC;

CompoundLabel* CompoundLabel::create(const std::string& constPartString, const std::string& constPartFontFilepath, const std::string& variablePartFontFilepath) {
	CompoundLabel* pRet = new(std::nothrow) CompoundLabel(constPartString, constPartFontFilepath, variablePartFontFilepath);
	if (pRet && pRet->initWithFonts(constPartString, constPartFontFilepath, variablePartFontFilepath))
	{
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

CompoundLabel::CompoundLabel(const std::string& constPartString, const std::string& constPartFontFilepath, const std::string& variablePartFontFilepath)
    : m_spaceBetweenParts(0.0f), m_constPart(nullptr), m_variablePart(nullptr), m_iconPart(nullptr)
{}

bool CompoundLabel::initWithFonts(const std::string& constPartString, const std::string& constPartFontFilepath, const std::string& variablePartFontFilepath) {
    m_constPart = Label::createWithTTF(constPartString, constPartFontFilepath, 20, Size::ZERO, TextHAlignment::LEFT);
	m_constPart->setAnchorPoint(Vec2(1, 0.0));
	
	m_variablePart = Label::createWithTTF("", variablePartFontFilepath, 20, Size::ZERO, TextHAlignment::CENTER);
	m_variablePart->setAnchorPoint(Vec2(0, 0.0));

	updateSize();

	this->addChild(m_constPart);
	this->addChild(m_variablePart);

	return true;
}

void CompoundLabel::setStyle(bool applyConstPart, bool applyVariablePart, unsigned int fontSize, const cocos2d::Color4B& textColor, const cocos2d::Color4B& outlineColor, int outlineSize, const cocos2d::Color4B& shadowColor, const cocos2d::Size& shadowOffset, int shadowBlurRadius) {
	if (applyConstPart) setSingleStyle(m_constPart, fontSize, textColor, outlineColor, outlineSize, shadowColor, shadowOffset, shadowBlurRadius);
	if (applyVariablePart) setSingleStyle(m_variablePart, fontSize, textColor, outlineColor, outlineSize, shadowColor, shadowOffset, shadowBlurRadius);

	updateSize();
}

void CompoundLabel::setSingleStyle(cocos2d::Label* part, unsigned int fontSize, const cocos2d::Color4B& textColor, const cocos2d::Color4B& outlineColor, int outlineSize, const cocos2d::Color4B& shadowColor, const cocos2d::Size& shadowOffset, int shadowBlurRadius) {
	if(textColor.a > 0) part->setTextColor(textColor);
	if (fontSize > 0 && part->getRenderingFontSize() != fontSize) {
		TTFConfig config = part->getTTFConfig();
		config.fontSize = fontSize;
		part->setTTFConfig(config);
	}
	if (outlineSize > 0 && outlineColor.a > 0) part->enableOutline(outlineColor, outlineSize);
	if (shadowBlurRadius > 0 && shadowColor.a > 0) part->enableShadow(shadowColor, shadowOffset, shadowBlurRadius);

	// It is assumed that `updateSize` will be called by the caller of this function
}

void CompoundLabel::setSpacing(float spaceBetweenParts) {
	if (m_spaceBetweenParts == spaceBetweenParts) return;

	m_spaceBetweenParts = spaceBetweenParts;
	updateSize();
}

void CompoundLabel::setVariablePartString(const std::string& text) {
	if (m_variablePart->getString() == text) return;

	m_variablePart->setString(text);
	
	updateSize();
}

void CompoundLabel::setIconTexture(const std::string& texturePath) {
	if (!m_iconPart) {
		m_iconPart = Sprite::create(texturePath);
		m_iconPart->setAnchorPoint(Vec2(0, 0));
		this->addChild(m_iconPart);

		return updateSize();
	}

	Texture2D* newTexture = Director::getInstance()->getTextureCache()->addImage(texturePath);
	if (m_iconPart->getTexture() == newTexture) return;

	m_iconPart->setTexture(newTexture);

	updateSize();
}

void CompoundLabel::updateSize() {
	const Size& constPartSize = m_constPart->getContentSize();
	const Size& variablePartSize = m_variablePart->getContentSize();
	Size iconSize = Size::ZERO;
	float iconSizeScale = 1.0f;

	float height = std::max(constPartSize.height, variablePartSize.height);

	// The anchor points are such that the base position for both parts can be the same (const protrudes left while variable is right)
	m_constPart->setPosition(Vec2(constPartSize.width, 0));
	m_variablePart->setPosition(Vec2(m_spaceBetweenParts + constPartSize.width, 0));

	if (m_iconPart) {
		iconSize = m_iconPart->getContentSize();
		// The icon is scaled up or down to match the text size
		iconSizeScale = height / iconSize.height;

		m_iconPart->setPosition(Vec2(2 * m_spaceBetweenParts + constPartSize.width + variablePartSize.width, 0));
		m_iconPart->setScale(iconSizeScale);
	}

	setContentSize(Size(constPartSize.width + variablePartSize.width + iconSize.width * iconSizeScale, height));
}