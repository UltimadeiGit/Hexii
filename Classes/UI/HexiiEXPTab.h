#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include "CompoundLabel.h"
#include "HexiiTab.h"
#include "EXPPurchaseBox.h"
#include "InvestRedMatterBox.h"

class HexiiEXPTab :
	public HexiiTab
{
private:
	HexiiEXPTab();
	HexiiEXPTab(const HexiiEXPTab&) = delete;
	HexiiEXPTab(HexiiEXPTab&&) = delete;

	bool init();
	void setFocus(Hexii* focus);

public:
	CREATE_FUNC(HexiiEXPTab);

	void update(float dt) override;
	void updateEXP(BigReal exp, BigReal expForNextLevel);
	void updateRedMatter(BigReal redMatter);

private:
	
	EXPPurchaseBox* m_expPurchaseBox = nullptr;
	InvestRedMatterBox* m_investRedMatterBox = nullptr;

	/*
	// Scale level (red matter)
	cocos2d::Sprite* m_scaleLevelPlate = nullptr;
	cocos2d::Sprite* m_scaleLevelIcon = nullptr;
	cocos2d::Label* m_scaleLevelLabel = nullptr;
	CompoundLabel* m_scaleLevelGainLabel = nullptr;
	cocos2d::ui::Button* m_scaleLevelButton = nullptr;
	CompoundLabel* m_scaleLevelButtonCostLabel = nullptr;
	*/
};

