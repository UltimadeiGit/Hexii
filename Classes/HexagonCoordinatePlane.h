#pragma once

#include <map>

#include "Hex.h"

// 2D Plane with a Hexagon based coordinate system (axial coordinates)
// _Ty must inherit from cocos2d::Node
template<typename _Ty>
class HexagonCoordinatePlane : public cocos2d::Layer {
public:
	_Ty* get(cocos2d::Vec2 pos) const;
	

	// Sets an object at pos by initializing a new one. Returns the new one
	_Ty* set(cocos2d::Vec2 pos);
	// Sets the object at pos to `obj`. Returns `obj`
	_Ty* set(cocos2d::Vec2 pos, _Ty* obj);

	// Converts a hex coordinate system position to a game coordinate position in local space (of this layer)
	cocos2d::Vec2 localPositionOf(const cocos2d::Vec2& pos) const;
	// Converts a game coordinate system position to a hex coordinate system position (pos should be in the local space of this node)
	cocos2d::Vec2 hexPositionOf(cocos2d::Vec2 pos) const;
	static float layerOf(const cocos2d::Vec2& pos);
	// Rounds `pos` to the nearest hex that contains it
	static cocos2d::Vec2 round(cocos2d::Vec2 pos);

	// `nodeLength` represents the vertical distance between the node at (0, 0) and (0, 1)
	inline bool initWithSize(float nodeLength) { m_nodeLength = nodeLength; return true; }

	CREATE_FUNC(HexagonCoordinatePlane<_Ty>);
private:
	bool init() { return true; }

	std::map<cocos2d::Vec2, _Ty*> m_inner;

	// In practice, vertical hexagon height 
	float m_nodeLength;
};
