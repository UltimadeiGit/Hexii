#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include "HexiiInfoTab.h"
#include "SettingsTab.h"
#include "TabbedWidget.h"

// Contains various tabs and can switch between them
class Dock :
	public TabbedWidget<3> {
private:
	bool init();

public:
	void update(float dt) override;

	HexiiInfoTab* getHexInfoTab() { return dynamic_cast<HexiiInfoTab*>(m_tabs[0]); }
	SettingsTab* getSettingsTab() { return dynamic_cast<SettingsTab*>(m_tabs[2]); }

	CREATE_FUNC(Dock);
	
protected:

	void onProgressionHexiiPurchased(cocos2d::EventCustom*);

	void enableTab(uint tabNumber, bool autoSwitch = true) override;

private:
	cocos2d::Sprite* m_background;
	cocos2d::EventListener* m_unlockListeners[2] = { nullptr };
};