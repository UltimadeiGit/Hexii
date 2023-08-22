#pragma once

#include "Memory.h"
#include "SimpleShader.h"
#include "Maths.h"
#include "BoolMap.h"
#include "CompoundLabel.h"
#include "Upgrade.h"
#include "JSON_FWD.hpp"
#include "ProgressBar.h"
#include "UpgradePath.h"
#include "BeeGroup.h"

#include "Hexagon.h"
#include "ShadedElement.h"

#include <functional>

class Hexii : public Hexagon {
public:
	// The type of a Hexii informs how its yield should be used, among other things
	enum class HexiiType {
		QueensChamber,
		WorkersCell
	};

	struct EventHexiiYieldData {
		Hexii* subject;
		HexiiType type;
		BigReal yield;
		cocos2d::Vec2 posAxial;
		uint District;
	};

	struct EventHexiiFocusData {
		Hexii* subject;
		bool active;
		cocos2d::Vec2 posAxial;
		uint District;
	};
	
	struct EventHexiiPurchaseData {
		Hexii* subject;
		bool active;
		cocos2d::Vec2 posAxial;
		uint District;
	};

private:
	bool init();

public:
	// arg1: District, arg2: axial position
	CREATE_FUNC_WITH_CTOR_2(Hexii, const uint, const cocos2d::Vec2);
	CREATE_FUNC_JSON(Hexii);
	
	void update(float dt) override;

	/// Exposed info 

	inline const cocos2d::Vec2& getAxialPosition() const { return Hexagon::getAxialPosition(); }
	inline cocos2d::Texture2D* getShadedRenderTexture() const { return m_shaded->getShadedRenderTexture(); }

	/// Gameplay

	inline uint getDistrict() const { return m_district; }
	inline bool getActive() const { return m_active; }
	inline bool getIsPressed() const { return m_isPressed; }
	void setActive(bool active);

	void addBeeGroup(UpgradePath* upgradePath, BeeGroup::BeeType beeType);

	void onBeeGained(cocos2d::EventCustom* evnt);
	void onUpgradePurchased(cocos2d::EventCustom* evnt);

	// Hexii stats

	inline HexiiType getHexiiType() const { return m_type; }
	inline BigReal getPurchaseCost() const { return getPurchaseCostFromDistrict(m_district); }

	BigReal getYield() const;
	// Returns the yield speed (per second)
	// If includeActiveBonus is true, the active yield speed multiplier is included if applicable (i.e if pressed)
	BigReal getYieldSpeed(bool includeActiveBonus = false) const;
	// Randomly determines if the yield is critical and returns the multiplier (1.0 if not critical)
	// when `times` > 1, this function returns the average critical multiplier
	BigReal getCritical(uint times) const;
	BigReal getAdjacencyYieldMultiplier() const;

	inline bool getUpgrade(const std::string& name) const { return m_upgrades(name); }
	inline const BoolMap& getUpgrades() const { return m_upgrades; }
	inline std::vector<BeeGroup>& getBeeGroups() { return m_beeGroups; }
	inline const std::vector<BeeGroup>& getBeeGroups() const { return m_beeGroups; }
	
	// Required for the json stuff
	inline const std::vector<Hexii*>& getYieldTargets() const { return m_yieldTargets; }
	
	// If suppressEvent is true, the onBeeGained will not be triggered. Used for internals
	void addEXP(BigReal exp, bool suppressEvent = false);
	// Adds `hex` as a target to receive part of this hex's yields. `angleBetween` is the counterclockwise angle from the horizontal
	// between this and `hex`, measured in degrees
	void addYieldTarget(Hexii* hex, float angleBetween);

	/// Info values
	
	BigReal getEXP() const { return m_exp; }
	// Returns the cost of purchasing 1 exp
	BigReal getEXPCost() const;

	// Returns the cost to purchase a hexii in `district`
	static BigReal getPurchaseCostFromDistrict(uint district);

	/// Events
	// Note: These functions must be dispatched by the plane since it requires hexagon collision detection

	void onTouchBegan();
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// Displays the cost label gradually
	inline void onHoverBegan() { m_isHovered = true; }
	inline void onHoverEnd() {	m_isHovered = false; }
#endif

private:
	/// Private Methods

	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* evnt);

	// Updates parts of the hex that are available while active i.e yields
	void updateActive(float dt);
	// Updates parts of the hex that are only available while inactive i.e purchasing functionality
	void updateInactive(float dt);

	// Yield some number of times (usually once)
	void yield(uint times);
	// Purchases this hex
	void purchase(BigReal cost);

	/// State

	const uint m_district;
	HexiiType m_type;

	bool m_isPressed = false;
	bool m_isHovered = false;
	time_t m_timeOfLastPress = 0;

	// An inactive hex is just a border
	bool m_active = false;

	/// Memoized values

	// Dirty if the yield needs to be recalculated
	mutable bool m_isYieldDirty = true;
	// Dirty if the yield speed needs to be recalculated
	mutable bool m_isYieldSpeedDirty = true;
	// Dirty if the critical chance and bonus need to be recalculated
	mutable bool m_isCritDirty = true;
	// Dirty if the adjacency yield multiplier needs to be recalculated
	mutable bool m_isAdjacencyDirty = true;

	mutable BigReal m_yield = 0.0f;
	mutable BigReal m_yieldSpeed = 0.0f;
	mutable BigReal m_criticalChance = 0.0f;
	mutable BigReal m_criticalYieldMultiplier = 0.0f;
	mutable BigReal m_adjacencyYieldMultiplier = 0.0f;
	mutable BigReal m_activeYieldSpeedMultiplier = 0.0f;

	/// Gameplay

	// Non-L0 hexii yield EXP to adjacent hexii of lower Districts. These are the yield targets
	std::vector<Hexii*> m_yieldTargets;
	// The groups of bees belonging to this hexii
	std::vector<BeeGroup> m_beeGroups;
	BoolMap m_upgrades;

	double m_progress = 0.0f;

	// The amount of exp this hexii has
	BigReal m_exp = 0;
	
	/// Shaded Children

	ShadedElement* m_shaded = nullptr;
	SimpleShaderPtr m_progressShader = nullptr;

	// Hexagon sprite
	// INHERITED FROM Hexagon: m_hex;

	/// Non-shaded Children

	// Multiple systems may be needed to send exp particles to the different yield targets
	std::vector<cocos2d::ParticleSystem*> m_yieldParticles;

	// Displays the cost to buy this hex if it's inactive
	CompoundLabel* m_purchaseCostLabel = nullptr;
	// Used for animation
	float m_purchaseCostLabelOpacity = 0.0f;
};

extern void to_json(nlohmann::json& j, const Hexii& hex);