#pragma once


#include <cocos2d.h>
#include <cocos/ui/UIButton.h>

#include "EventUtility.h"
#include "CompoundLabel.h"
#include "GameplayCommon.h"

class HexiiSacrificeHUD :
	public cocos2d::Node 
{
public:
	bool init() override;

	EventUtility::ID eventID = EventUtility::generateID();

	CREATE_FUNC(HexiiSacrificeHUD);

	// Updates the sacrifice and red matter gain values
	void update(float dt) override;

	void updateValues();

private:
	void onCancelButtonPressed(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType);
	void onConfirmButtonPressed(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType);

	CompoundLabel* getGreenMatterSacrificeLabel() { return m_sacrifices[0]; }
	CompoundLabel* getLayerSacrificeLabel(int layer) { return m_sacrifices[layer + 1]; }
	CompoundLabel* getRespecSacrificeLabel() { return m_sacrifices[STAT_LABEL_COUNT - 1]; }

	CompoundLabel* getGreenMatterRedMatterGainLabel() { return m_redMatterGains[0]; }
	CompoundLabel* getLayerRedMatterGainLabel(int layer) { return m_redMatterGains[layer + 1]; }
	CompoundLabel* getRespecRedMatterGainLabel() { return m_redMatterGains[STAT_LABEL_COUNT - 1]; }

	bool m_hidden = false;

	// Sprites
	
	cocos2d::Sprite* m_header;
	cocos2d::Sprite* m_backgroundPlate;

	// Labels

	constexpr static int STAT_LABEL_COUNT = 2 + GameplayCommon::MAX_LAYER;

	// Sacrifices from green matter, layers 0~2 and respec
	CompoundLabel* m_sacrifices[STAT_LABEL_COUNT];
	// Red matter gains from green matter, layers 0~2 and respec
	CompoundLabel* m_redMatterGains[STAT_LABEL_COUNT];

	// Buttons

	cocos2d::ui::Button* m_cancelButton;
	cocos2d::ui::Button* m_confirmButton;
	CompoundLabel* m_redMatterGainLabel;

	// Values

	BigReal m_redMatterGain = 0;
};