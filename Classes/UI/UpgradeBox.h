#pragma once

#include "PurchasableBox.h"
#include "Upgrade.h"
#include "Hexii.h"
#include "Tooltip.h"

class UpgradeBox : public PurchasableBox {
private:
	inline UpgradeBox() : PurchasableBox(true) {}

	bool init() override;

public:
	void update(float dt) override;

	CREATE_FUNC(UpgradeBox);

	inline const UpgradePtr getUpgrade() const { return m_focus; }

	inline BigReal getCost() override { return m_focus->greenMatterCost * Hexii::getUpgradePurchaseCostMultiplier(m_subject->getLayer()); }

	// Sets the upgrade information for this box and the state of purchase
	void setUpgrade(UpgradePtr upgrade, Hexii* subject, UpgradeTrackerPtr tracker);
	// State of locked, revealed or purchased
	void setState(Upgrade::State state);
	
private:
	void tryPurchase() override;

	void updateIsAffordable() override;
	// Updates the contribution label
	void updateLabels() override;

	// The upgrade for which this box is displaying information
	UpgradePtr m_focus = nullptr;
	bool m_focusHasContribution = false;
	// The hexii to which the focused upgrade belongs
	Hexii* m_subject = nullptr;
	// Used to purchase the upgrade and synchronize state
	UpgradeTrackerPtr m_subjectUpgradeTracker = nullptr;

	cocos2d::ui::RichText* m_description = nullptr;
	TooltipWidget* m_descriptionWidget = nullptr;

	// Keep track of whether or not the cost is affordable
	bool m_isAffordable = false;

	// Whether or not the upgrade has been bought or can be bought at this level
	Upgrade::State m_upgradeState = Upgrade::State::LOCKED;

	friend class HexiiFirstUpgradePurchasedProgressionEvent;
};