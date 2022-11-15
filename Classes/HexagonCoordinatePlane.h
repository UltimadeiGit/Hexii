#pragma once

#include <map>

#include "Memory.h"

// 2D Plane with a _Tyagon based coordinate system
// _Ty must inherit from cocos2d::Node
template<typename _Ty>
class HexagonCoordinatePlane : public cocos2d::Layer {
private:
	typedef std::shared_ptr<_Ty> _TyPtr;
public:
	_TyPtr get(cocos2d::Vec2 pos) const;
	

	// Sets an object at pos by initializing a new one. Returns the new one
	_TyPtr set(cocos2d::Vec2 pos);
	// Sets the object at pos to `obj`. Returns `obj`
	_TyPtr set(cocos2d::Vec2 pos, _TyPtr obj);

	cocos2d::Vec2 worldPositionOf(const cocos2d::Vec2& pos) const;
	static unsigned int layerOf(const cocos2d::Vec2& pos);

	// `nodeLength` represents the vertical distance between the node at (0, 0) and (0, 1)
	inline bool initWithSize(float nodeLength) { m_nodeLength = nodeLength; return true; }

	CREATE_FUNC_SHARED(HexagonCoordinatePlane<_Ty>);
private:
	bool init() { return true; }

	std::map<cocos2d::Vec2, _TyPtr> m_inner;

	// In practice, vertical hexagon height 
	float m_nodeLength;
};
