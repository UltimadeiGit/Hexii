#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include "HexiiInfoTab.h"
#include "SettingsTab.h"


// Contains various tabs and can switch between them
class Dock :
	public cocos2d::ui::Widget {
private:
	bool init();

public:
	void update(float dt) override;

	HexiiInfoTab* getHexInfoTab() { return dynamic_cast<HexiiInfoTab*>(m_tabs[0]); }
	SettingsTab* getSettingsTab() { return dynamic_cast<SettingsTab*>(m_tabs[2]); }

	CREATE_FUNC(Dock);
private:
	void onTabPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType, uint tabNumber);

	// Enable a tab when a certain trigger happens (evnt is ignored, nullptr is safe to pass)
	void enableTab(cocos2d::EventCustom* evnt, uint tabNumber);
	void switchTab(unsigned short tab);

	cocos2d::Sprite* m_background;

	unsigned short m_currentTab = 0;
	cocos2d::Layer* m_tabs[3];
	cocos2d::EventListener* m_unlockListeners[2] = { nullptr };
	cocos2d::ui::Button* m_tabButtons[3];
};