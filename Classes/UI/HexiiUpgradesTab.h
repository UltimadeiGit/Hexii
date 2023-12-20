#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIScrollView.h>

#include "HexiiTab.h"
#include "UpgradeBox.h"

class HexiiUpgradesTab :
	public HexiiTab 
{
private:
	HexiiUpgradesTab();
	HexiiUpgradesTab(const HexiiUpgradesTab&) = delete;
	HexiiUpgradesTab(HexiiUpgradesTab&&) = delete;

	bool init();

public:
	CREATE_FUNC_WITH_CTOR_1(HexiiUpgradesTab, UpgradePathPtr);

	virtual void update(float dt) override;

	void setFocus(Hexii* focus) override;

	// Update the state of the scroll view and upgrade boxes
	void updateStates();
private:
	
	// Update the foci of the upgrade boxes
	void updateBoxes();
	UpgradeBox* addUpgradeToList(UpgradePtr upgrade);

	inline void updateTrackerStoredScrollViewPos() { if(m_focusTracker) m_focusTracker->uiOffsetInUpgradesTabScrollView = m_upgradeScrollView->getInnerContainerPosition().x; }

private:
	UpgradePathPtr m_focusPath;
	// Tracker for the upgrades listed in this tab
	UpgradeTrackerPtr m_focusTracker = nullptr;

	cocos2d::ui::ScrollView* m_upgradeScrollView = nullptr;
	std::vector<UpgradeBox*> m_upgradeBoxes;

	friend class HexiiFirstUpgradePurchasedProgressionEvent;
};