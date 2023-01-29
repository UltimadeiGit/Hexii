#pragma once

#include "Memory.h"
#include "SimpleShader.h"
#include "Maths.h"
#include "BoolMap.h"
#include "CompoundLabel.h"
#include "Upgrades.h"
#include "JSON_FWD.hpp"
#include "ProgressBar.h"

#include <functional>

class Hex : 
	public cocos2d::Node {
public:
	// The 'role' of a Hex (determined by its universe) informs how its yield should be used
	enum class Role {
		HOME_L0,
		HOME
	};

	struct EventHexLevelUpData {
		// The hex that leveled up
		Hex* subject;
		BigInt levelBefore;
		BigInt level;
		BigReal exp;
		BigReal totalEXP;
		BigReal expRequiredForNextLevel;
	};

	struct EventHexYieldData {
		Hex* subject;
		Role role;
		BigReal yield;
		cocos2d::Vec2 posAxial;
		uint layer;
	};

	struct EventHexFocusData {
		Hex* subject;
		bool active;
		cocos2d::Vec2 posAxial;
		uint layer;
	};
	
	struct EventHexPurchaseData {
		Hex* subject;
		bool active;
		cocos2d::Vec2 posAxial;
		uint layer;
	};

	struct EventHexUpgradePurchaseData {
		Hex* subject;
		UpgradePtr upgrade;
	};
	
private:
	bool init();

public:
	// arg1: Layer, arg2: axial position
	CREATE_FUNC_WITH_CTOR_2(Hex, const uint, const cocos2d::Vec2);
	CREATE_FUNC_JSON(Hex);
	
	/*
private: 
	Hex(const nlohmann::json& data); 
	bool init(const nlohmann::json& data); 
public: 
	inline static Hex* create(const nlohmann::json& data) { 
		Hex* pRet = new Hex(data);
		if (pRet && pRet->init(data)) { 
			pRet->autorelease(); 
			return pRet; 
		} 
		else { 
			CCLOGERROR("Unknown error occurred"); 
			delete pRet; 
			pRet = nullptr; 
			return nullptr; 
		}
	}
	*/

	/// Node functions

	void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	void update(float dt) override;
	
	/// Element functions

	inline uint getLayer() const { return m_layer; }
	inline const cocos2d::Vec2& getAxialPosition() const { return m_posAxial; }
	inline bool getActive() const { return m_active; }
	inline cocos2d::Texture2D* getShadedRenderTexture() const { return m_shaded->getSprite()->getTexture(); }

	void setActive(bool active);
	// Note: applyChildren is ignored
	void setCameraMask(unsigned short mask, bool applyChildren = false) override;

	/// Gameplay values

	// Upgrade specific contributions
	
	BigReal getYieldFromYieldUp1Upgrade() const;
	BigReal getYieldFromYieldUp2Upgrade() const;
	BigReal getYieldSpeedFactorFromSpeedUp1Upgrade() const;
	BigReal getYieldSpeedFactorFromSpeedUp2Upgrade() const;
	// Returns how much of the yield is coming from the given upgrade. Returns as a multiplier, or if `asConstant` is set, returns the
	// total yield the multiplier translates to
	BigReal getContributionFromUpgrade(const std::string& upgradeName, bool asConstant) const;
	inline static BigReal multiplierToPercentageContribution(BigReal multiplierContribution) { return (multiplierContribution - 1) * 100; }

	// Hex stats

	inline BigInt getLevel() const { return m_level; }
	inline BigReal getEXP() const { return m_exp; }
	inline BigReal getTotalEXP() const { return m_totalEXP; }
	inline BigReal getEXPRequiredForNextLevel() const { return m_expRequiredForNextLevel; }
	inline BigReal getPurchaseCost() const { return getPurchaseCostFromLayer(m_layer); }
	// The cost of purchasing 1 EXP for this hex
	BigReal getEXPCost() const;
	BigReal getYield() const;
	BigReal getYieldSpeed() const;
	inline Role getRole() const { return m_role; }
	inline bool getUpgrade(const std::string& name) const { return m_upgrades(name); }
	inline const BoolMap& getUpgrades() const { return m_upgrades; }
	// Required for the json stuff
	inline const std::vector<Hex*>& getYieldTargets() const { return m_yieldTargets; }

	// If suppressEvent is true, the onHexLevelUp will not be triggered. Used for internals
	void addEXP(BigReal exp, bool suppressEvent = false);
	void levelUp(bool suppressEvent = false);
	void unlockUpgrade(UpgradePtr upgrade);
	// Adds `hex` as a target to receive part of this hex's yields. `angleBetween` is the counterclockwise angle from the horizontal
	// between this and `hex`, measured in degrees
	void addYieldTarget(Hex* hex, float angleBetween);

	/// Info values
	
	// Returns the cost to purchase a hex in `layer`
	static BigReal getPurchaseCostFromLayer(uint layer);
	// Returns the amount of exp required to reach `level` in total
	static BigReal getEXPRequiredToReachLevelFromLayer(BigReal level, uint layer);
	// Returns the level that `exp` corresponds to
	static BigReal getLevelFromEXP(BigInt exp, uint layer);

	/// Events
	// Note: These functions must be dispatched by the plane since it requires hexagon collision detection

	void onTouchBegan();
	// Displays the cost label gradually
	void onHoverBegan();
	void onHoverEnd();

private:
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* evnt);

	// Updates parts of the hex that are available while active i.e yields
	void updateActive(float dt);
	// Updates parts of the hex that are only available while inactive i.e purchasing functionality
	void updateInactive(float dt);

	// Refreshes the progress bar progress
	void updateProgressBar();

	// Returns the total yield from constant bonuses (e.g +0.5 per level)
	BigReal getConstantYield() const;
	// Returns the total yield multiplier from additive bonuses
	BigReal getAdditiveYield() const;
	// Returns the total yield multiplier from multiplicative bonuses
	BigReal getMultiplicativeYield() const;

	// Yield some number of times (usually once)
	void yield(uint times);
	// Purchases this hex
	void purchase(BigReal cost);

	const uint m_layer;
	const cocos2d::Vec2 m_posAxial;

	// An inactive hex is just a border
	bool m_active = false;

	// Displays the cost to buy this hex if it's inactive
	CompoundLabel* m_purchaseCostLabel = nullptr;
	// Used for animation
	float m_purchaseCostLabelOpacity = 0.0f;

	// Hexagon sprite
	cocos2d::Sprite* m_hex = nullptr;
	ProgressBar* m_expBar = nullptr;
	cocos2d::ParticleSystem* m_levelUpParticles = nullptr;
	// Multiple systems may be needed to send exp particles to the different yield targets
	std::vector<cocos2d::ParticleSystem*> m_yieldParticles;
	// Shows the hex after the progress shader has applied
	cocos2d::RenderTexture* m_shaded = nullptr;

	SimpleShader* m_shader = nullptr;

	// Speed = yields per second = 1 / baseYieldDelay
	const BigReal m_baseYieldSpeed;
	double m_progress = 0.0f;

	BigInt m_level = 0;
	// Amount of EXP into the current level
	BigReal m_exp = 0;
	BigReal m_totalEXP = 0;
	BigReal m_expRequiredForNextLevel = 6;
	// Non-L0 hexii yield EXP to adjacent hexii of lower layers. These are the yield targets
	std::vector<Hex*> m_yieldTargets;

	bool m_isPressed = false;
	bool m_isHovered = false;

	const Role m_role;
	// Associates the upgrade names with a bool value depending on if they've been acquired
	BoolMap m_upgrades;		
};

extern void to_json(nlohmann::json& j, const Hex& hex);