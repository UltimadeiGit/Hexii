#include "HexInfoTab.h"
#include "Resources.h"

USING_NS_CC;

HexInfoTab::HexInfoTab() {}

bool HexInfoTab::init() {
	setContentSize(Size(420, 1320));

	/// Background overlay

	m_backgroundOverlay = Sprite::create("components/HexInfoTabBackgroundOverlay.png");
	m_backgroundOverlay->setAnchorPoint(Vec2(0, 0));
	m_backgroundOverlay->setPosition(Vec2(1, 591));

	/// Hex sprite

	m_focusSprite = Sprite::create("gameplay/HexInactive.png");
	m_focusSprite->setContentSize(Size(283 * HEXAGON_HEIGHT_TO_WIDTH, 283));
	m_focusSprite->setAnchorPoint(Vec2(0.5, 0.5));
	m_focusSprite->setPosition(Vec2(213, 1144));
	m_focusSprite->setFlippedY(true);

	m_layerLabel = Label::createWithTTF("--", "fonts/SpeakPro-Heavy.ttf", 72, Size::ZERO, TextHAlignment::CENTER);
	m_layerLabel->setTextColor(Color4B::WHITE);
	m_layerLabel->enableOutline(Color4B::BLACK, 1);
	m_layerLabel->enableShadow(Color4B::BLACK, Size::ZERO, 12);
	m_layerLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_layerLabel->setPosition(Vec2(283 * HEXAGON_HEIGHT_TO_WIDTH * 0.5, 283 * 0.5));

	m_focusSprite->addChild(m_layerLabel);

	/// Pin button

	m_pinButton = ui::Button::create("widgets/PinDisabled.png", "widgets/PinSelected.png", "widgets/PinDisabled.png");
	m_pinButton->setAnchorPoint(Vec2(0.5, 0.5));
	m_pinButton->setPosition(Vec2(390, 1303));
	m_pinButton->setScale(2.0f);
	m_pinButton->addTouchEventListener(CC_CALLBACK_2(HexInfoTab::onPinButtonPressed, this));

	/// Labels

	m_levelLabel = CompoundLabel::create("Level ", "fonts/SpeakPro-Heavy.ttf", "fonts/SpeakPro-Heavy.ttf");
	m_levelLabel->setStyle(true, true, 48, Color4B::WHITE, Color4B::BLACK, 1, Color4B::BLACK, Size(2, -2), 2);
	m_levelLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_levelLabel->setPosition(Vec2(22, 795));

	m_expLabel = CompoundLabel::create("EXP:", "fonts/SpeakPro-Heavy.ttf", "fonts/SpeakPro-Heavy.ttf");
	m_expLabel->setStyle(true, true, 48, Color4B::WHITE, Color4B::BLACK, 1, Color4B::BLACK, Size(2, -2), 2);
	m_expLabel->setIconTexture("icons/EXP.png");
	m_expLabel->setSpacing(15.0f);
	m_expLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_expLabel->setPosition(Vec2(22, 735));

	m_yieldLabel = CompoundLabel::create("Yield:", "fonts/SpeakPro-Heavy.ttf", "fonts/SpeakPro-Heavy.ttf");
	m_yieldLabel->setStyle(true, true, 48, Color4B::WHITE, Color4B::BLACK, 1, Color4B::BLACK, Size(2, -2), 2);
	m_yieldLabel->setIconTexture("icons/GreenMatter.png");
	m_yieldLabel->setSpacing(15.0f);
	m_yieldLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_yieldLabel->setPosition(Vec2(22, 675));

	/// Purchase EXP button

	// TODO: Why is the functionality for this button not in a callback?
	m_purchaseEXPButton = ui::Button::create("widgets/buttons/PurchaseEXPButtonNeutral.png", "widgets/buttons/PurchaseEXPButtonSelected.png", "widgets/buttons/PurchaseEXPButtonDisabled.png");
	m_purchaseEXPButton->setAnchorPoint(Vec2(0.5, 0.5));
	m_purchaseEXPButton->setPosition(Vec2(213, 940));
	m_purchaseEXPButton->addTouchEventListener(CC_CALLBACK_2(HexInfoTab::onPurchaseEXPButtonPressed, this));

	m_purchaseEXPGreenMatterCostLabel = CompoundLabel::create("", "fonts/SpeakPro-Heavy.ttf", "fonts/SpeakPro-Heavy.ttf");
	m_purchaseEXPGreenMatterCostLabel->setStyle(true, true, 32, Color4B::WHITE, Color4B::BLACK, 1, Color4B::BLACK, Size(2, -2), 2);
	m_purchaseEXPGreenMatterCostLabel->setIconTexture("icons/GreenMatter.png");
	m_purchaseEXPGreenMatterCostLabel->setSpacing(5.0f);
	m_purchaseEXPGreenMatterCostLabel->setAnchorPoint(Vec2(1.0, 0.5));
	m_purchaseEXPGreenMatterCostLabel->setPosition(Vec2(155, 45));

	m_purchaseEXPDesiredEXPLabel = CompoundLabel::create("", "fonts/SpeakPro-Heavy.ttf", "fonts/SpeakPro-Heavy.ttf");
	m_purchaseEXPDesiredEXPLabel->setStyle(true, true, 32, Color4B::WHITE, Color4B::BLACK, 1, Color4B::BLACK, Size(2, -2), 2);
	m_purchaseEXPDesiredEXPLabel->setIconTexture("icons/EXP.png");
	m_purchaseEXPDesiredEXPLabel->setSpacing(5.0f);
	m_purchaseEXPDesiredEXPLabel->setAnchorPoint(Vec2(0.0, 0.5));
	m_purchaseEXPDesiredEXPLabel->setPosition(Vec2(225, 45));

	m_purchaseEXPButton->addChild(m_purchaseEXPGreenMatterCostLabel);
	m_purchaseEXPButton->addChild(m_purchaseEXPDesiredEXPLabel);

	/// Upgrades

	m_upgradeScrollView = ui::ScrollView::create();
	m_upgradeScrollView->setAnchorPoint(Vec2(0.0, 0.0));
	m_upgradeScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	m_upgradeScrollView->setContentSize(Size(415, 451)); // 415, 3625
	m_upgradeScrollView->setInnerContainerSize(Size(415, 451));
	m_upgradeScrollView->setBounceEnabled(true);
	m_upgradeScrollView->setPosition(Vec2(0, 572));
	m_upgradeScrollView->setFlippedY(true);
	// Invisible scrollbar
	m_upgradeScrollView->setScrollBarOpacity(0);

	/// Events

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexLevelUp", CC_CALLBACK_1(HexInfoTab::onHexLevelUp, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexFocus", CC_CALLBACK_1(HexInfoTab::onHexFocus, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexUpgradePurchase", CC_CALLBACK_1(HexInfoTab::onHexUpgradePurchase, this)), this);

	setVisible(false);

	this->addChild(m_backgroundOverlay);
	this->addChild(m_pinButton);
	this->addChild(m_focusSprite);
	this->addChild(m_levelLabel);
	this->addChild(m_expLabel);
	this->addChild(m_yieldLabel);
	this->addChild(m_purchaseEXPButton);
	this->addChild(m_upgradeScrollView);

	return true;
}

void HexInfoTab::update(float dt) {
	if (!m_focus) return;

	// Useful variables that may change every frame

	BigReal exp = m_focus->getEXP();
	BigReal expDistanceFromNextLevel = m_focus->getEXPRequiredForNextLevel() - m_focus->getTotalEXP();
	BigReal expRequiredForNextLevel = exp + expDistanceFromNextLevel;

	/// Update labels

	m_expLabel->setVariablePartString(formatBigReal(exp) + " / " + formatBigReal(expRequiredForNextLevel));

	/// Update progress bar

	// Ratio of current exp into this level over the exp needed to level up

	/// Update buttons

	m_purchaseEXPButtonDesiredEXP = expRequiredForNextLevel * 0.05;
	if (m_purchaseEXPButtonDesiredEXP < 1) m_purchaseEXPButtonDesiredEXP = 1;
	m_purchaseEXPButtonDesiredEXP = std::floorf(m_purchaseEXPButtonDesiredEXP);
	// Since exp costs increase after a level up, only buy at most the exp required to reach the next level
	if (m_purchaseEXPButtonDesiredEXP > expDistanceFromNextLevel) m_purchaseEXPButtonDesiredEXP = expDistanceFromNextLevel;

	// Update the corersponding label
	m_purchaseEXPDesiredEXPLabel->setVariablePartString(formatBigReal(m_purchaseEXPButtonDesiredEXP));

	BigReal costToPurchaseDesiredEXP = m_purchaseEXPButtonDesiredEXP * m_focus->getEXPCost();
	m_purchaseEXPGreenMatterCostLabel->setVariablePartString(formatBigReal(costToPurchaseDesiredEXP));

	// If the exp provided by the button is unaffordable, grey out the button. Otherwise, enabled it again
	if (Resources::getInstance()->getGreenMatter() < costToPurchaseDesiredEXP) {
		if (m_purchaseEXPButton->isEnabled()) {
			m_purchaseEXPButton->setBright(false);
			m_purchaseEXPButton->setEnabled(false);
		}
	}
	else if (!m_purchaseEXPButton->isEnabled()) {
		m_purchaseEXPButton->setBright(true);
		m_purchaseEXPButton->setEnabled(true);
	}

	// Every 1/60th of a second, try and purchase EXP if held down
	//if (m_purchaseEXPButton->isEnabled() && m_purchaseEXPButton->isHighlighted() &&
	//	Director::getInstance()->getTotalFrames() % (int)std::ceil(Director::getInstance()->getFrameRate() / 60) == 0
	//	) purchaseEXP();

	/// Update upgrades

	for (auto& i : m_upgradeScrollView->getChildren()) i->update(dt);
}

void HexInfoTab::onHexLevelUp(cocos2d::EventCustom* evnt) {
	Hex::EventHexLevelUpData* data = static_cast<Hex::EventHexLevelUpData*>(evnt->getUserData());

	// Only update this box if the hex that leveled up was this tab's focus
	if (data->subject != m_focus) return;

	/// Update labels for hex stats

	m_levelLabel->setVariablePartString(std::to_string(data->level));
	m_yieldLabel->setVariablePartString(formatBigReal(m_focus->getYield()));

	updateUpgradesList(data->levelBefore, data->level);
}

void HexInfoTab::onHexFocus(cocos2d::EventCustom* evnt) {
	Hex::EventHexFocusData* data = static_cast<Hex::EventHexFocusData*>(evnt->getUserData());

	// TODO: Implement locking
	if (data->active) setFocus(data->subject);
}

void HexInfoTab::onHexUpgradePurchase(cocos2d::EventCustom* evnt) {
	m_yieldLabel->setVariablePartString(formatBigReal(m_focus->getYield()));
}

void HexInfoTab::onPurchaseEXPButtonPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
	// Only care about the release
	if (evntType != ui::Widget::TouchEventType::ENDED) return;

	if (m_purchaseEXPButton->isEnabled()) purchaseEXP();
}

