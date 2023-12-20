#pragma once

#include "PurchasableBox.h"
#include "Hexii.h"

class EXPPurchaseBox :
	public PurchasableBox
{
private:
	EXPPurchaseBox() : PurchasableBox(false, "+") {}

	bool init() override;

public:
	CREATE_FUNC(EXPPurchaseBox);

	virtual BigReal getCost() override { return m_cost; }

	void setFocusHexii(Hexii* focus);

	void update(float dt) override;
	void updateValues(BigReal exp, BigReal expForNextLevel);
	void updateLabels();

private:
	void onPurchaseButtonPressed(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType) override;

	void updateIsAffordable() override;
	void purchaseEXP();

	Hexii* m_focusHexii = nullptr;

	// The amount of EXP the purchase button will give
	BigReal m_desiredEXP = 0;

	// The amount of greenMatter the exp purchase will cost
	BigReal m_cost = 0;
};