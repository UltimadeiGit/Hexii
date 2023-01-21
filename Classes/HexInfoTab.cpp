#include "HexInfoTab.h"
#include "Maths.h"

USING_NS_CC;

HexInfoTab::HexInfoTab() : m_focus(nullptr), m_upgradesList(nullptr), m_hexEXPBar(nullptr), m_focusSprite(nullptr) {}

bool HexInfoTab::init() {
	setContentSize(Size(420, 1320));

	m_upgradesList = ui::ScrollView::create();
	m_upgradesList->setAnchorPoint(Vec2(0.0, 0.0));
	m_upgradesList->setDirection(ui::ScrollView::Direction::VERTICAL);
	m_upgradesList->setInnerContainerSize(Size(420, 930));
	m_upgradesList->setContentSize(Size(420, 4650));
	m_upgradesList->setBounceEnabled(true);	
	m_upgradesList->setPosition(Vec2(15, 70));
	m_upgradesList->setBackGroundColor(Color3B::BLACK);

	m_hexEXPBar = ProgressBar::create(155, 6);
	m_hexEXPBar->setAnchorPoint(Vec2(0.0, 0.5));
	m_hexEXPBar->setPosition(Vec2(30, 1102));

	m_focusSprite = Sprite::create("HexagonInactive.png");
	m_focusSprite->setContentSize(Size(161 * HEXAGON_HEIGHT_TO_WIDTH, 161));
	m_focusSprite->setAnchorPoint(Vec2(0.5, 0.5));
	m_focusSprite->setPosition(Vec2(110, 1221));
	m_focusSprite->setFlippedY(true);

	m_layerLabel = Label::createWithTTF("--", "fonts/OCR.ttf", 20, Size::ZERO, TextHAlignment::CENTER);
	m_layerLabel->setTextColor(Color4B::WHITE);
	m_layerLabel->enableOutline(Color4B::BLACK, 1);
	m_layerLabel->enableShadow(Color4B::BLACK, Size::ZERO, 2);
	m_layerLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_layerLabel->setPosition(Vec2(112, 1317));

	m_layerLabel = Label::createWithTTF("--", "fonts/OCR.ttf", 20, Size::ZERO, TextHAlignment::CENTER);
	m_layerLabel->setTextColor(Color4B::WHITE);
	m_layerLabel->enableOutline(Color4B::BLACK, 1);
	m_layerLabel->enableShadow(Color4B::BLACK, Size::ZERO, 2);
	m_layerLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_layerLabel->setPosition(Vec2(112, 1317));

	m_levelLabel = CompoundLabel::create("Level ", "fonts/OCR.ttf", "fonts/OCR.ttf");
	m_levelLabel->setStyle(true, true, 36, Color4B::WHITE, Color4B::BLACK, 1, Color4B::BLACK, Size(2, -2), 2);
	m_levelLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_levelLabel->setPosition(Vec2(300, 1276));

	m_expLabel = CompoundLabel::create("EXP:", "fonts/OCR.ttf", "fonts/OCR.ttf");
	m_expLabel->setStyle(true, true, 26, Color4B::WHITE, Color4B::BLACK, 1, Color4B::BLACK, Size(2, -2), 2);
	m_expLabel->setIconTexture("icons/EXP.png");
	m_expLabel->setSpacing(15.0f);
	m_expLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_expLabel->setPosition(Vec2(300, 1231));

	m_yieldLabel = CompoundLabel::create("Yield:", "fonts/OCR.ttf", "fonts/OCR.ttf");
	m_yieldLabel->setStyle(true, true, 26, Color4B::WHITE, Color4B::BLACK, 1, Color4B::BLACK, Size(2, -2), 2);
	m_yieldLabel->setIconTexture("icons/GreenMatter.png");
	m_yieldLabel->setSpacing(15.0f);
	m_yieldLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_yieldLabel->setPosition(Vec2(300, 1186));

	this->addChild(m_upgradesList);
	this->addChild(m_hexEXPBar);
	this->addChild(m_focusSprite);
	this->addChild(m_layerLabel);
	this->addChild(m_levelLabel);
	this->addChild(m_expLabel);
	this->addChild(m_yieldLabel);

	setFocus(nullptr);

	return true;
}

void HexInfoTab::update(float dt) {
	if (m_focus == nullptr) {
		return;
	}

	unsigned int level = m_focus->getLevel();
	unsigned int lastEXPRequirement = m_focus->getEXPRequiredForLevel(level);

	m_levelLabel->setVariablePartString(std::to_string(level));
	m_expLabel->setVariablePartString(std::to_string(m_focus->getEXP()));
	m_yieldLabel->setVariablePartString(m_focus->getYield().to_string());

	m_hexEXPBar->setProgress(
		// Ratio of current exp into this level over the exp needed to level up
		(float)(m_focus->getEXP() - lastEXPRequirement) /
		(m_focus->getEXPRequiredForLevel(level + 1) - lastEXPRequirement)
	);

	m_focusSprite->setTexture(m_focus->getShadedRenderTexture());
}

void HexInfoTab::setFocus(Hex* focus) {
	m_focus = focus;

	// If a focus exists, visible otherwise not
	setVisible(m_focus);

	if (!m_focus) return;

	m_layerLabel->setString("L" + std::to_string(m_focus->getLayer()));
}
