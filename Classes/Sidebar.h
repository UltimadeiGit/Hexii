#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include "HexInfoTab.h"
#include "SettingsTab.h"


// Contains various tabs and can switch between them
class Sidebar :
	public cocos2d::ui::Widget {
private:
	bool init();

public:
	void update(float dt) override;

	HexInfoTab* getHexInfoTab() { return dynamic_cast<HexInfoTab*>(m_tabs[0]); }
	SettingsTab* getSettingsTab() { return dynamic_cast<SettingsTab*>(m_tabs[5]); }

	CREATE_FUNC(Sidebar);
private:
	void switchTab(unsigned short tab);

	cocos2d::Sprite* m_background;

	unsigned short m_currentTab = 0;
	cocos2d::Layer* m_tabs[6];
	cocos2d::ui::Button* m_tabButtons[6];
};