#include "Hex.h"

USING_NS_CC;

bool Hex::init() {
	return Sprite::initWithPolygon(getPinfo());
}

cocos2d::Rect Hex::getTextureRect() const {
	return Sprite::getTextureRect();
}

cocos2d::Rect Hex::getTextureBaseRect() {
	return getPinfo().getRect();
}

cocos2d::PolygonInfo& Hex::getPinfo() {
	static cocos2d::PolygonInfo pinfo = AutoPolygon::generatePolygon("Hexagon.png");
	return pinfo;
}