void HexInfoTab::onPinButtonPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
	// Only care about the release
	if (evntType != ui::Widget::TouchEventType::ENDED) return;

	// Pin / unpin

	m_pinned = !m_pinned;

	if(m_pinned) m_pinButton->loadTextureNormal("widgets/PinActive.png"); 
	else m_pinButton->loadTextureNormal("widgets/PinDisabled.png");

	_eventDispatcher->dispatchCustomEvent("onPinButtonPressed", new bool(m_pinned));
}

void HexInfoTab::setFocus(Hex* focus) {
	// TODO: This function is *slow*. Needs optimization

	if (m_focus == focus || m_pinned) return;

	m_focus = focus;

	if (!m_focus) return setVisible(false);

	BigInt level = m_focus->getLevel();
	uint layer = m_focus->getLayer();

	// Update variables that change with focus but not per frame

	m_layerLabel->setString("L" + std::to_string(layer));
	m_yieldLabel->setIconTexture(layer == 0 ? "icons/GreenMatter.png" : "icons/EXP.png");
	m_yieldLabel->setVariablePartString(formatBigReal(m_focus->getYield()));
	m_levelLabel->setVariablePartString(std::to_string(level));
	m_focusSprite->setTexture(m_focus->getShadedRenderTexture());

	m_upgradeScrollView->removeAllChildrenWithCleanup(true);
	m_previewUpgradeBoxes.clear();
	updateUpgradesList(0, level);
}

