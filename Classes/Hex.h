#pragma once

#include "Memory.h"

class Hex : public cocos2d::Sprite {
private:
	bool init();

public:

	CREATE_FUNC(Hex);

	cocos2d::Rect getTextureRect() const;
	static cocos2d::Rect getTextureBaseRect();

	void setColor(cocos2d::Color3B col);

	inline bool getActive() const { return m_active; }
	void setActive(bool active);

private:
	static cocos2d::PolygonInfo& getActivePinfo();
	static cocos2d::PolygonInfo& getInactivePinfo();
	cocos2d::PolygonInfo& getPinfo();
	
	// An inactive hex is just a border
	bool m_active;
};