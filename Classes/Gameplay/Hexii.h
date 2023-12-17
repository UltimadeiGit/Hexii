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
#include "EventUtility.h"
#include "UpgradeTracker.h"

#include "Hexagon.h"
#include "ShadedElement.h"
#include "ShaderSubject.h"

#include <functional>

class Hexii : public Hexagon, public ShaderSubject {
public:
	// The type of a Hexii informs how its yield should be used, among other things
	enum class HexiiType {
		HOME_L0,
		HOME
	};

	struct EventHexiiYieldData {
		Hexii* subject;
		HexiiType type;
		BigReal yield;
		cocos2d::Vec2 posAxial;
		uint layer;
	};

	struct EventHexiiFocusData {
		Hexii* subject;
		bool active;
		cocos2d::Vec2 posAxial;
		uint layer;
	};
	
	struct EventHexiiPurchaseData {
		Hexii* subject;
		bool active;
		cocos2d::Vec2 posAxial;
		uint layer;
	};

	struct EventHexiiLevelGainedData {
		// The spec that gained a level
		Hexii* subject;
		BigInt levelChange;
		// The new states of the changed upgrades
		//std::vector<std::pair<UpgradePtr, Upgrade::State>> changedStates;
	};

	struct EventHexiiUpgradePurchasedData {
		Hexii* subject;
		UpgradeTrackerPtr tracker;
		UpgradePtr upgrade;
	};

	constexpr static const char* EVENT_PURCHASE = "onHexiiPurchase";
	constexpr static const char* EVENT_FOCUS = "onHexiiFocus";
	constexpr static const char* EVENT_LEVEL_GAINED = "onHexiiLevelGained";
	constexpr static const char* EVENT_UPGRADE_PURCHASED = "onHexiiUpgradePurchased";

private:
	bool init();

public:
	// arg1: layer, arg2: axial position
	CREATE_FUNC_WITH_CTOR_2(Hexii, const uint, const cocos2d::Vec2);
	CREATE_FUNC_JSON(Hexii);
	
	void update(float dt) override;

	/// Exposed info 

	inline const cocos2d::Vec2& getAxialPosition() const { return Hexagon::getAxialPosition(); }
	inline cocos2d::Texture2D* getShadedRenderTexture() const { return m_shaded->getShadedRenderTexture(); }
	inline UpgradeTrackerPtr getStandardUpgradeTracker() const { return m_standardPathTracker; }
	UpgradeTrackerPtr getUpgradeTrackerFromPath(UpgradePathPtr path) const;

	/// Gameplay

	inline uint getLayer() const { return m_layer; }
	inline bool getActive() const { return m_active; }
	inline bool getIsPressed() const { return m_isPressed; }
	void setActive(bool active);

	void purchaseUpgrade(UpgradeTrackerPtr tracker, UpgradePtr upgrade);

	// Hexii stats

	inline HexiiType getHexiiType() const { return m_type; }
	inline BigReal getPurchaseCost() const { return getPurchaseCostFromlayer(m_layer); }

	BigReal getYield() const;
	// Returns the yield speed (per second)
	// If includeActiveBonus is true, the active yield speed multiplier is included if applicable (i.e if pressed)
	BigReal getYieldSpeed(bool includeActiveBonus = false) const;
	// Randomly determines if the yield is critical and returns the multiplier (1.0 if not critical)
	// when `times` > 1, this function returns the average critical multiplier
	BigReal getCritical(uint times) const;
	BigReal getAdjacencyYieldMultiplier() const;

	//inline bool getUpgrade(const std::string& name) const { return m_upgrades(name); }
	//inline const BoolMap& getUpgrades() const { return m_upgrades; }
	//inline std::vector<Specialization>& getSpecializations() { return m_specGroups; }
	//inline const std::vector<Specialization>& getSpecializations() const { return m_specGroups; }
	
	// Required for the json stuff
	inline const std::vector<Hexii*>& getYieldTargets() const { return m_yieldTargets; }
	
	inline BigReal getEXPForNextLevel() const { return m_expForNextLevel; }

	static BigReal getEXPRequiredForLevel(BigInt level, uint layer);
	static BigInt getLevelFromEXP(BigReal exp, uint layer);
	static BigReal getUpgradePurchaseCostMultiplier(uint layer);

