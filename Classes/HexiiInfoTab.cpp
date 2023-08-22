#include "HexiiInfoTab.h"
#include "Resources.h"
#include "ColourSchemes.h"

constexpr float LABEL_SPACING = 15.0f;
constexpr float LABEL_WIDTH = 315.0f;
constexpr uint LABEL_FONT_SIZE = 36;

USING_NS_CC;

HexiiInfoTab::HexiiInfoTab() {}

bool HexiiInfoTab::init() {
	/// Background overlay

	m_backgroundOverlay = Sprite::create("components/HexiiInfoTabBackgroundOverlay.png");
	m_backgroundOverlay->setAnchorPoint(Vec2(0, 0));
	//m_backgroundOverlay->setPosition(Vec2(1, 591));
	this->addChild(m_backgroundOverlay, -1);

	setContentSize(m_backgroundOverlay->getContentSize());

	/// Hexii sprite

	m_focusSprite = Sprite::create("gameplay/HexInactive.png");
	m_focusSprite->setContentSize(Size(283 * HEXAGON_HEIGHT_TO_WIDTH, 283));
	m_focusSprite->setAnchorPoint(Vec2(0.5, 0.5));
	m_focusSprite->setPosition(Vec2(189, 172));
	m_focusSprite->setFlippedY(true);
	this->addChild(m_focusSprite, 1);

	m_hexiiNameLabel = Label::createWithTTF("", "fonts/BreeSerif.ttf", 36, Size::ZERO, TextHAlignment::CENTER);
	m_hexiiNameLabel->setTextColor(Color4B::WHITE);
	m_hexiiNameLabel->enableOutline(Color4B::BLACK, 3);
	m_hexiiNameLabel->enableShadow(Color4B::BLACK, Size::ZERO, 4);
	m_hexiiNameLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_hexiiNameLabel->setPosition(Vec2(190, 360));
	this->addChild(m_hexiiNameLabel, 1);

	m_hexiiDistrictLabel = Label::createWithTTF("", "fonts/BreeSerif.ttf", 24, Size::ZERO, TextHAlignment::CENTER);
	m_hexiiDistrictLabel->setTextColor(Color4B::WHITE);
	m_hexiiDistrictLabel->enableOutline(Color4B::BLACK, 1);
	m_hexiiDistrictLabel->enableShadow(Color4B::BLACK, Size::ZERO, 2);
	m_hexiiDistrictLabel->enableItalics();
	m_hexiiDistrictLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_hexiiDistrictLabel->setPosition(Vec2(190, 410));
	this->addChild(m_hexiiDistrictLabel, 1);

	/*
	/// Pin button

	m_pinButton = ui::Button::create("widgets/PinDisabled.png", "widgets/PinSelected.png", "widgets/PinDisabled.png");
	m_pinButton->setAnchorPoint(Vec2(0.5, 0.5));
	m_pinButton->setPosition(Vec2(390, 1303));
	m_pinButton->setScale(2.0f);
	m_pinButton->addTouchEventListener(CC_CALLBACK_2(HexiiInfoTab::onPinButtonPressed, this));
	*/

	/// Labels

	m_yieldLabel = CompoundLabel::create("Yield:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_yieldLabel->setStyle(true, true, LABEL_FONT_SIZE, Color4B::WHITE, Color4B::BLACK, 0, Color4B::BLACK, Size(4, -4), 4);
	m_yieldLabel->setStyle(false, true, LABEL_FONT_SIZE, INACTIVE_BLUE);
	m_yieldLabel->setIconTexture("icons/XBSmall.png");
	m_yieldLabel->setWidthConstrant(LABEL_WIDTH, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_yieldLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_yieldLabel->setPosition(Vec2(400, 350));
	this->addChild(m_yieldLabel, 1);

	// The label to the left of the yield time value (says "Every ")
	cocos2d::Label* yieldTimeLeft = Label::createWithTTF("Every ", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	yieldTimeLeft->setTextColor(Color4B::WHITE);
	yieldTimeLeft->enableShadow(Color4B::BLACK, Size(4, -4), 4);
	yieldTimeLeft->setAnchorPoint(Vec2(0.0, 0.5));
	yieldTimeLeft->setPosition(Vec2(400, 295));
	this->addChild(yieldTimeLeft, 1);

	// The label to the right of the yield time value (says " seconds")
	cocos2d::Label* yieldTimeRight = Label::createWithTTF(" seconds", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	yieldTimeRight->setTextColor(Color4B::WHITE);
	yieldTimeRight->enableShadow(Color4B::BLACK, Size(4, -4), 4);
	yieldTimeRight->setAnchorPoint(Vec2(1.0, 0.5));
	yieldTimeRight->setPosition(Vec2(400 + LABEL_WIDTH, 295));
	this->addChild(yieldTimeRight, 1);

	m_yieldSpeedLabel = Label::createWithTTF("", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	m_yieldSpeedLabel->setTextColor(INACTIVE_BLUE);
	m_yieldSpeedLabel->enableShadow(Color4B::BLACK, Size(4, -4), 4);
	m_yieldSpeedLabel->setAnchorPoint(Vec2(1.0, 0.5));
	m_yieldSpeedLabel->setPosition(Vec2(400 + LABEL_WIDTH - yieldTimeRight->getContentSize().width, 295));
	this->addChild(m_yieldSpeedLabel, 1);

	m_expLabel = CompoundLabel::create("XB:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_expLabel->setStyle(true, true, LABEL_FONT_SIZE, Color4B::WHITE, Color4B::BLACK, 0, Color4B::BLACK, Size(4, -4), 4);
	m_expLabel->setStyle(false, true, LABEL_FONT_SIZE, GENERAL_IMPORTANT_VALUE);
	m_expLabel->setIconTexture("icons/XBSmall.png");
	m_expLabel->setWidthConstrant(LABEL_WIDTH, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_expLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_expLabel->setPosition(Vec2(400, 240));
	this->addChild(m_expLabel, 1);

	m_nextBeeLabel = CompoundLabel::create("Next Bee:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_nextBeeLabel->setStyle(true, true, LABEL_FONT_SIZE, Color4B::WHITE, Color4B::BLACK, 0, Color4B::BLACK, Size(4, -4), 4);
	m_nextBeeLabel->setStyle(false, true, LABEL_FONT_SIZE, GENERAL_IMPORTANT_VALUE);
	m_nextBeeLabel->setIconTexture("icons/XBSmall.png");
	m_nextBeeLabel->setWidthConstrant(LABEL_WIDTH, LABEL_SPACING, CompoundLabel::VariablePartFloat::RIGHT);
	m_nextBeeLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_nextBeeLabel->setPosition(Vec2(400, 160));
	this->addChild(m_nextBeeLabel, 1);

	m_beeNameLabel = Label::createWithTTF("", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	m_beeNameLabel->enableShadow(Color4B::BLACK, Size(4, -4), 4);
	m_beeNameLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_beeNameLabel->setPosition(Vec2(400, 105));
	this->addChild(m_beeNameLabel, 1);

	m_beeCountLabel = Label::createWithTTF("", "fonts/BreeSerif.ttf", LABEL_FONT_SIZE, Size::ZERO, TextHAlignment::RIGHT);
	m_beeCountLabel->enableShadow(Color4B::BLACK, Size(4, -4), 4);
	m_beeCountLabel->setAnchorPoint(Vec2(1.0, 0.5));
	m_beeCountLabel->setPosition(Vec2(400 + LABEL_WIDTH, 105));
	this->addChild(m_beeCountLabel, 1);

	/// Purchase EXP button

	m_purchaseEXPButton = ui::Button::create("widgets/buttons/PurchaseEXPButtonNeutral.png", "widgets/buttons/PurchaseEXPButtonSelected.png", "widgets/buttons/PurchaseEXPButtonDisabled.png");
	m_purchaseEXPButton->setAnchorPoint(Vec2(0.5, 0.5));
	m_purchaseEXPButton->setPosition(Vec2(213, 940));
	m_purchaseEXPButton->addTouchEventListener(CC_CALLBACK_2(HexiiInfoTab::onPurchaseEXPButtonPressed, this));

	m_purchaseEXPNectarCostLabel = CompoundLabel::create("", "fonts/SpeakPro-Heavy.ttf", "fonts/SpeakPro-Heavy.ttf");
	m_purchaseEXPNectarCostLabel->setStyle(true, true, LABEL_FONT_SIZE, Color4B::WHITE, Color4B::BLACK, 1, Color4B::BLACK, Size(4, -4), 4);
	//m_purchaseEXPNectarCostLabel->setIconTexture("icons/Nectar.png");
	m_purchaseEXPNectarCostLabel->setSpacingConstraint(5.0f);
	m_purchaseEXPNectarCostLabel->setAnchorPoint(Vec2(1.0, 0.5));
	m_purchaseEXPNectarCostLabel->setPosition(Vec2(155, 45));

	m_purchaseEXPDesiredEXPLabel = CompoundLabel::create("", "fonts/SpeakPro-Heavy.ttf", "fonts/SpeakPro-Heavy.ttf");
	m_purchaseEXPDesiredEXPLabel->setStyle(true, true, LABEL_FONT_SIZE, Color4B::WHITE, Color4B::BLACK, 1, Color4B::BLACK, Size(4, -4), 4);
	//m_purchaseEXPDesiredEXPLabel->setIconTexture("icons/EXP.png");
	m_purchaseEXPDesiredEXPLabel->setSpacingConstraint(5.0f);
	m_purchaseEXPDesiredEXPLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_purchaseEXPDesiredEXPLabel->setPosition(Vec2(225, 45));

	m_purchaseEXPButton->addChild(m_purchaseEXPNectarCostLabel, 1);
	m_purchaseEXPButton->addChild(m_purchaseEXPDesiredEXPLabel, 1);
	this->addChild(m_purchaseEXPButton, 1);

	/// Upgrades

	m_upgradeScrollView = ui::ScrollView::create();
	m_upgradeScrollView->setAnchorPoint(Vec2(0.0, 0.0));
	m_upgradeScrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	m_upgradeScrollView->setContentSize(Size(415, 451)); // 415, 3625
	m_upgradeScrollView->setInnerContainerSize(Size(415, 451));
	m_upgradeScrollView->setBounceEnabled(true);
	m_upgradeScrollView->setPosition(Vec2(572, 0));
	//m_upgradeScrollView->setFlippedY(true);
	// Invisible scrollbar
	m_upgradeScrollView->setScrollBarOpacity(0);
	this->addChild(m_upgradeScrollView, 2);

	/// Events
	
	EventUtility::addGlobalEventListener("onHexiiFocus", this, &HexiiInfoTab::onHexiiFocus);

	setVisible(false);

	return true;
}

void HexiiInfoTab::update(float dt) {
	if (!m_focusHexii) return;

	/// Update labels

	// TODO: Add a setting to change the speed of UI updates
	updateHexiiLabels(false, true, true);	

	// Every 1/60th of a second, try and purchase EXP if held down
	//if (m_purchaseEXPButton->isEnabled() && m_purchaseEXPButton->isHighlighted() &&
	//	Director::getInstance()->getTotalFrames() % (int)std::ceil(Director::getInstance()->getFrameRate() / 60) == 0
	//	) purchaseEXP();

	/// Update upgrades
	for (auto& i : m_upgradeScrollView->getChildren()) i->update(dt);
}

void HexiiInfoTab::onHexiiFocus(cocos2d::EventCustom* evnt) {
	Hexii::EventHexiiFocusData* data = static_cast<Hexii::EventHexiiFocusData*>(evnt->getUserData());

	if (data->active) setFocusHexii(data->subject);
}

void HexiiInfoTab::onBeeGroupFocus(Ref*, cocos2d::ui::Widget::TouchEventType evntType, uint tabNumber) {
	setFocusBeeGroup(&m_focusHexii->getBeeGroups()[tabNumber]);
}

void HexiiInfoTab::onPurchaseEXPButtonPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
	// Only care about the release
	if (evntType != ui::Widget::TouchEventType::ENDED) return;

	if (m_purchaseEXPButton->isEnabled()) purchaseEXP();
}

/*

void HexiiInfoTab::onUpgradePurchased(cocos2d::EventCustom* evnt) {
	m_yieldLabel->setVariablePartString(formatBigReal(m_focus->getYield()));
}

void HexiiInfoTab::onBeeGained(cocos2d::EventCustom* evnt) {
	Hexii::EventBeeGainedData* data = static_cast<Hexii::EventBeeGainedData*>(evnt->getUserData());

	// Only update this box if the hex that leveled up was this tab's focus
	if (data->subject != m_focus) return;

	/// Update labels for hex stats

	m_beeCountLabel->setVariablePartString(std::to_string(data->beeCount));
	m_yieldLabel->setVariablePartString(formatBigReal(m_focus->getYield()));

	updateUpgradesList(data->beeCountBefore, data->beeCount);
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

void HexiiInfoTab::onBeeGained(cocos2d::EventCustom* evnt) {
	auto data = EventUtility::getEventData<BeeGroup::EventBeeGainedData>(evnt);

	updateBeeGroupLabels(false, true);
	if (data->changedStates.size() > 0) updateScrollView();
}

void HexiiInfoTab::setFocusHexii(Hexii* focus) {
	if (m_focusHexii == focus) return;

	/// Cleanup

	// Remove bee group tabs
	for (auto i : m_beeGroupTabButtons) i->removeFromParent();
	m_beeGroupTabButtons.clear();

	m_focusHexii = focus;

	// Quick return if no focus
	if (!m_focusHexii) {
		setFocusBeeGroup(nullptr);
		setVisible(false);

		return;
	}	

	/// Setup

	auto& beeGroups = m_focusHexii->getBeeGroups();
	// Setup the bee groups
	if (beeGroups.size() == 0) setFocusBeeGroup(nullptr);
	else for (auto& beeGroup : beeGroups) {
		// TODO: Make the bee group tabs

		// TODO: Remove
		setFocusBeeGroup(&beeGroup);
	}

	// Update properties that change with focus but not per frame
	m_yieldLabel->setIconTexture(m_focusHexii->getHexiiType() == Hexii::HexiiType::QueensChamber ? "icons/NectarSmall.png" : "icons/XBSmall.png");
	m_focusSprite->setTexture(m_focusHexii->getShadedRenderTexture());
	updateHexiiLabels(true, true, true);
}

void HexiiInfoTab::setFocusBeeGroup(BeeGroup* focus) {
	if (m_focusBeeGroup == focus) return;

	/// Cleanup

	// Remove listeners
	if (m_beeGainedListener) {
		_eventDispatcher->removeEventListener(m_beeGainedListener);
	}

	// Clear the old upgrades
	m_upgradeScrollView->removeAllChildrenWithCleanup(true);
	m_upgradeBoxes.clear();

	m_focusBeeGroup = focus;
	// Quick return if no focus
	if (!m_focusBeeGroup) return;

	/// Setup the new bee group

	// Add listeners
	m_beeGainedListener = EventUtility::addTargetedEventListener(BeeGroup::EVENT_BEE_GAINED, this, m_focusBeeGroup->eventID, &HexiiInfoTab::onBeeGained);

	// Update labels
	updateBeeGroupLabels(true, true);

	// Update upgrades list
	auto& upgrades = focus->upgradePath->getUpgrades();
	for (uint i = 0; i < upgrades.size(); i++) {
		BeeGroupUpgradeBox* box = BeeGroupUpgradeBox::create();

		box->setUpgrade(upgrades[i], m_focusBeeGroup);
		box->setAnchorPoint(Vec2(0, 0));
		box->setPosition(Vec2(i * (box->getContentSize().width + 40), 0)); // Width of a box + 40 points spacing
		m_upgradeBoxes.push_back(box);
		m_upgradeScrollView->addChild(box);
	}
	updateScrollView();
}

void HexiiInfoTab::updateHexiiLabels(bool nameLabel, bool yieldLabel, bool expLabels) {
	if (nameLabel) {
		m_hexiiNameLabel->setString(m_focusHexii->getName());
		m_hexiiDistrictLabel->setString("District " + std::to_string(m_focusHexii->getDistrict()));
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
		BigReal expForNextBee = m_focusBeeGroup->getEXPForNextBee() - exp;

		m_expLabel->setVariablePartString(formatBigReal(exp) /* + " / " + formatBigReal(exp + expForNextBee)*/);
		m_nextBeeLabel->setVariablePartString(formatBigReal(exp + expForNextBee));

		/// Update button labels

		// Since exp costs increase after a gaining a bee, only buy at most the exp required to reach the next beeCount
		BigReal desiredEXP = std::min(std::floorl(std::max(expForNextBee * 0.05, (BigReal)1.0)), expForNextBee);
		BigReal costToPurchaseDesiredEXP = desiredEXP * m_focusHexii->getEXPCost();

		if (desiredEXP != m_purchaseEXPButtonDesiredEXP || costToPurchaseDesiredEXP != m_costToPurchaseDesiredEXP) {
			m_purchaseEXPButtonDesiredEXP = desiredEXP;
			m_costToPurchaseDesiredEXP = costToPurchaseDesiredEXP;

			// Update the relevant labels
			m_purchaseEXPDesiredEXPLabel->setVariablePartString(formatBigReal(desiredEXP));
			m_purchaseEXPNectarCostLabel->setVariablePartString(formatBigReal(costToPurchaseDesiredEXP));			
		}

		/// Update button state

		// If the exp provided by the button is unaffordable, grey out the button. Otherwise, enabled it again
		if (Resources::getInstance()->getNectar() < costToPurchaseDesiredEXP) {
			if (m_purchaseEXPButton->isEnabled()) {
				m_purchaseEXPButton->setBright(false);
				m_purchaseEXPButton->setEnabled(false);
			}
		}
		else if (!m_purchaseEXPButton->isEnabled()) {
			m_purchaseEXPButton->setBright(true);
			m_purchaseEXPButton->setEnabled(true);
		}
	}
}

void HexiiInfoTab::updateBeeGroupLabels(bool nameLabel, bool countLabel) {
	if(nameLabel) m_beeNameLabel->setString(m_focusBeeGroup->getBeeNameFriendly() + "s:");
	if(countLabel) m_beeCountLabel->setString(std::to_string(m_focusBeeGroup->getBeeCount()));
}

void HexiiInfoTab::updateScrollView() {
	auto& upgradeStates = m_focusBeeGroup->getUpgradeStates();
	bool updated = false;

	// Iterate over the upgrade states until a locked one
	for (uint i = 0; i < upgradeStates.size(); i++) {
		if (upgradeStates[i] == BeeGroupUpgradeBox::State::LOCKED) {
			// Assume all following upgrades are also locked, therefore use this to set the size of the scroll view
			m_upgradeScrollView->setContentSize(Size(
				i * (m_upgradeBoxes[i]->getContentSize().width + 40), 
				m_upgradeScrollView->getContentSize().height
			));
			updated = true;
			break;
		}
	}

	// If no locked upgrades were found, then the scroll view should be the full width
	if (!updated) m_upgradeScrollView->setContentSize(Size(
		upgradeStates.size() * (m_upgradeBoxes[0]->getContentSize().width + 40),
		m_upgradeScrollView->getContentSize().height
	));
}

void HexiiInfoTab::purchaseEXP() {
	BigReal cost = m_purchaseEXPButtonDesiredEXP * m_focusHexii->getEXPCost();

	// Pay
	Resources::getInstance()->addNectar(-cost);

	m_focusHexii->addEXP(m_purchaseEXPButtonDesiredEXP);
}

/*
BeeGroupUpgradeBox* HexiiInfoTab::addUpgradeToList(UpgradePtr upgrade) {
	// TODO: This function should add all upgrades in one go and then just change the visibility later

	BeeGroupUpgradeBox* upgradeBox = BeeGroupUpgradeBox::create();
	BeeGroupUpgradeBox::State state;

	// Set the state of the upgrade box (whether it's locked, revealed or fully purchased)

	if (upgrade->beeRequirement > m_focus->getLevel()) state = BeeGroupUpgradeBox::State::LOCKED;
	else if (m_focus->getUpgrade(fmt::format(upgrade->name))) state = BeeGroupUpgradeBox::State::PURCHASED;
	else state = BeeGroupUpgradeBox::State::REVEALED;

	upgradeBox->setUpgrade(upgrade, m_focus, state);
	upgradeBox->setAnchorPoint(Vec2(0, 1));
	upgradeBox->setPosition(Vec2(0, 160 * m_upgradeScrollView->getChildrenCount()));

	m_upgradeScrollView->addChild(upgradeBox);

	return upgradeBox;
}


void HexiiInfoTab::updateUpgradesList(BigInt beeCountBefore, BigInt beeCount) {
	// Number of preview upgrade boxes that should be displayed
	constexpr int PREVIEW_COUNT = 2;

	// Update the locked boxes

	// The list of preview boxes after this function has updated the list
	std::vector<BeeGroupUpgradeBox*> updatedPreviewUpgradeBoxes;
	for (uint i = 0; i < m_previewUpgradeBoxes.size(); i++) {
		// If a preview box has reached the reveal beeCount, reveal it. Otherwise, it remains a preview so it stays in the updated preview \
		boxes list

		if (beeCount >= m_previewUpgradeBoxes[i]->getUpgrade()->beeRequirement) m_previewUpgradeBoxes[i]->setState(BeeGroupUpgradeBox::State::REVEALED);
		else updatedPreviewUpgradeBoxes.push_back(m_previewUpgradeBoxes[i]);
	}

	// If there are no more locked boxes and all upgrades are available, this function is done
	if (updatedPreviewUpgradeBoxes.size() == 0 && m_upgradeScrollView->getChildrenCount() == Upgrades::UPGRADE_COUNT) {
		m_previewUpgradeBoxes = updatedPreviewUpgradeBoxes;
		return;
	}

	/// Grabs all upgrades that are currently available for purchase, plus the next two

	Upgrades::UpgradeList list = Upgrades::getUpgradesBetweenLevels(beeCountBefore + 1, beeCount);

	// The latest upgrade referenced so far
	UpgradePtr lastUpgrade = nullptr;
	if (updatedPreviewUpgradeBoxes.size() > 0) lastUpgrade = updatedPreviewUpgradeBoxes.back()->getUpgrade();
	else if (list.size() > 0) lastUpgrade = list.back();

	// The total number of locked boxes should be PREVIEW_COUNT, so this bonus list increases up to that
	Upgrades::UpgradeList previewUpgradeList = Upgrades::getUpgradesFollowing(lastUpgrade, PREVIEW_COUNT - updatedPreviewUpgradeBoxes.size());

	// When a beeCount up happens, a locked upgrade may become available. If this happens, the upgrade is present in locked boxes\
	and it is also returned in `list`. This upgrade shouldn't be added again, so i needs to start offsetted based on how many of \
	these newly unlocked ones there are. Everything else afterwards is to be appended
	for (unsigned int i = m_previewUpgradeBoxes.size() - updatedPreviewUpgradeBoxes.size(); i < list.size(); i++) {
		addUpgradeToList(list[i]);
	}

	for (unsigned int i = 0; i < previewUpgradeList.size(); i++) {
		updatedPreviewUpgradeBoxes.push_back(addUpgradeToList(previewUpgradeList[i]));
	}

	/// Update container size

	// Used to preserve the scroll position. 100% means top, not bottom (because the y is inverted). NaN is the initial scroll value \
	before user input
	float percentScrolled = m_upgradeScrollView->getScrolledPercentVertical();
	if (!(percentScrolled >= 0.0f)) { // Meaning NaN
		percentScrolled = 100.0f;
	}
	// TODO: Optimize
	// Updated size after new boxes have been added
	Size newScrollViewSize = m_upgradeScrollView->getInnerContainerSize();

	// Height is not allowed to go below the content height
	newScrollViewSize.height = std::max((long)m_upgradeScrollView->getContentSize().height, (long)(160 * m_upgradeScrollView->getChildrenCount()));
	
	m_upgradeScrollView->setInnerContainerSize(newScrollViewSize);
	if(percentScrolled >= 0) m_upgradeScrollView->jumpToPercentVertical(percentScrolled);

	m_previewUpgradeBoxes = updatedPreviewUpgradeBoxes;
}
*/