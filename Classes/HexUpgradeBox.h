#pragma once

#include <cocos/ui/UIButton.h>
#include <cocos2d.h>
#include <functional>
#include "CompoundLabel.h"
#include "Upgrades.h"

class HexUpgradeBox :
	public cocos2d::ui::Widget {
private:
	bool init();

public:
	enum class State {
		LOCKED,
		REVEALED,
		PURCHASED
	};

	void update(float dt) override;

	CREATE_FUNC(HexUpgradeBox);

	inline const UpgradePtr getUpgrade() const { return m_focus; }

	// Sets the upgrade information for this box and the state of purchase
	void setUpgrade(UpgradePtr upgrade, State state = State::LOCKED);
	// State of locked, revealed or purchased
	void setState(State state);

	void onPurchaseUpgradeButtonPressed(Ref*);

	std::function<void(UpgradePtr)> purchaseUpgradeFunction;
private:
	// The upgrade for which this box is displaying information
	UpgradePtr m_focus;

	// Keep track of whether or not the cost is affordable
	bool m_isAffordable = false;
	// Whether or not the upgrade has been bought or can be bought at this level
	State m_upgradeState = State::LOCKED;

	cocos2d::Sprite* m_background = nullptr;
	cocos2d::Label* m_lockedLabel = nullptr;
	cocos2d::Sprite* m_icon = nullptr;
	cocos2d::Label* m_description = nullptr;
	CompoundLabel* m_costLabel = nullptr;
	cocos2d::ui::Button* m_purchaseUpgradeButton = nullptr;	
};