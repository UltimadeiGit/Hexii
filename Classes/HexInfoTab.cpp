#include "HexInfoTab.h"
#include "Currencies.h"

USING_NS_CC;

HexInfoTab::HexInfoTab() : m_focus(nullptr), m_hexEXPBar(nullptr), m_focusSprite(nullptr), m_layerLabel(nullptr), m_levelLabel(nullptr),
	m_expLabel(nullptr), m_yieldLabel(nullptr), m_upgradesList(nullptr) {}

bool HexInfoTab::init() {
	setContentSize(Size(420, 1320));

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

	m_purchaseEXPButton = ui::Button::create("widgets/buttons/ButtonEXPPurchaseNeutral.png", "widgets/buttons/ButtonEXPPurchaseSelected.png", "widgets/buttons/ButtonEXPPurchaseDisabled.png");
	m_purchaseEXPButton->setAnchorPoint(Vec2(0.5, 0.5));
	m_purchaseEXPButton->setPosition(Vec2(315, 1106));

	m_upgradesList = ui::ScrollView::create();
	m_upgradesList->setAnchorPoint(Vec2(0.0, 0.0));
	m_upgradesList->setDirection(ui::ScrollView::Direction::VERTICAL);
	m_upgradesList->setInnerContainerSize(Size(415, 625)); // 415, 725
	m_upgradesList->setContentSize(Size(415, 625)); // 415, 3625
	m_upgradesList->setBounceEnabled(true);
	m_upgradesList->setPosition(Vec2(0, 864));
	m_upgradesList->setFlippedY(true);
	// Invisible scrollbar
	m_upgradesList->setScrollBarOpacity(0);
	//m_upgradesList->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	//m_upgradesList->setBackGroundColor(Color3B::BLACK);

	this->addChild(m_hexEXPBar);
	this->addChild(m_focusSprite);
	this->addChild(m_layerLabel);
	this->addChild(m_levelLabel);
	this->addChild(m_expLabel);
	this->addChild(m_yieldLabel);
	this->addChild(m_purchaseEXPButton);
	this->addChild(m_upgradesList);

	return true;
}

void HexInfoTab::update(float dt) {
	if (m_focus == nullptr) {
		if(isVisible()) setVisible(false);
		return;
	}

	BigInt level = m_focus->getLevel();
	BigReal exp = m_focus->getEXP();
	BigReal expRequiredForNextLevel = m_focus->getEXPRequiredForLevel(level + 1);
	BigReal expDistanceFromNextLevel = expRequiredForNextLevel - exp;

	/// Update labels for hex stats

	m_levelLabel->setVariablePartString(std::to_string(level));
	m_expLabel->setVariablePartString(formatBigReal(exp));
	m_yieldLabel->setVariablePartString(formatBigReal(m_focus->getYield()));
	
	/// Update progress bar and sprite

	// Ratio of current exp into this level over the exp needed to level up
	m_hexEXPBar->setProgress(exp / expRequiredForNextLevel);
	m_focusSprite->setTexture(m_focus->getShadedRenderTexture());

	/// Update upgrades

	for (auto& i : m_upgradesList->getChildren()) i->update(dt);

	// Level changed
	if (level > m_storedFocuslevel) {
		updateUpgradesList();
		m_storedFocuslevel = level;
	}

	/// Update buttons

	m_purchaseEXPButtonDesiredEXP = expRequiredForNextLevel * 0.01;
	if (m_purchaseEXPButtonDesiredEXP < 1) m_purchaseEXPButtonDesiredEXP = 1;
	m_purchaseEXPButtonDesiredEXP = std::floor(m_purchaseEXPButtonDesiredEXP);
	// Since exp costs increase after a level up, only buy at most the exp required to reach the next level
	if (m_purchaseEXPButtonDesiredEXP > expDistanceFromNextLevel) m_purchaseEXPButtonDesiredEXP = expDistanceFromNextLevel;

	// If the exp provided by the button is unaffordable, grey out the button. Otherwise, enabled it again
	if (Currencies::getGreenMatter() < m_purchaseEXPButtonDesiredEXP * m_focus->getEXPCost()) {
		if (m_purchaseEXPButton->isEnabled()) {
			m_purchaseEXPButton->setBright(false);
			m_purchaseEXPButton->setEnabled(false);
		}
	}
	else if (!m_purchaseEXPButton->isEnabled()) {
		m_purchaseEXPButton->setBright(true);
		m_purchaseEXPButton->setEnabled(true);
	}

	if (m_purchaseEXPButton->isEnabled() && m_purchaseEXPButton->isHighlighted()) {
		// Every 1/60th of a second, try and purchase EXP if held down
		if (Director::getInstance()->getTotalFrames() % (int)std::ceil(Director::getInstance()->getFrameRate() / 60) == 0) {
			purchaseEXP();
		}
	}
}

