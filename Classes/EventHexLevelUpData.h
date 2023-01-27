#pragma once

#include <cocos2d.h>

class EventHexLevelUpData : 
	public cocos2d::EventCustom {
public:
	inline EventHexLevelUp() : cocos2d::Event(cocos2d::Event::Type::CUSTOM) {}

private:
};