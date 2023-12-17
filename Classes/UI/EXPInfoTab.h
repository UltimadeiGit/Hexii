#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include "CompoundLabel.h"
#include "HexiiTab.h"

class EXPInfoTab :
	public HexiiTab
{
private:
	EXPInfoTab();
	EXPInfoTab(const EXPInfoTab&) = delete;
	EXPInfoTab(EXPInfoTab&&) = delete;

	bool init();
	void setFocus(Hexii* focus) {m_focusHexii = focus;}

public:
	CREATE_FUNC(EXPInfoTab);

	virtual void update(float dt) override;
	void updateLabels(BigReal exp, BigReal expForNextLevel);	

private:
	
	void onPurchaseEXPButtonPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType);
	void purchaseEXP();

private:
	// The amount of EXP the purchase button will give
	BigReal m_purchaseEXPButtonDesiredEXP = 0;
	// The amount of greenMatter the exp purchase will cost
	BigReal m_costToPurchaseDesiredEXP = 0;

	// Purchase EXP
	cocos2d::Sprite* m_purchaseEXPPlate = nullptr;
	cocos2d::Sprite* m_purchaseEXPIcon = nullptr;
	cocos2d::Label* m_purchaseEXPLabel = nullptr;
	CompoundLabel* m_purchaseEXPGainLabel = nullptr;
	cocos2d::ui::Button* m_purchaseEXPButton = nullptr;
	CompoundLabel* m_purchaseEXPButtonCostLabel = nullptr;
};

