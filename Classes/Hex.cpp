#include "Hex.h"

USING_NS_CC;

bool Hex::init() {
	return Sprite::initWithPolygon(getPinfo());
}

cocos2d::Rect Hex::getTextureRect() const {
	return Sprite::getTextureRect();
}

cocos2d::Rect Hex::getTextureBaseRect() {
	// Active and inactive both have the same rect so either works
	return getActivePinfo().getRect();
}

void Hex::setColor(cocos2d::Color3B col) {
	Sprite::setColor(col);
}
void Hex::setActive(bool active) {
	if (m_active == active) return;

	m_active = active;
	Sprite::initWithPolygon(getPinfo());
}

cocos2d::PolygonInfo& Hex::getActivePinfo() {
	/// TODO: Replace static variables with resource stuff
	static cocos2d::PolygonInfo pinfo = AutoPolygon::generatePolygon("Hexagon.png");
	return pinfo;
}

cocos2d::PolygonInfo& Hex::getInactivePinfo() {
	/// TODO: Replace static variables with resource stuff
	static cocos2d::PolygonInfo pinfo = AutoPolygon::generatePolygon("HexagonInactive.png");
	return pinfo;
}

cocos2d::PolygonInfo& Hex::getPinfo() {
	return m_active ? getActivePinfo() : getInactivePinfo();
}
