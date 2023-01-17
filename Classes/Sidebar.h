#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include "HexInfoTab.h"

// Contains various tabs and can switch between them
class Sidebar :
	public cocos2d::Node {
public:
	bool init();
	virtual void update(float dt) override;

	HexInfoTab* getHexInfoTab() { return dynamic_cast<HexInfoTab*>(m_tabs[0]); }
	HexInfoTab* getSettingsTab() { return dynamic_cast<HexInfoTab*>(m_tabs[5]); }

	CREATE_FUNC(Sidebar);
private:
	void switchTab(unsigned short tab);

	cocos2d::Sprite* m_background;

	unsigned short m_currentTab = 0;
	cocos2d::Layer* m_tabs[6];
	cocos2d::ui::Button* m_tabButtons[6];
};