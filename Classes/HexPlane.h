#pragma once

#include <map>

#include "Hex.h"

// 2D Plane with a Hexagon based coordinate system (axial coordinates)
// _Ty must inherit from cocos2d::Node
class HexPlane : public cocos2d::Layer {
public:
	// Used for considering neighboring hexii
	struct HexPosPair {
		Hex* hex;
		cocos2d::Vec2 pos;
	};

public:
	Hex* get(cocos2d::Vec2 pos) const;
	
	// Sets and returns the hex at pos by initializing a new one
	Hex* set(cocos2d::Vec2 pos);

	// Converts a hex coordinate system position to a game coordinate position in local space (of this layer)
	cocos2d::Vec2 localPositionOf(const cocos2d::Vec2& pos) const;
	// Converts a game coordinate system position to a hex coordinate system position (pos should be in the local space of this node)
	cocos2d::Vec2 hexPositionOf(cocos2d::Vec2 pos) const;
	static float layerOf(const cocos2d::Vec2& pos);
	// Rounds `pos` to the nearest hex that contains it
	static cocos2d::Vec2 round(cocos2d::Vec2 pos);

	// Returns the hexii adjacent to the one at `pos`. Will always return 6 pairs unless `activeOnly` is true.
	// nullptr indicates a hex does not exist at that coord
	std::vector<HexPosPair> neighborsOf(cocos2d::Vec2 pos, bool activeOnly);

	// `nodeLength` represents the vertical distance between the node at (0, 0) and (0, 1)
	inline bool initWithSize(float nodeLength) { m_nodeLength = nodeLength; return true; }

	void update(float dt) override;

	CREATE_FUNC(HexPlane);
private:
	Hex* set(cocos2d::Vec2 pos, Hex* hex);

	bool init() { return true; }

	std::map<cocos2d::Vec2, Hex*> m_inner;

	// In practice, vertical hexagon height 
	float m_nodeLength;
};
