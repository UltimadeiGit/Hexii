#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include "CompoundLabel.h"

class EXPInfoTab :
	public cocos2d::Layer
{
private:

	EXPInfoTab();
	EXPInfoTab(const EXPInfoTab&) = delete;
	EXPInfoTab(EXPInfoTab&&) = delete;

	bool init();

public:
	virtual void update(float dt) override;

	CREATE_FUNC(EXPInfoTab);

private:

	// Purchase EXP
	cocos2d::Sprite* m_purchaseEXPPlate = nullptr;
	cocos2d::Sprite* m_purchaseEXPIcon = nullptr;
	cocos2d::Label* m_purchaseEXPLabel = nullptr;
	CompoundLabel* m_purchaseEXPGainLabel = nullptr;
	cocos2d::ui::Button* m_purchaseEXPButton = nullptr;
	CompoundLabel* m_purchaseEXPButtonCostLabel = nullptr;
};

