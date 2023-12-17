#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include "CompoundLabel.h"

class SettingsTab :
	public cocos2d::Layer {
public:
	bool init();
	virtual void update(float dt) override;

	CREATE_FUNC(SettingsTab);

private:
	void onSaveButtonPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType);
	
	cocos2d::ui::Button* m_saveButton = nullptr;
	CompoundLabel* m_autosaveLabel = nullptr;
};