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
	Hex* getHexAtPos(cocos2d::Vec2 posAxial) const;
	// Returns all the hexii contained in `layer`
	std::vector<HexPosPair> getHexiiInLayer(uint layer);

	// Sets and returns the hex at pos by initializing a new one
	Hex* placeHexAtPos(cocos2d::Vec2 posAxial);


	// Converts a hex coordinate system position to a game coordinate position in local space (of this layer)
	cocos2d::Vec2 localPositionOf(const cocos2d::Vec2& posAxial) const;
	// Converts a game coordinate system position to a hex coordinate system position (pos should be in the local space of this node)
	cocos2d::Vec2 axialPositionOf(cocos2d::Vec2 posLocal) const;
	static float layerOf(const cocos2d::Vec2& posAxial);
	// Rounds `posAxial` to the nearest hex that contains it
	static cocos2d::Vec2 round(cocos2d::Vec2 posAxial);

	// Returns the hexii adjacent to the one at `posAxial`. Will always return 6 pairs unless `activeOnly` is true.
	// nullptr indicates a hex does not exist at that coord
	std::vector<HexPosPair> neighborsOf(cocos2d::Vec2 posAxial, bool activeOnly);
	

	void update(float dt) override;

	// arg1: Hex height
	CREATE_FUNC_WITH_CTOR_1(HexPlane, const float);
private:
	Hex* placeHexAtPos(cocos2d::Vec2 posAxial, Hex* hex);

	bool init() { return true; }

	// The plane's internal storage
	std::map<cocos2d::Vec2, Hex*> m_hexMap;

	// The vertical height of each hex. This must remain constant for the plane to have any sensible use
	const float m_hexHeight;
};
