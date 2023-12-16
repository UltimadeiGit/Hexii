#pragma once

#include <cocos/ui/UIButton.h>
#include <cocos2d.h>
#include <functional>
#include "CompoundLabel.h"
#include "Upgrade.h"
#include "Hexii.h"

class UpgradeBox :
	public cocos2d::ui::Widget {
private:
	bool init();

public:
	void update(float dt) override;

	CREATE_FUNC(UpgradeBox);

	inline const UpgradePtr getUpgrade() const { return m_focus; }
	BigReal getCost();

	// Sets the upgrade information for this box and the state of purchase
	void setUpgrade(UpgradePtr upgrade, Hexii* subject, UpgradeTrackerPtr tracker);
	// State of locked, revealed or purchased
	void setState(Upgrade::State state);
	
private:
	void onPurchaseUpgradeButtonPressed(cocos2d::Ref*, Widget::TouchEventType evntType);
	// TODO: Instead, have the Upgrade tab update the box whenever a level is gained
	//void onLevelGained(cocos2d::EventCustom* evnt);
	//void onUpgradePurchased(cocos2d::EventCustom* evnt);

	void updateContributionLabelString();

	// The upgrade for which this box is displaying information
	UpgradePtr m_focus = nullptr;
	bool m_focusHasContribution = false;
	// The hexii to which the focused upgrade belongs
	Hexii* m_subject = nullptr;
	// Used to purchase the upgrade and synchronize state
	UpgradeTrackerPtr m_subjectUpgradeTracker = nullptr;

	// Keep track of whether or not the cost is affordable
	bool m_isAffordable = false;
	// Whether or not the upgrade has been bought or can be bought at this level
	Upgrade::State m_upgradeState = Upgrade::State::LOCKED;

	/// UI elements

	cocos2d::Sprite* m_upgradePlate = nullptr;
	cocos2d::Sprite* m_icon = nullptr;
	cocos2d::Label* m_lockedLabel = nullptr;
	//cocos2d::Label* m_description = nullptr;
	cocos2d::Label* m_nameLabel = nullptr;
	CompoundLabel* m_contributionLabel = nullptr;
	cocos2d::ui::Button* m_purchaseUpgradeButton = nullptr;
	CompoundLabel* m_purchaseUpgradeCostLabel = nullptr;

	//cocos2d::Sprite* m_background = nullptr;

	friend class HexiiFirstUpgradePurchasedProgressionEvent;
};