void HexInfoTab::setFocus(Hex* focus) {
	if (m_focus == focus) return;

	m_focus = focus;
	// If a focus exists, visible otherwise not
	setVisible(m_focus);
	if (!m_focus) return;

	// Update variables that change with focus but not per frame

	uint layer = m_focus->getLayer();

	m_storedFocuslevel = 0;
	m_layerLabel->setString("L" + std::to_string(layer));
	m_yieldLabel->setIconTexture(layer == 0 ? "icons/GreenMatter.png" : "icons/EXP.png");

	m_upgradesList->removeAllChildrenWithCleanup(true);
	m_lockedBoxes.clear();
	updateUpgradesList();

	m_storedFocuslevel = m_focus->getLevel();
}

void HexInfoTab::purchaseUpgrade(UpgradePtr upgrade) {
	m_focus->unlockUpgrade(std::string(upgrade->name));
}

void HexInfoTab::purchaseEXP() {
	BigReal cost = m_purchaseEXPButtonDesiredEXP * m_focus->getEXPCost();

	// Pay
	Currencies::instance()->addGreenMatter(-cost);

	m_focus->addEXP(m_purchaseEXPButtonDesiredEXP);
}

HexUpgradeBox* HexInfoTab::addUpgradeToList(UpgradePtr upgrade) {
	HexUpgradeBox* upgradeBox = HexUpgradeBox::create();
	HexUpgradeBox::State state;

	if (upgrade->unlockLevel > m_focus->getLevel()) state = HexUpgradeBox::State::LOCKED;
	else if (m_focus->getUpgrade(std::string(upgrade->name))) state = HexUpgradeBox::State::PURCHASED;
	else state = HexUpgradeBox::State::REVEALED;

	upgradeBox->setUpgrade(upgrade, state);
	upgradeBox->setAnchorPoint(Vec2(0, 1));
	upgradeBox->setPosition(Vec2(0, 160 * m_upgradesList->getChildrenCount()));
	upgradeBox->purchaseUpgradeFunction = CC_CALLBACK_1(HexInfoTab::purchaseUpgrade, this);

	m_upgradesList->addChild(upgradeBox);

	return upgradeBox;
}


void HexInfoTab::updateUpgradesList() {
	constexpr int PREVIEW_COUNT = 2;

	BigInt level = m_focus->getLevel();

	// Update the locked boxes

	std::vector<HexUpgradeBox*> updatedLockedBoxes;
	for (uint i = 0; i < m_lockedBoxes.size(); i++) {
		if (level >= m_lockedBoxes[i]->getUpgrade()->unlockLevel) m_lockedBoxes[i]->setState(HexUpgradeBox::State::REVEALED);
		else updatedLockedBoxes.push_back(m_lockedBoxes[i]);
	}

	// If there are no more locked boxes and all upgrades are available, this function is done
	if (updatedLockedBoxes.size() == 0 && m_upgradesList->getChildrenCount() == Upgrades::getUpgradeCount()) {
		m_lockedBoxes = updatedLockedBoxes;
		return;
	}

	/// Grabs all upgrades that are currently available for purchase, plus the next two

	Upgrades::UpgradeList list = Upgrades::getUpgradesBetweenLevels(m_storedFocuslevel + 1, level);

	// The latest upgrade referenced so far
	UpgradePtr lastUpgrade = nullptr;
	if (updatedLockedBoxes.size() > 0) lastUpgrade = updatedLockedBoxes.back()->getUpgrade();
	else if (list.size() > 0) lastUpgrade = list.back();

	// The total number of locked boxes should be PREVIEW_COUNT, so this bonus list increases up to that
	Upgrades::UpgradeList bonusList = Upgrades::getUpgradesFollowing(lastUpgrade, PREVIEW_COUNT - updatedLockedBoxes.size());

	// When a level up happens, a locked upgrade may become available. If this happens, the upgrade is present in locked boxes\
	and it is also returned in `list`. This upgrade shouldn't be added again, so i needs to start offsetted based on how many of \
	these newly unlocked ones there are. Everything else afterwards is to be appended
	for (unsigned int i = m_lockedBoxes.size() - updatedLockedBoxes.size(); i < list.size(); i++) {
		addUpgradeToList(list[i]);
	}

	for (unsigned int i = 0; i < bonusList.size(); i++) {
		updatedLockedBoxes.push_back(addUpgradeToList(bonusList[i]));
	}

	m_lockedBoxes = updatedLockedBoxes;
}