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
    : m_constPart(nullptr), m_variablePart(nullptr), m_iconPart(nullptr)
{}

bool CompoundLabel::initWithFonts(const std::string& constPartString, const std::string& constPartFontFilepath, const std::string& variablePartFontFilepath) {
    m_constPart = Label::createWithTTF(constPartString, constPartFontFilepath, 20, Size::ZERO, TextHAlignment::LEFT);
	m_constPart->setAnchorPoint(Vec2(0, 0.0));
	
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

void CompoundLabel::setSpacingConstraint(float constVariableSpacing, float variableIconSpacing) {
	if (m_spacingConstraintFirst == constVariableSpacing && m_spacingConstraintSecond == variableIconSpacing) return;

	m_spacingConstraintFirst = constVariableSpacing;
	m_spacingConstraintSecond = variableIconSpacing;
	setConstraintMode(ConstraintMode::SPACING);
	updateSize();
}

void CompoundLabel::setWidthConstrant(float width, float minSpacing, VariablePartFloat variablePartFloat)
{
	// Width Constraint: \
		constPartSize.width + variablePartSize.width + iconSize.width * iconSizeScale + m_spaceBetweenParts * 2 === size.width
	// Height Constraint: \
		max(constPartSize.height, variablePartSize.height) === size.height

	//m_constPart->setOverflow(Label::Overflow::SHRINK);

	// We want to try to keep spaceBetweenParts constant, so we need to adjust the size of the parts

	const Size& constPartSize = m_constPart->getContentSize();
	const Size& variablePartSize = m_variablePart->getContentSize();
	Size iconSize = m_iconPart->getContentSize() * m_iconPart->getScale();
	
	float contentWidth = constPartSize.width + variablePartSize.width + iconSize.width;

	float totalSpacingAllowed = width - contentWidth;

	switch (variablePartFloat) {
		case VariablePartFloat::LEFT:
			//m_spacingConstraintFirst = minSpacing;
			//m_spacingConstraintSecond = totalSpacingAllowed - minSpacing;
			m_variablePart->setAnchorPoint(Vec2(0, 0.0));
			//m_variablePart->setPosition(Vec2(constPartSize.width + m_spacingConstraintFirst, 0.0));
			m_variablePart->setAlignment(TextHAlignment::LEFT);
			break;
		case VariablePartFloat::RIGHT:
			//m_spacingConstraintFirst = totalSpacingAllowed - minSpacing;
			//m_spacingConstraintSecond = minSpacing;
			m_variablePart->setAnchorPoint(Vec2(1.0, 0.0));
			//m_variablePart->setPosition(Vec2(width - iconSize.width - m_spacingConstraintSecond, 0.0));
			m_variablePart->setAlignment(TextHAlignment::RIGHT);
			break;
		case VariablePartFloat::CENTER:
			//m_spacingConstraintFirst = m_spacingConstraintSecond = totalSpacingAllowed / 2;
			m_variablePart->setAnchorPoint(Vec2(0.5, 0.0));
			//m_variablePart->setPosition(Vec2(constPartSize.width + (variablePartSize.width / 2) + m_spacingConstraintFirst, 0.0));
			m_variablePart->setAlignment(TextHAlignment::CENTER);
			break;
	}

	//if (m_spacingConstraintFirst < minSpacing) m_spacingConstraintFirst = minSpacing;
	//if (m_spacingConstraintSecond < minSpacing) m_spacingConstraintSecond = minSpacing;
	
	m_widthConstraint = width;
	m_widthConstraintMinSpacing = minSpacing;
	m_widthConstraintFloatType = variablePartFloat;
	setConstraintMode(ConstraintMode::WIDTH);	
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
		m_iconPart->setAnchorPoint(Vec2(0, 0.0));
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
	//m_constPart->setPosition(Vec2(constPartSize.width, 0));
	
	if (m_iconPart) {
		iconSize = m_iconPart->getContentSize();
		// The icon is scaled up or down to match the text size
		iconSizeScale = height / iconSize.height;

		m_iconPart->setScale(iconSizeScale);
	}

	if (m_constraintMode == ConstraintMode::WIDTH) Node::setContentSize(Size(m_widthConstraint, height));
	else Node::setContentSize(Size(
		constPartSize.width + variablePartSize.width + iconSize.width * iconSizeScale + m_spacingConstraintFirst + m_spacingConstraintSecond,
		height));

	updatePositions();
}

void CompoundLabel::updatePositions() {
	const Size& constPartSize = m_constPart->getContentSize();
	const Size& variablePartSize = m_variablePart->getContentSize();
	Size iconSize = m_iconPart ? m_iconPart->getContentSize() * m_iconPart->getScale() : Size::ZERO;

	if (m_constraintMode == ConstraintMode::WIDTH) {

		switch (m_widthConstraintFloatType) {
		case VariablePartFloat::LEFT:
			m_variablePart->setPosition(Vec2(constPartSize.width + m_widthConstraintMinSpacing, 0.0));
			break;
		case VariablePartFloat::RIGHT:
			m_variablePart->setPosition(Vec2(m_widthConstraint - iconSize.width - m_widthConstraintMinSpacing, 0.0));
			break;
		case VariablePartFloat::CENTER:
			m_variablePart->setPosition(Vec2(m_widthConstraint / 2, 0.0));
			break;
		}
		m_iconPart->setPosition(Vec2(m_widthConstraint - iconSize.width, 0.0));
	}
	else { // Spacing constraint
		m_variablePart->setPosition(Vec2(constPartSize.width + m_spacingConstraintFirst, 0));
		if(m_iconPart) m_iconPart->setPosition(Vec2(constPartSize.width + variablePartSize.width + m_spacingConstraintFirst + m_spacingConstraintSecond, 0));
	}
}
