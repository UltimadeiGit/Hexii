#include "HexiiInfoTab.h"
#include "Resources.h"
#include "ColourSchemes.h"
#include "Progression.h"

constexpr float LABEL_SPACING = 15.0f;
constexpr float LABEL_HEIGHT = 55.0f;
//constexpr float LABEL_WIDTH = 315.0f;
constexpr float INFO_PLATE_WIDTH = 427.0f;
constexpr float INFO_PLATE_START = 380.0f;
constexpr cocos2d::Vec2 TAB_POS = { 758 + 58, 10 };
constexpr cocos2d::Vec2 TAB_BUTTON_POS = { 790 + 58, 455 };
constexpr uint LABEL_FONT_SIZE = 42;

USING_NS_CC;

HexiiInfoTab::HexiiInfoTab() {}

bool HexiiInfoTab::init() {
	/// Background overlay

	m_backgroundOverlay = Sprite::create("UI/HexiiInfoTab/BackgroundOverlay.png");
	m_backgroundOverlay->setAnchorPoint(Vec2(0, 0));
	//m_backgroundOverlay->setPosition(Vec2(1, 591));
	this->addChild(m_backgroundOverlay, -1);

	setContentSize(m_backgroundOverlay->getContentSize());

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
	constexpr float LABEL_TOP_Y = 350.0f;
	constexpr float WIDTH_CONSTRAINT = INFO_PLATE_WIDTH - LABEL_PADDING;
	int labelIndex = 0;

	m_levelLabel = CompoundLabel::create("Level", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_levelLabel->setStyle(true, true, LABEL_FONT_SIZE * 1.3, Color4B::WHITE, Color4B::BLACK, 4, Color4B::BLACK, Size(4, -4), 4);
	m_levelLabel->setStyle(false, true, LABEL_FONT_SIZE * 1.3, GENERAL_IMPORTANT_VALUE);
	//m_levelLabel->setIconTexture("icons/EXPSmall.png");
	m_levelLabel->setConstraintMode(CompoundLabel::ConstraintMode::SPACING);
	m_levelLabel->setSpacingConstraint(LABEL_SPACING);
	//m_levelLabel->setWidthConstrant(LABEL_WIDTH, LABEL_SPACING, CompoundLabel::VariablePartFloat::);
	m_levelLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_levelLabel->setPosition({ INFO_PLATE_START + LABEL_PADDING + (INFO_PLATE_WIDTH / 2), LABEL_TOP_Y + 10 - (LABEL_HEIGHT * labelIndex++)});
	this->addChild(m_levelLabel, 1);

	m_yieldLabel = CompoundLabel::create("Yield:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_yieldLabel->setStyle(true, true, LABEL_FONT_SIZE, Color4B::WHITE, Color4B::BLACK, 0, Color4B::BLACK, Size(4, -4), 4);
	m_yieldLabel->setStyle(false, true, LABEL_FONT_SIZE, INACTIVE_BLUE);
	m_yieldLabel->setIconTexture("icons/EXPSmall.png");
	m_yieldLabel->setWidthConstrant(WIDTH_CONSTRAINT, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_yieldLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_yieldLabel->setPosition({ LABEL_LEFT_X, LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex++) });
	this->addChild(m_yieldLabel, 1);

	// The label to the left of the yield time value (says "Every ")
	cocos2d::Label* yieldTimeLeft = Label::createWithTTF("Every ", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	yieldTimeLeft->setTextColor(Color4B::WHITE);
	yieldTimeLeft->enableShadow(Color4B::BLACK, Size(4, -4), 4);
	yieldTimeLeft->setAnchorPoint(Vec2(0.0, 0.5));
	yieldTimeLeft->setPosition({ LABEL_LEFT_X, LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex) });
	this->addChild(yieldTimeLeft, 1);

	// The label to the right of the yield time value (says " seconds")
	cocos2d::Label* yieldTimeRight = Label::createWithTTF(" seconds", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	yieldTimeRight->setTextColor(Color4B::WHITE);
	yieldTimeRight->enableShadow(Color4B::BLACK, Size(4, -4), 4);
	yieldTimeRight->setAnchorPoint(Vec2(1.0, 0.5));
	yieldTimeRight->setPosition({ LABEL_RIGHT_X, LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex) });
	this->addChild(yieldTimeRight, 1);

	m_yieldSpeedLabel = Label::createWithTTF("", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	m_yieldSpeedLabel->setTextColor(INACTIVE_BLUE);
	m_yieldSpeedLabel->enableShadow(Color4B::BLACK, Size(4, -4), 4);
	m_yieldSpeedLabel->setAnchorPoint(Vec2(1.0, 0.5));
	m_yieldSpeedLabel->setPosition({ 
		LABEL_RIGHT_X - yieldTimeRight->getContentSize().width,
		LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex++) 
	});
	this->addChild(m_yieldSpeedLabel, 1);

	m_expLabel = CompoundLabel::create("EXP:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_expLabel->setStyle(true, true, LABEL_FONT_SIZE, Color4B::WHITE, Color4B::BLACK, 0, Color4B::BLACK, Size(4, -4), 4);
	m_expLabel->setStyle(false, true, LABEL_FONT_SIZE, GENERAL_IMPORTANT_VALUE);
	m_expLabel->setIconTexture("icons/EXPSmall.png");
	m_expLabel->setWidthConstrant(WIDTH_CONSTRAINT, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_expLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_expLabel->setPosition({ LABEL_LEFT_X, LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex++) });
	this->addChild(m_expLabel, 1);

	m_nextLevelLabel = CompoundLabel::create("Next Level:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_nextLevelLabel->setStyle(true, true, LABEL_FONT_SIZE, Color4B::WHITE, Color4B::BLACK, 0, Color4B::BLACK, Size(4, -4), 4);
	m_nextLevelLabel->setStyle(false, true, LABEL_FONT_SIZE, GENERAL_IMPORTANT_VALUE);
	m_nextLevelLabel->setIconTexture("icons/EXPSmall.png");
	m_nextLevelLabel->setWidthConstrant(WIDTH_CONSTRAINT, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_nextLevelLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_nextLevelLabel->setPosition({ LABEL_LEFT_X, LABEL_TOP_Y - (LABEL_HEIGHT * labelIndex++) });
	this->addChild(m_nextLevelLabel, 1);

	/*
	m_specNameLabel = CompoundLabel::create("Spec:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_specNameLabel->setStyle(true, true, LABEL_FONT_SIZE, Color4B::WHITE, Color4B::BLACK, 0, Color4B::BLACK, Size(4, -4), 4);
	m_specNameLabel->setWidthConstrant(LABEL_WIDTH, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_specNameLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_specNameLabel->setPosition(Vec2(400, 160));
	this->addChild(m_specNameLabel, 1);
	*/
	
	/*
	
	*/

	/// EXPInfoTab

	m_expInfoTab = EXPInfoTab::create();
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
	updateHexiiLabels(false, true, true);	

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

void HexiiInfoTab::onUpgradePurchased(cocos2d::EventCustom* evnt) {
	m_yieldLabel->setVariablePartString(formatBigReal(m_focus->getYield()));
}

void HexiiInfoTab::onLevelGained(cocos2d::EventCustom* evnt) {
	Hexii::EventLevelGainedData* data = static_cast<Hexii::EventLevelGainedData*>(evnt->getUserData());

	// Only update this box if the hex that leveled up was this tab's focus
	if (data->subject != m_focus) return;

	/// Update labels for hex stats

	m_levelLabel->setVariablePartString(std::to_string(data->level));
	m_yieldLabel->setVariablePartString(formatBigReal(m_focus->getYield()));

	updateUpgradesList(data->levelBefore, data->level);
}

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

	Progression::whenAchieved(Progression::hexiiUpgradesAvailable(), [this, tabNumber]() {
		enableTab(tabNumber, false);
		m_tabButtons[tabNumber]->setVisible(true);
	});
}

void HexiiInfoTab::setFocusHexii(Hexii* focus) {
	if (m_focusHexii == focus) return;

	/// Cleanup

	// Remove old tabs (first tab is always the EXP tab)
	for (uint i = 1; i < HEXII_INFO_TAB_COUNT; i++) removeTab(i);

	// Remove listeners
	if (m_levelGainedListener) {
		_eventDispatcher->removeEventListener(m_levelGainedListener);
	}

	m_focusHexii = focus;
	// Propagate focus to subtabs
	if(m_tabs[m_currentTab] != nullptr) getCurrentTab()->setFocus(focus);

	// Quick return if no focus
	if (!m_focusHexii) {
		//setFocusSpecialization(nullptr);
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

	/*auto& specGroups = m_focusHexii->getSpecializations();
	// Setup the specs
	if (specGroups.size() == 0) setFocusSpecialization(nullptr);
	else for (auto& specGroup : specGroups) {
	}
	*/

	// Update properties that change with focus but not per frame
	m_yieldLabel->setIconTexture(m_focusHexii->getHexiiType() == Hexii::HexiiType::HOME_L0 ? "icons/GreenMatterSmall.png" : "icons/EXPSmall.png");
	m_focusSprite->setTexture(m_focusHexii->getShadedRenderTexture());
	updateHexiiLabels(true, true, true);

	// Finally switch back to the last active tab
	switchTab(m_currentTab);
}

/*
void HexiiInfoTab::setFocusSpecialization(Specialization* focus) {
	if (m_focusSpecialization == focus) return;

	/// Cleanup

	// Remove listeners
	if (m_levelGainedListener) {
		_eventDispatcher->removeEventListener(m_levelGainedListener);
	}

	// Clear the old upgrades
	//m_upgradeScrollView->removeAllChildrenWithCleanup(true);
	//m_upgradeBoxes.clear();

	m_focusSpecialization = focus;
	// Quick return if no focus
	if (!m_focusSpecialization) return;

	/// Setup the new spec

	// Add listeners
	m_levelGainedListener = EventUtility::addTargetedEventListener(Specialization::EVENT_LEVEL_GAINED, this, m_focusSpecialization->eventID, &HexiiInfoTab::onLevelGained);

	// Update labels
	updateSpecializationLabels(true, true);

	/*
	// Update upgrades list
	auto& upgrades = focus->upgradePath->getUpgrades();
	for (uint i = 0; i < upgrades.size(); i++) {
		UpgradeBox* box = UpgradeBox::create();

		box->setUpgrade(upgrades[i], m_focusSpecialization);
		box->setAnchorPoint(Vec2(0, 0));
		box->setPosition(Vec2(i * (box->getContentSize().width + 40), 0)); // Width of a box + 40 points spacing
		//m_upgradeBoxes.push_back(box);
		m_upgradeScrollView->addChild(box);
	}
	updateScrollView();
	
}
*/

void HexiiInfoTab::updateHexiiLabels(bool nameLabel, bool yieldLabel, bool expLabels) {
	if (nameLabel) {
		m_hexiiNameLabel->setString(m_focusHexii->getName());
		m_hexiilayerLabel->setString("Layer " + std::to_string(m_focusHexii->getLayer()));
	}
	if (yieldLabel) {
		m_yieldLabel->setVariablePartString(formatBigReal(m_focusHexii->getYield()));
		m_yieldSpeedLabel->setString(formatBigReal(1.0 / m_focusHexii->getYieldSpeed(true), false, 3, 2));

		if (m_focusHexii->getIsPressed()) {
			m_yieldLabel->setStyle(false, true, LABEL_FONT_SIZE, ACTIVE_ORANGE);
			m_yieldSpeedLabel->setTextColor(ACTIVE_ORANGE);
		}
		else {
			m_yieldLabel->setStyle(false, true, LABEL_FONT_SIZE, INACTIVE_BLUE);
			m_yieldSpeedLabel->setTextColor(INACTIVE_BLUE);
		}
	}
	if (expLabels) {
		BigReal exp = m_focusHexii->getEXP();
		BigReal expForNextLevel = m_focusHexii->getEXPForNextLevel() - exp;

		m_expLabel->setVariablePartString(formatBigReal(exp));
		m_nextLevelLabel->setVariablePartString(formatBigReal(expForNextLevel));
		m_levelLabel->setVariablePartString(std::to_string(m_focusHexii->getLevel()));

		if (m_expInfoTab->isVisible()) m_expInfoTab->updateLabels(exp, expForNextLevel);
	}

	// TODO: Only show this after specs have been unlocked
	//if (nameLabel) m_specNameLabel->setVariablePartString(m_focusSpecialization->getSpecNameFriendly());
	//if (countLabel) 
}