void HexInfoTab::purchaseEXP() {
	BigReal cost = m_purchaseEXPButtonDesiredEXP * m_focus->getEXPCost();

	// Pay
	Resources::getInstance()->addGreenMatter(-cost);

	m_focus->addEXP(m_purchaseEXPButtonDesiredEXP);
}

HexUpgradeBox* HexInfoTab::addUpgradeToList(UpgradePtr upgrade) {
	// TODO: This function should add all upgrades in one go and then just change the visibility later

	HexUpgradeBox* upgradeBox = HexUpgradeBox::create();
	HexUpgradeBox::State state;

	// Set the state of the upgrade box (whether it's locked, revealed or fully purchased)

	if (upgrade->unlockLevel > m_focus->getLevel()) state = HexUpgradeBox::State::LOCKED;
	else if (m_focus->getUpgrade(fmt::format(upgrade->name))) state = HexUpgradeBox::State::PURCHASED;
	else state = HexUpgradeBox::State::REVEALED;

	upgradeBox->setUpgrade(upgrade, m_focus, state);
	upgradeBox->setAnchorPoint(Vec2(0, 1));
	upgradeBox->setPosition(Vec2(0, 160 * m_upgradeScrollView->getChildrenCount()));

	m_upgradeScrollView->addChild(upgradeBox);

	return upgradeBox;
}


