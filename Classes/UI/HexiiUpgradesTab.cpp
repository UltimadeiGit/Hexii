#include "HexiiUpgradesTab.h"
#include "UICommon.h"

USING_NS_CC;

HexiiUpgradesTab::HexiiUpgradesTab(UpgradePathPtr focusPath) : m_focusPath(focusPath){}

bool HexiiUpgradesTab::init()
{
	/// Scroll view

	m_upgradeScrollView = ui::ScrollView::create();
	m_upgradeScrollView->setAnchorPoint(Vec2(0.0, 0.0));
	m_upgradeScrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	m_upgradeScrollView->setContentSize(UICommon::SCROLLVIEW_CONTENT_SIZE);
	m_upgradeScrollView->setInnerContainerSize({ UICommon::PURCHASABLE_BOX_WIDTH, UICommon::SCROLLVIEW_CONTENT_SIZE.height });
	m_upgradeScrollView->setBounceEnabled(true);
	m_upgradeScrollView->setPosition({0, 0});
	//m_upgradeScrollView->setFlippedY(true);
	// Invisible scrollbar
	m_upgradeScrollView->setScrollBarOpacity(0);
	this->addChild(m_upgradeScrollView, 2);

	/// Upgrade boxes

	auto& upgrades = m_focusPath->getUpgrades();

	for (uint i = 0; i < upgrades.size(); i++) {
		UpgradeBox* box = UpgradeBox::create();
		box->setUpgrade(upgrades[i], nullptr, nullptr);
		box->setAnchorPoint({0, 0});
		box->setPosition({(UICommon::PURCHASABLE_BOX_WIDTH + UICommon::PURCHASABLE_BOX_SPACING) * i, 0});
		box->setVisible(false);

		m_upgradeBoxes.push_back(box);
		m_upgradeScrollView->addChild(box);
	}

    return true;
}

void HexiiUpgradesTab::update(float dt) {
	/// Update upgrades
	for (auto& i : m_upgradeScrollView->getChildren()) i->update(dt);
	updateTrackerStoredScrollViewPos();
}

void HexiiUpgradesTab::setFocus(Hexii* focus) {
	updateTrackerStoredScrollViewPos();

	HexiiTab::setFocus(focus);
	m_focusTracker = focus->getUpgradeTrackerFromPath(m_focusPath);

	if (m_focusTracker != nullptr) {
		updateBoxes();
		updateStates();
	}
}

void HexiiUpgradesTab::updateStates() {
	if (m_focusTracker == nullptr) return;

	// Number of preview upgrade boxes that should be displayed
	constexpr int PREVIEW_COUNT = 1;

	auto& upgradeStates = m_focusTracker->getStates();
	// The number of locked states encountered so far
	uint numLocked = 0;
	uint numVisible = 0;

	// Iterate over the upgrade states until a locked one
	for (uint i = 0; i < upgradeStates.size(); i++) {
		if (upgradeStates[i] == Upgrade::State::LOCKED) {
			// Make the first PREVIEW_COUNT upgrade boxes visible
			if (numLocked < PREVIEW_COUNT) {
				m_upgradeBoxes[i]->setVisible(true);
				numVisible++;
			} else m_upgradeBoxes[i]->setVisible(false);
			numLocked++;
		}
		else {
			// If the upgrade is not locked, then it should be visible
			m_upgradeBoxes[i]->setVisible(true);
			numVisible++;
		}

		// Update upgrade box
		m_upgradeBoxes[i]->setState(upgradeStates[i]);
	}

	// Update scroll view based on how many upgrade boxes are visible
	m_upgradeScrollView->setInnerContainerSize(Size(
		numVisible * (UICommon::PURCHASABLE_BOX_WIDTH + UICommon::PURCHASABLE_BOX_SPACING),
		m_upgradeScrollView->getContentSize().height
	));

	m_upgradeScrollView->setInnerContainerPosition({ -std::min(-m_focusTracker->uiOffsetInUpgradesTabScrollView, m_upgradeScrollView->getInnerContainerSize().width - UICommon::SCROLLVIEW_CONTENT_SIZE.width), 0 });
}

void HexiiUpgradesTab::updateBoxes() {
	auto& upgrades = m_focusTracker->getUpgradeStatePairs();

	for (uint i = 0; i < m_upgradeBoxes.size(); i++) {
		m_upgradeBoxes[i]->setUpgrade(upgrades[i].first, m_focusHexii, m_focusTracker);
	}
}

/*
void HexiiInfoTab::updateUpgradesList(BigInt levelBefore, BigInt level) {
	

	// Update the locked boxes

	// The list of preview boxes after this function has updated the list
	std::vector<UpgradeBox*> updatedPreviewUpgradeBoxes;
	for (uint i = 0; i < m_previewUpgradeBoxes.size(); i++) {
		// If a preview box has reached the reveal level, reveal it. Otherwise, it remains a preview so it stays in the updated preview \
		boxes list

		if (level >= m_previewUpgradeBoxes[i]->getUpgrade()->levelRequirement) m_previewUpgradeBoxes[i]->setState(Upgrade::State::REVEALED);
		else updatedPreviewUpgradeBoxes.push_back(m_previewUpgradeBoxes[i]);
	}

	// If there are no more locked boxes and all upgrades are available, this function is done
	if (updatedPreviewUpgradeBoxes.size() == 0 && m_upgradeScrollView->getChildrenCount() == Upgrades::UPGRADE_COUNT) {
		m_previewUpgradeBoxes = updatedPreviewUpgradeBoxes;
		return;
	}

	/// Grabs all upgrades that are currently available for purchase, plus the next two

	Upgrades::UpgradeVec list = Upgrades::getUpgradesBetweenLevels(levelBefore + 1, level);

	// The latest upgrade referenced so far
	UpgradePtr lastUpgrade = nullptr;
	if (updatedPreviewUpgradeBoxes.size() > 0) lastUpgrade = updatedPreviewUpgradeBoxes.back()->getUpgrade();
	else if (list.size() > 0) lastUpgrade = list.back();

	// The total number of locked boxes should be PREVIEW_COUNT, so this bonus list increases up to that
	Upgrades::UpgradeVec previewUpgradeList = Upgrades::getUpgradesFollowing(lastUpgrade, PREVIEW_COUNT - updatedPreviewUpgradeBoxes.size());

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
*/