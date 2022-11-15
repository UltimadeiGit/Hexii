#pragma once

#include "Memory.h"

class Hex : public cocos2d::Sprite {
private:
	bool init();

public:

	CREATE_FUNC_SHARED(Hex);

	cocos2d::Rect getTextureRect() const;
	static cocos2d::Rect getTextureBaseRect();
private:
	static cocos2d::PolygonInfo& getPinfo();
	
};

typedef std::shared_ptr<Hex> HexPtr;