#include "HexiiInfoTab.h"
#include "Resources.h"
#include "ColourSchemes.h"
#include "Progression.h"
#include "UICommon.h"

constexpr float LABEL_SPACING = 15.0f;
constexpr float LABEL_HEIGHT = 55.0f;
//constexpr float LABEL_WIDTH = 315.0f;
constexpr float INFO_PLATE_WIDTH = 548.0f;
constexpr float INFO_PLATE_START = 365.0f;
constexpr cocos2d::Vec2 TAB_POS = { INFO_PLATE_WIDTH + 409, 10 };
constexpr cocos2d::Vec2 TAB_BUTTON_POS = { INFO_PLATE_WIDTH + 441, 460 };
constexpr uint LABEL_FONT_SIZE = 38;

USING_NS_CC;

#define LABEL_STYLE Color4B::WHITE, Color4B::BLACK, 4, Color4B::BLACK, Size(4, -4), 4
#define LABEL_STYLE_LIGHT Color4B::WHITE, Color4B::BLACK, 3, Color4B::BLACK, Size(0, 0), 2


HexiiInfoTab::HexiiInfoTab() {}

bool HexiiInfoTab::init() {
	/// Background overlay

	m_backgroundOverlay = Sprite::create("UI/HexiiInfoTab/BackgroundOverlay.png");
	m_backgroundOverlay->setAnchorPoint(Vec2(0, 0));
	//m_backgroundOverlay->setPosition(Vec2(1, 591));
	this->addChild(m_backgroundOverlay, -1);

	setContentSize(UICommon::SIDEBAR_SIZE);

	/// Hexii sprite

	m_focusSprite = Sprite::create("gameplay/HexInactive.png");
	m_focusSprite->setContentSize(Size(283 * HEXAGON_HEIGHT_TO_WIDTH, 283));
	m_focusSprite->setAnchorPoint(Vec2(0.5, 0.5));
	m_focusSprite->setPosition(Vec2(198, 170));
	m_focusSprite->setFlippedY(true);
	this->addChild(m_focusSprite, 1);

	m_hexiiNameLabel = Label::createWithTTF("", "fonts/BreeSerif.ttf", 36, Size::ZERO, TextHAlignment::CENTER);
	m_hexiiNameLabel->setTextColor(Color4B::WHITE);
	m_hexiiNameLabel->enableOutline(Color4B::BLACK, 3);
	m_hexiiNameLabel->enableShadow(Color4B::BLACK, Size::ZERO, 4);
	m_hexiiNameLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_hexiiNameLabel->setPosition(Vec2(190, 360));
	this->addChild(m_hexiiNameLabel, 1);

	m_hexiilayerLabel = Label::createWithTTF("", "fonts/BreeSerif.ttf", 24, Size::ZERO, TextHAlignment::CENTER);
	m_hexiilayerLabel->setTextColor(Color4B::WHITE);
	m_hexiilayerLabel->enableOutline(Color4B::BLACK, 1);
	m_hexiilayerLabel->enableShadow(Color4B::BLACK, Size::ZERO, 2);
	m_hexiilayerLabel->enableItalics();
	m_hexiilayerLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_hexiilayerLabel->setPosition(Vec2(190, 410));
	this->addChild(m_hexiilayerLabel, 1);

	/*
	/// Pin button

	m_pinButton = ui::Button::create("widgets/PinDisabled.png", "widgets/PinSelected.png", "widgets/PinDisabled.png");
	m_pinButton->setAnchorPoint(Vec2(0.5, 0.5));
	m_pinButton->setPosition(Vec2(390, 1303));
	m_pinButton->setScale(2.0f);
	m_pinButton->addTouchEventListener(CC_CALLBACK_2(HexiiInfoTab::onPinButtonPressed, this));
	*/

	/// Labels

	constexpr float LABEL_PADDING = 20.0f;
	constexpr float LABEL_LEFT_X = INFO_PLATE_START + LABEL_PADDING;
	constexpr float LABEL_RIGHT_X = INFO_PLATE_START + INFO_PLATE_WIDTH - LABEL_PADDING + 5;
	constexpr float LABEL_TOP_Y = 385.0f;
	constexpr float WIDTH_CONSTRAINT = INFO_PLATE_WIDTH;
	int labelIndex = 0;

	// Level label

	const float LEVEL_LABEL_Y = LABEL_TOP_Y + 10 - (LABEL_HEIGHT * labelIndex++);

	m_levelLabel = CompoundLabel::create("Level", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_levelLabel->setStyle(true, true, LABEL_FONT_SIZE * 1.3, LABEL_STYLE);
	m_levelLabel->setStyle(false, true, LABEL_FONT_SIZE * 1.3, GENERAL_IMPORTANT_VALUE);
	//m_levelLabel->setIconTexture("icons/EXPSmall.png");
	m_levelLabel->setSpacingConstraint(LABEL_SPACING);
	//m_levelLabel->setWidthConstrant(LABEL_WIDTH, LABEL_SPACING, CompoundLabel::VariablePartFloat::);
	m_levelLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_levelLabel->setPosition({ INFO_PLATE_START + LABEL_PADDING + (INFO_PLATE_WIDTH / 2), LEVEL_LABEL_Y});
	this->addChild(m_levelLabel, 1);

	m_rawLevelLabel = CompoundLabel::create("(", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_rawLevelLabel->setStyle(true, true, LABEL_FONT_SIZE * 0.8, LABEL_STYLE_LIGHT);
	m_rawLevelLabel->setStyle(false, true, LABEL_FONT_SIZE * 0.8, GENERAL_IMPORTANT_VALUE);
	//m_levelLabel->setIconTexture("icons/EXPSmall.png");
	m_rawLevelLabel->setSpacingConstraint(5.0f);
	//m_levelLabel->setWidthConstrant(LABEL_WIDTH, LABEL_SPACING, CompoundLabel::VariablePartFloat::);
	m_rawLevelLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_rawLevelLabel->setPosition({ 0, LEVEL_LABEL_Y});
	this->addChild(m_rawLevelLabel, 1);

	m_levelsFromRedMatterLabel = CompoundLabel::create("+ ", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_levelsFromRedMatterLabel->setStyle(true, true, LABEL_FONT_SIZE * 0.8, LABEL_STYLE_LIGHT);
	m_levelsFromRedMatterLabel->setStyle(false, true, LABEL_FONT_SIZE * 0.8, COMMON_RED);
	//m_levelsFromRedMatterLabel->setIconTexture("icons/EXPSmall.png");
	m_levelsFromRedMatterLabel->setSpacingConstraint(0.0f);
	//m_levelsFromRedMatterLabel->setWidthConstrant(LABEL_WIDTH, LABEL_SPACING, CompoundLabel::VariablePartFloat::);
	m_levelsFromRedMatterLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_levelsFromRedMatterLabel->setPosition({ 0, LEVEL_LABEL_Y });
	this->addChild(m_levelsFromRedMatterLabel, 1);

	m_levelLabelRight = Label::createWithTTF(")", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE * 0.8, Size::ZERO, TextHAlignment::RIGHT);
	CompoundLabel::setSingleStyle(m_levelLabelRight, LABEL_FONT_SIZE * 0.8, LABEL_STYLE_LIGHT);
	m_levelLabelRight->setAnchorPoint(Vec2(0.0, 0.5));
	m_levelLabelRight->setPosition({ 0, LEVEL_LABEL_Y });
	this->addChild(m_levelLabelRight, 1);

	// Scaling

	const float SCALE_LABEL_Y = LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex++);

	m_scalingLabel = CompoundLabel::create("Level scale:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_scalingLabel->setStyle(true, true, LABEL_FONT_SIZE, LABEL_STYLE);
	m_scalingLabel->setStyle(false, true, LABEL_FONT_SIZE, COMMON_RED);
	//m_scalingLabel->setIconTexture("icons/EXPSmall.png");
	m_scalingLabel->setSpacingConstraint(20.0f, 5.0f);
	//m_scalingLabel->setWidthConstrant(WIDTH_CONSTRAINT, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_scalingLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_scalingLabel->setPosition({ LABEL_LEFT_X, SCALE_LABEL_Y });
	this->addChild(m_scalingLabel, 1);

	m_redMatterLabel = CompoundLabel::create("from", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_redMatterLabel->setStyle(true, true, LABEL_FONT_SIZE, LABEL_STYLE);
	m_redMatterLabel->setStyle(false, true, LABEL_FONT_SIZE, COMMON_RED);
	m_redMatterLabel->setIconTexture("icons/RedMatterSmall.png");
	m_redMatterLabel->setSpacingConstraint(10.0f, 5.0f);
	//m_redMatterLabel->setWidthConstrant(WIDTH_CONSTRAINT, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_redMatterLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_redMatterLabel->setPosition({ LABEL_LEFT_X, SCALE_LABEL_Y });
	this->addChild(m_redMatterLabel, 1);

	// Yield label

	const float YIELD_LABEL_Y = LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex++);

	m_yieldLabel = CompoundLabel::create("Yield:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_yieldLabel->setStyle(true, true, LABEL_FONT_SIZE, LABEL_STYLE);
	m_yieldLabel->setStyle(false, true, LABEL_FONT_SIZE, INACTIVE_BLUE);
	m_yieldLabel->setIconTexture("icons/EXPSmall.png");
	m_yieldLabel->setSpacingConstraint(20.0f, 5.0f);
	//m_yieldLabel->setWidthConstrant(WIDTH_CONSTRAINT, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_yieldLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_yieldLabel->setPosition({ LABEL_LEFT_X, YIELD_LABEL_Y });
	this->addChild(m_yieldLabel, 1);

	// The label to the left of the yield time value (says "Every ")
	m_yieldTimeLeft = Label::createWithTTF("/", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	CompoundLabel::setSingleStyle(m_yieldTimeLeft, LABEL_FONT_SIZE, LABEL_STYLE);
	m_yieldTimeLeft->setAnchorPoint(Vec2(0.0, 0.5));
	m_yieldTimeLeft->setPosition({ LABEL_LEFT_X, YIELD_LABEL_Y });
	this->addChild(m_yieldTimeLeft, 1);

	// The label to the right of the yield time value (says " seconds")
	m_yieldTimeRight = Label::createWithTTF("seconds", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	CompoundLabel::setSingleStyle(m_yieldTimeRight, LABEL_FONT_SIZE, LABEL_STYLE);
	m_yieldTimeRight->setAnchorPoint(Vec2(0.0, 0.5));
	m_yieldTimeRight->setPosition({ LABEL_LEFT_X, YIELD_LABEL_Y });
	this->addChild(m_yieldTimeRight, 1);

	m_yieldSpeedLabel = Label::createWithTTF("", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	CompoundLabel::setSingleStyle(m_yieldSpeedLabel, LABEL_FONT_SIZE, LABEL_STYLE);
	m_yieldSpeedLabel->setTextColor(INACTIVE_BLUE);
	m_yieldSpeedLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_yieldSpeedLabel->setPosition({
		LABEL_LEFT_X, //LABEL_RIGHT_X - m_yieldTimeRight->getContentSize().width,
		YIELD_LABEL_Y
		});
	this->addChild(m_yieldSpeedLabel, 1);

	/// EXP 

	m_expLabel = CompoundLabel::create("EXP:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_expLabel->setStyle(true, true, LABEL_FONT_SIZE, LABEL_STYLE);
	m_expLabel->setStyle(false, true, LABEL_FONT_SIZE, GENERAL_IMPORTANT_VALUE);
	m_expLabel->setIconTexture("icons/EXPSmall.png");
	//m_expLabel->setSpacingConstraint(20.0f, 5.0f);
	m_expLabel->setWidthConstrant(WIDTH_CONSTRAINT, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_expLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_expLabel->setPosition({ LABEL_LEFT_X, LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex++) });
	this->addChild(m_expLabel, 1);

	m_nextLevelLabel = CompoundLabel::create("Next level in:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_nextLevelLabel->setStyle(true, true, LABEL_FONT_SIZE, LABEL_STYLE);
	m_nextLevelLabel->setStyle(false, true, LABEL_FONT_SIZE, GENERAL_IMPORTANT_VALUE);
	m_nextLevelLabel->setIconTexture("icons/EXPSmall.png");
	//m_nextLevelLabel->setSpacingConstraint(20.0f, 5.0f);
	m_nextLevelLabel->setWidthConstrant(WIDTH_CONSTRAINT, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_nextLevelLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_nextLevelLabel->setPosition({ LABEL_LEFT_X, LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex++) });
	this->addChild(m_nextLevelLabel, 1);

	/*
	// The label to the right of the exp to next level label, closing the brackets
	m_nextLevelRight = Label::createWithTTF(")", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	m_nextLevelRight->setTextColor(Color4B::WHITE);
	m_nextLevelRight->enableShadow(Color4B::BLACK, Size(4, -4), 4);
	m_nextLevelRight->setAnchorPoint(Vec2(0.0, 0.5));
	m_nextLevelRight->setPosition({ LABEL_RIGHT_X, LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex) });
	this->addChild(m_nextLevelRight, 1);
	*/

	/// HexiiEXPTab

	m_expInfoTab = HexiiEXPTab::create();
	initTab(0, m_expInfoTab, TAB_POS, { 0, 1 }, TAB_BUTTON_POS, { 110, 0 }, "UI/HexiiInfoTab/EXPTab", "UI/HexiiInfoTab/DisabledTab");

	m_standardL0UpgradesTab = HexiiUpgradesTab::create(UpgradePath::getStandardL0Path());
	m_standardL0UpgradesTab->retain();
	m_standardUpgradesTab = HexiiUpgradesTab::create(UpgradePath::getStandardPath());
	m_standardUpgradesTab->retain();

	switchTab(0);

	/// Events
	
	EventUtility::addGlobalEventListener(Hexii::EVENT_FOCUS, this, &HexiiInfoTab::onHexiiFocus);

	setFocusHexii(nullptr);

	return true;
}

void HexiiInfoTab::update(float dt) {
	if (!m_focusHexii) return;

	/// Update labels

	// TODO: Add a setting to change the speed of UI updates
	updateStatLabels();

	/// Update sub tab
	getCurrentTab()->update(dt);
}

void HexiiInfoTab::onHexiiFocus(cocos2d::EventCustom* evnt) {
	Hexii::EventHexiiFocusData* data = static_cast<Hexii::EventHexiiFocusData*>(evnt->getUserData());

	if (data->active) setFocusHexii(data->subject);
}

void HexiiInfoTab::switchTab(unsigned short tab) {
	TabbedWidget::switchTab(tab);
	HexiiTab* currentTab = getCurrentTab();
	currentTab->setFocus(m_focusHexii);
}

/*
void HexiiInfoTab::onPinButtonPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
	// Only care about the release
	if (evntType != ui::Widget::TouchEventType::ENDED) return;

	// Pin / unpin

	m_pinned = !m_pinned;

	if(m_pinned) m_pinButton->loadTextureNormal("widgets/PinActive.png"); 
	else m_pinButton->loadTextureNormal("widgets/PinDisabled.png");

	_eventDispatcher->dispatchCustomEvent("onPinButtonPressed", new bool(m_pinned));
}
*/

void HexiiInfoTab::onLevelGained(cocos2d::EventCustom* evnt) {
	// auto data = EventUtility::getEventData<Hexii::EventHexiiLevelGainedData>(evnt);

	// updateSpecializationLabels(false, true);
	if (isCurrentTabAnUpgradesTab()) {
		HexiiUpgradesTab* upgradesTab = dynamic_cast<HexiiUpgradesTab*>(getCurrentTab());
		upgradesTab->updateStates();
	}
}

void HexiiInfoTab::initUpgradesTab(unsigned short tabNumber, HexiiUpgradesTab* tab, const std::string& pathName) {
	initTab(tabNumber, tab, TAB_POS, { 0, 1 }, TAB_BUTTON_POS, { 110, 0 }, "UI/HexiiInfoTab/" + pathName + "UpgradesTab", "UI/HexiiInfoTab/DisabledTab", false);
	
	m_tabButtons[tabNumber]->setVisible(false);

	// Enable the tab when the upgrades become available
	Progression::hexiiUpgradesAvailable()->when(ProgressionEvent::State::ACHIEVED, [this, tabNumber]() {
		enableTab(tabNumber, false);
		m_tabButtons[tabNumber]->setVisible(true);
	});
}

void HexiiInfoTab::setFocusHexii(Hexii* focus) {
	if (m_focusHexii == focus && focus != nullptr) return;

	/// Cleanup

	// Remove old tabs (first tab is always the EXP tab)
	for (uint i = 1; i < HEXII_INFO_TAB_COUNT; i++) removeTab(i);

	// Remove listeners
	if (m_levelGainedListener) _eventDispatcher->removeEventListener(m_levelGainedListener);

	m_focusHexii = focus;
	// Propagate focus to subtabs
	if(m_tabs[m_currentTab] != nullptr) getCurrentTab()->setFocus(focus);

	// Quick return if no focus
	if (!m_focusHexii) {
		setVisible(false);
		return;
	}

	/// Setup

	// Setup the new tabs
	if (m_focusHexii->getHexiiType() == Hexii::HexiiType::HOME_L0) {
		initUpgradesTab(1, m_standardL0UpgradesTab, "StandardPath");
	}
	else {
		initUpgradesTab(1, m_standardUpgradesTab, "StandardPath");
	}

	// Setup the new listener
	m_levelGainedListener = EventUtility::addTargetedEventListener(Hexii::EVENT_LEVEL_GAINED, this, m_focusHexii->eventID, &HexiiInfoTab::onLevelGained);

	// Update properties that change with focus but not per frame
	m_yieldLabel->setIconTexture(m_focusHexii->getHexiiType() == Hexii::HexiiType::HOME_L0 ? "icons/GreenMatterSmall.png" : "icons/EXPSmall.png");
	m_focusSprite->setTexture(m_focusHexii->getShadedRenderTexture());
	
	updateNameLabel();
	updateStatLabels();

	// Finally switch back to the last active tab
	switchTab(m_currentTab);
}

void HexiiInfoTab::updateNameLabel() {
	m_hexiiNameLabel->setString(m_focusHexii->getName());
	m_hexiilayerLabel->setString("Layer " + std::to_string(m_focusHexii->getLayer()));
}

void HexiiInfoTab::updateStatLabels() {
	constexpr float PADDING = 10.0f;

	//// Yield label

	// Update the text

	m_yieldLabel->setVariablePartString(formatBigReal(m_focusHexii->getYield()));
	m_yieldSpeedLabel->setString(formatBigReal(1.0 / m_focusHexii->getYieldSpeed(true), false, 3, 2));

	// Update positions

	float labelX = m_yieldLabel->getPosition().x + m_yieldLabel->getContentSize().width + PADDING;
	float labelY = m_yieldLabel->getPosition().y;

	m_yieldTimeLeft->setPosition(labelX, labelY);
	labelX += m_yieldTimeLeft->getContentSize().width + PADDING;
	m_yieldSpeedLabel->setPosition(labelX, labelY);
	labelX += m_yieldSpeedLabel->getContentSize().width + PADDING;
	m_yieldTimeRight->setPosition(labelX, labelY);

	if (m_focusHexii->getIsPressed()) {
		m_yieldLabel->setStyle(false, true, LABEL_FONT_SIZE, ACTIVE_ORANGE);
		m_yieldSpeedLabel->setTextColor(ACTIVE_ORANGE);
	}
	else {
		m_yieldLabel->setStyle(false, true, LABEL_FONT_SIZE, INACTIVE_BLUE);
		m_yieldSpeedLabel->setTextColor(INACTIVE_BLUE);
	}

	//// EXP Label

	/*
	/// Position the EXP label

	labelX = m_expLabel->getPosition().x + m_expLabel->getContentSize().width + PADDING;
	labelY = m_expLabel->getPosition().y;

	m_nextLevelLabel->setPosition(labelX, labelY);
	labelX += m_nextLevelLabel->getContentSize().width + PADDING;
	m_nextLevelRight->setPosition(labelX, labelY);
	*/

	// Update the text

	BigReal exp = m_focusHexii->getEXP();
	BigReal expForNextLevel = m_focusHexii->getEXPForNextLevel() - exp;

	m_expLabel->setVariablePartString(formatBigReal(exp));
	m_nextLevelLabel->setVariablePartString(formatBigReal(expForNextLevel));

	//// Scaling label

	// Update positions

	labelX = m_scalingLabel->getPosition().x + m_scalingLabel->getContentSize().width + PADDING;
	labelY = m_scalingLabel->getPosition().y;

	m_redMatterLabel->setPosition(labelX, labelY);

	// Update the text

	m_scalingLabel->setVariablePartString("x" + formatBigReal(m_focusHexii->getLevelScale(), false, 2, 3));
	m_redMatterLabel->setVariablePartString(formatBigReal(m_focusHexii->getRedMatterInvested()));

	// Update the EXP info tab
	if (m_expInfoTab->isVisible()) {
		m_expInfoTab->updateEXP(exp, expForNextLevel);
		m_expInfoTab->updateRedMatter(m_focusHexii->getRedMatterInvested());
	}

	//// Level label

	// Update text

	BigInt rawLevel = m_focusHexii->getRawLevel();
	BigInt totalLevel = m_focusHexii->getTotalLevel();

	m_levelLabel->setVariablePartString(formatBigInt(totalLevel));
	m_rawLevelLabel->setVariablePartString(formatBigInt(rawLevel));
	m_levelsFromRedMatterLabel->setVariablePartString(formatBigInt(totalLevel - rawLevel));

	// The extra detail on the level label is only applicable if the hexii has red matter levels	

	bool hasRedMatterLevels = rawLevel != totalLevel;

	m_rawLevelLabel->setVisible(hasRedMatterLevels);
	m_levelsFromRedMatterLabel->setVisible(hasRedMatterLevels);
	m_levelLabelRight->setVisible(hasRedMatterLevels);

	if (hasRedMatterLevels) {
		// Update positions

		labelX = m_levelLabel->getPosition().x + (m_levelLabel->getContentSize().width / 2) + (PADDING * 2);
		labelY = m_levelLabel->getPosition().y;

		m_rawLevelLabel->setPosition(labelX, labelY);
		labelX += (m_rawLevelLabel->getContentSize().width) + 5;// + PADDING;
		m_levelsFromRedMatterLabel->setPosition(labelX, labelY);
		labelX += (m_levelsFromRedMatterLabel->getContentSize().width / 2) + 5;
		m_levelLabelRight->setPosition(labelX, labelY);
	}
}