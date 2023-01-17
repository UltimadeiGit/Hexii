#pragma once

#include <cocos2d.h>

class SettingsTab :
	public cocos2d::Layer {
public:
	bool init();
	virtual void update(float dt) override;

	CREATE_FUNC(SettingsTab);
private:

};