#pragma once

#include <cocos2d.h>
#include "Maths.h"

// Flat top hexagon 
class Hexagon : public cocos2d::Node {
public:
	/// Static conversion methods

	// Converts axial coordinates to pixel coordinates
	static cocos2d::Vec2 axialToPixel(const cocos2d::Vec2& posAxial, const float hexHeight);
	// Converts pixel coordinates to axial coordinates
	static cocos2d::Vec2 pixelToAxial(const cocos2d::Vec2& posPixel, const float hexHeight);

	static float layerOf(const cocos2d::Vec2& posAxial);
	// Rounds `posAxial` to the nearest hex that contains it
	static cocos2d::Vec2 axialRound(cocos2d::Vec2 posAxial);

protected:

public:
	Hexagon(const cocos2d::Vec2& posAxial);

	inline const cocos2d::Vec2& getAxialPosition() const { return m_posAxial; }
	inline void setAxialPosition(const cocos2d::Vec2& posAxial) { m_posAxial = posAxial; }

protected:
	cocos2d::Sprite* m_hex = nullptr;

private:
	cocos2d::Vec2 m_posAxial;
};