	// If suppressEvent is true, the onLevelGained will not be triggered. Used for internals
	void addEXP(BigReal exp, bool suppressEvent = false);
	// Called whenever exp is gained, to check if the level count should be increased.
	// returns the change in level count
	// suppressEvent: if true, no events will be dispatched (used for loading)
	BigInt updateLevel(BigReal exp, bool suppressEvent = false);

	// Adds `hex` as a target to receive part of this hex's yields. `angleBetween` is the counterclockwise angle from the horizontal
	// between this and `hex`, measured in degrees
	void addYieldTarget(Hexii* hex, float angleBetween);

	/// Info values
	
	BigReal getEXP() const { return m_exp; }
	BigInt getLevel() const { return m_level; }
	// Returns the cost of purchasing 1 exp
	BigReal getEXPCost() const;

	// Returns the cost to purchase a hexii in `layer`
	static BigReal getPurchaseCostFromlayer(uint layer);

	/// Events
	// Note: These functions must be dispatched by the plane since it requires hexagon collision detection

	void onShaderEffectChanged(SimpleShaderPtr shader) override;
	void focus();

	void onTouchBegan();
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// Displays the cost label gradually
	inline void onHoverBegan() { m_isHovered = true; }
	inline void onHoverEnd() {	m_isHovered = false; }
#endif

	const EventUtility::ID eventID = EventUtility::generateID();

	/// Upgrade contributions

	BigReal getYieldFromYieldUp1Upgrade() const;
	BigReal getYieldFromYieldUp2Upgrade() const;
	BigReal getYieldSpeedMultiplierFromSpeedUp1Upgrade() const;
	BigReal getYieldSpeedMultiplierFromSpeedUp2Upgrade() const;
	BigReal getCriticalChanceFromCriticalChance1Upgrade() const;
	BigReal getCriticalYieldMultiplierFromCriticalBonus1Upgrade() const;
	BigReal getActiveYieldSpeedMultiplierFromStrongArmUpgrade() const;
	BigReal getAdjacencyYieldMultiplierFromSupport1Upgrade() const;

	// Marks the yield as dirty, so that it will be recalculated on the next update
	// This may be caused by adjacency bonus changes, global power, etc
	void markYieldDirty() { m_isYieldDirty = true; }

	// Returns the total yield from constant bonuses (e.g +0.5 per level)
	BigReal getConstantYield() const;
	// Returns the total yield multiplier from additive bonuses
	BigReal getAdditiveYieldMultiplier() const;
	// Returns the total yield multiplier from multiplicative bonuses
	BigReal getMultiplicativeYieldMultiplier() const;

	// Returns the extra yields per second from constant sources
	BigReal getConstantYieldSpeed() const;
	// Returns the multiplier to yields per second from additive sources
	BigReal getAdditiveYieldSpeedMultiplier() const;
	// Returns the multiplier to yields per second from multiplicative sources
	BigReal getMultiplicativeYieldSpeedMultiplier() const;

	// Returns an additive increase to the chance of a crit (represented as a value between 0 and 1)
	BigReal getCriticalChance() const;
	// Returns a multiplicative increase to the yield of a crit
	BigReal getCriticalYieldMultiplier() const;
	// Returns a multiplicative increase to yield from the active bonus
	BigReal getActiveYieldSpeedMultiplier() const;

	// Gives a percentage, or if `evaluated` is set, the total value the upgrade evaluates to
	BigReal getContributionFromUpgrade(const std::string& upgradeName, bool evaluated) const;

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

	const uint m_layer;
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

	mutable BigReal m_expForNextLevel = 6;

	mutable BigReal m_yield = 0.0f;
	mutable BigReal m_yieldSpeed = 0.0f;
	mutable BigReal m_criticalChance = 0.0f;
	mutable BigReal m_criticalYieldMultiplier = 0.0f;
	mutable BigReal m_adjacencyYieldMultiplier = 0.0f;
	mutable BigReal m_activeYieldSpeedMultiplier = 0.0f;

	/// Gameplay

	// Non-L0 hexii yield EXP to adjacent hexii of lower layers. These are the yield targets
	std::vector<Hexii*> m_yieldTargets;
	
	// The groups of specializations belonging to this hexii
	UpgradeTrackerPtr m_standardPathTracker = nullptr;
	//std::vector<Specialization> m_specGroups;
	//BoolMap m_upgrades;

	double m_progress = 0.0f;

	// The amount of exp this hexii has
	BigReal m_exp = 0;
	BigInt m_level = 0;
	
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