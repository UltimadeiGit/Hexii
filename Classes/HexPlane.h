#pragma once

#include <map>

#include "Hex.h"

// 2D Plane with a Hexagon based coordinate system (axial coordinates)
// _Ty must inherit from cocos2d::Node
class HexPlane : 
	public cocos2d::Node {
public:
	// Used for considering neighboring hexii
	struct HexPosPair {
		Hex* hex;
		cocos2d::Vec2 pos;
	};

private:
	static constexpr int ACTION_TAG_PAN_CAMERA = 1;

	bool init();

public:
	// arg1: Hex height
	CREATE_FUNC_WITH_CTOR_1(HexPlane, const float);

	void update(float dt) override;

	/// Creation

	// Sets and returns the hex at pos by initializing a new one
	Hex* placeHexAtPos(cocos2d::Vec2 posAxial);

	/// Retrieval

	Hex* getHexAtPos(cocos2d::Vec2 posAxial) const;
	// Returns all the hexii contained in `layer`
	std::vector<HexPosPair> getHexiiInLayer(uint layer);
	// Returns the hexii adjacent to the one at `posAxial`. Will always return 6 pairs unless `activeOnly` is true.
	// nullptr indicates a hex does not exist at that coord
	std::vector<HexPosPair> neighborsOf(cocos2d::Vec2 posAxial, bool activeOnly);
	
	/// Position conversion

	// Converts a hex coordinate system position to a game coordinate position in local space (of this layer)
	cocos2d::Vec2 localPositionOf(const cocos2d::Vec2& posAxial) const;
	// Converts a game coordinate system position to a hex coordinate system position (pos should be in the local space of this node)
	cocos2d::Vec2 axialPositionOf(cocos2d::Vec2 posLocal) const;
	
	/// Maths
	
	static float layerOf(const cocos2d::Vec2& posAxial);
	// Rounds `posAxial` to the nearest hex that contains it
	static cocos2d::Vec2 round(cocos2d::Vec2 posAxial);	
	
private:
	/// Events

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* evnt);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* evnt);
	void onHexYield(cocos2d::EventCustom* evnt);
	void onHexPurchase(cocos2d::EventCustom* evnt);
	void onHexFocus(cocos2d::EventCustom* evnt);

	void onPinButtonPressed(cocos2d::EventCustom* evnt);

#ifdef CC_PLATFORM_PC
	// Used for right clicks especially
	void onMousePressed(cocos2d::EventMouse* mouse);
	void onMouseUp(cocos2d::EventMouse* mouse);
	// Used for hovering
	void onMouseMoved(cocos2d::EventMouse* mouse);

	Hex* m_mouseOverHex = nullptr;
#endif // CC_PLATFORM_PC

	// Utility

	cocos2d::Vec2 getTouchPos(cocos2d::Touch* touch) const { return touch->getLocation() + m_camera->getPosition(); }
	cocos2d::Vec2 getMousePos(cocos2d::EventMouse* mouse) const { return mouse->getLocationInView() + m_camera->getPosition(); }

	// The vertical height of each hex. This must remain constant for the plane to have any sensible use
	const float m_hexHeight;

	// The camera for the hexii
	cocos2d::Camera* m_camera = nullptr;
	// If pinned, the camera won't be panned when a hex is touched
	bool m_pinned = false;

	// The plane's internal storage
	std::map<cocos2d::Vec2, Hex*> m_hexMap;	
};
