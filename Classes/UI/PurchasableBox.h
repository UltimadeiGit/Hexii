#pragma once

#include <cocos/ui/UIButton.h>
#include <cocos2d.h>
#include <functional>
#include "CompoundLabel.h"
#include "Maths.h"

class PurchasableBox :
	public cocos2d::ui::Widget {
protected:
	// If `hidePurchaseButtonWhenUnaffordable` is true, the purchase button will be hidden when the purchase is unaffordable
	// Otherwise, the purchase button will be disabled and bright set to false when the purchase is unaffordable
	PurchasableBox(bool hidePurchaseButtonWhenUnaffordable, const std::string& footerConstPartStr = "");

	virtual bool init() override;

public:
	bool isAffordable() { return m_isAffordable; }
	// Define a cost for the purchase
	virtual BigReal getCost() = 0;


protected:
	// Determines whether or not the purchase button is enabled
	void togglePurchaseable(bool purchasable);

	virtual void updateLabels() = 0;
	virtual void updateIsAffordable() = 0;

	virtual void onPurchaseButtonPressed(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType) = 0;

	bool m_isPurchasable = true;
	bool m_isAffordable = false;

	bool m_hidePurchaseButtonWhenUnaffordable = false;
	const std::string m_footerConstPartStr;

	/// UI elements

	cocos2d::Sprite* m_backgroundPlate = nullptr;
	cocos2d::Sprite* m_icon = nullptr;
	cocos2d::Label* m_lockedLabel = nullptr;
	//cocos2d::Label* m_description = nullptr;
	cocos2d::Label* m_nameLabel = nullptr;
	CompoundLabel* m_footerLabel = nullptr;
	cocos2d::ui::Button* m_purchaseButton = nullptr;
	CompoundLabel* m_purchaseCostLabel = nullptr;
};