void HexInfoTab::updateUpgradesList(BigInt levelBefore, BigInt level) {
	// Number of preview upgrade boxes that should be displayed
	constexpr int PREVIEW_COUNT = 2;

	// Update the locked boxes

	// The list of preview boxes after this function has updated the list
	std::vector<HexUpgradeBox*> updatedPreviewUpgradeBoxes;
	for (uint i = 0; i < m_previewUpgradeBoxes.size(); i++) {
		// If a preview box has reached the reveal level, reveal it. Otherwise, it remains a preview so it stays in the updated preview \
		boxes list

		if (level >= m_previewUpgradeBoxes[i]->getUpgrade()->unlockLevel) m_previewUpgradeBoxes[i]->setState(HexUpgradeBox::State::REVEALED);
		else updatedPreviewUpgradeBoxes.push_back(m_previewUpgradeBoxes[i]);
	}

	// If there are no more locked boxes and all upgrades are available, this function is done
	if (updatedPreviewUpgradeBoxes.size() == 0 && m_upgradeScrollView->getChildrenCount() == Upgrades::UPGRADE_COUNT) {
		m_previewUpgradeBoxes = updatedPreviewUpgradeBoxes;
		return;
	}

	/// Grabs all upgrades that are currently available for purchase, plus the next two

	Upgrades::UpgradeList list = Upgrades::getUpgradesBetweenLevels(levelBefore + 1, level);

	// The latest upgrade referenced so far
	UpgradePtr lastUpgrade = nullptr;
	if (updatedPreviewUpgradeBoxes.size() > 0) lastUpgrade = updatedPreviewUpgradeBoxes.back()->getUpgrade();
	else if (list.size() > 0) lastUpgrade = list.back();

	// The total number of locked boxes should be PREVIEW_COUNT, so this bonus list increases up to that
	Upgrades::UpgradeList previewUpgradeList = Upgrades::getUpgradesFollowing(lastUpgrade, PREVIEW_COUNT - updatedPreviewUpgradeBoxes.size());

	// When a level up happens, a locked upgrade may become available. If this happens, the upgrade is present in locked boxes\
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