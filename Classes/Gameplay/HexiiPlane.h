#pragma once

#include <map>

#include "Hexii.h"
#include "JSON_FWD.hpp"
#include "CocosUtility.h"

// 2D Plane with a Hexagon based coordinate system (axial coordinates)
class HexiiPlane : 
	public cocos2d::Node {
public:
	// Used for considering neighboring hexii
	struct HexiiPosPair {
		Hexii* hex;
		cocos2d::Vec2 pos;
	};

private:
	static constexpr int ACTION_TAG_PAN_CAMERA = 1;

	bool init();

public:
	// arg1: Hexii height
	CREATE_FUNC_WITH_CTOR_1(HexiiPlane, const float);
	CREATE_FUNC_JSON(HexiiPlane);

	inline static HexiiPlane* getInstance() { return HexiiPlane::m_instance; }

	void update(float dt) override;

	/// Camera

	// Sets the camera to pan to the specified hex
	void panCameraTo(Hexii* hex);

	/// Creation

	// Sets and returns the hex at pos by initializing a new one
	Hexii* placeHexAtPos(cocos2d::Vec2 posAxial);
	// Sets `hex` at the specified pos
	Hexii* placeHexAtPos(Hexii* hex, cocos2d::Vec2 posAxial);

	/// Retrieval

	inline const float getHexHeight() const { return m_hexHeight; }
	Hexii* getHexiiAtPos(cocos2d::Vec2 posAxial) const;
	// Returns all the hexii contained in `layer`
	std::vector<HexiiPosPair> getHexiiInlayer(uint layer) const;
	// Returns the hexii adjacent to the one at `posAxial`. Will always return 6 pairs unless `activeOnly` is true.
	// nullptr indicates a hex does not exist at that coord
	std::vector<HexiiPosPair> neighborsOf(cocos2d::Vec2 posAxial, bool activeOnly) const;	

	/// For hexii

	// Gets the adjacency bonuses owed to `target` from any adjacent hexii offering bonuses
	BigReal getAdjacencyBonuses(const Hexii* target) const;
	
private:
	/// Events
	
	void onHexiiYield(cocos2d::EventCustom* evnt);
	void onHexiiPurchase(cocos2d::EventCustom* evnt);
	void onHexiiFocus(cocos2d::EventCustom* evnt);

	void onHexiiUpgradePurchased(cocos2d::EventCustom* evnt);

	void onPinButtonPressed(cocos2d::EventCustom* evnt);

	// Sacrifice

	void onSacrificeConfirmed (cocos2d::EventCustom* evnt);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// Used for right clicks especially
	void onMousePressed(cocos2d::EventMouse* mouse);
	void onMouseUp(cocos2d::EventMouse* mouse);
	// Used for hovering
	void onMouseMoved(cocos2d::EventMouse* mouse);

	// Returns the position of the mouse relative to the camera
	
	cocos2d::Vec2 m_lastMousePos;
	Hexii* m_mouseOverHex = nullptr;
#endif // CC_PLATFORM_PC

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* evnt);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* evnt);
	cocos2d::Vec2 getTouchPos(cocos2d::Camera* camera, cocos2d::Touch* touch) const { return CocosUtility::getCameraMousePos(camera, touch->getLocation()); }

	// The vertical height of each hex. This must remain constant for the plane to have any sensible use
	const float m_hexHeight;

	// If pinned, the camera won't be panned when a hex is touched
	bool m_pinned = false;

	// The plane's internal storage
	std::map<cocos2d::Vec2, Hexii*> m_hexMap;	

	static HexiiPlane* m_instance;
};

extern void to_json(nlohmann::json& j, const HexiiPlane& plane);