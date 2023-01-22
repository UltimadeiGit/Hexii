#pragma once

#include "Memory.h"
#include "SimpleShader.h"
#include "Maths.h"
#include "BoolMap.h"

#include <functional>

class Hex : 
	public cocos2d::Node {
private:
	bool init();

public:
	// The 'role' of a Hex (determined by its universe) informs how its yield should be used
	enum class Role {
		HOME_L0,
		HOME
	} const role;

	// arg1: Layer
	CREATE_FUNC_WITH_CTOR_1(Hex, const uint);

	/// Node functions

	void update(float dt) override;
	virtual void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

	/// Element functions

	inline uint getLayer() const { return m_layer; }
	inline bool getActive() const { return m_active; }
	inline cocos2d::Texture2D* getShadedRenderTexture() const { return m_shaded->getSprite()->getTexture(); }

	void setActive(bool active);

	void onTouchBegan();
	void onTouchEnded();

	/// Gameplay values

	// Upgrade specific contributions

	BigReal getYieldFromYieldUp1Upgrade() const;
	BigReal getYieldSpeedFactorFromSpeedUp1Upgrade() const;
	
	inline BigInt getLevel() const { return m_level; }
	inline BigReal getEXP() const { return m_exp; }
	// The cost of purchasing 1 EXP for this hex
	BigReal getEXPCost() const;
	BigReal getYield() const;
	BigReal getYieldSpeed() const;
	inline bool getUpgrade(const std::string& name) const { return m_upgrades(name); }

	void addEXP(BigReal exp);
	void unlockUpgrade(const std::string& name);

	/// Info values
	
	inline BigReal getEXPRequiredForLevel(BigInt level) { return getEXPRequiredForLevel(m_layer, level); }
	static BigReal getEXPRequiredForLevel(uint layer, BigInt level);
	
	std::function<void(Hex*)> yieldFunction;

private:
	const uint m_layer;

	// An inactive hex is just a border
	bool m_active;

	cocos2d::Label* m_debugLabel = nullptr;
	// Hexagon sprite
	cocos2d::Sprite* m_hex = nullptr;
	// A shader will apply to the whole Hex after the progress outline to do a relief effect
	cocos2d::RenderTexture* m_shaded = nullptr;

	SimpleShader* m_shader = nullptr;

	// Speed = yields per second = 1 / baseYieldDelay
	const BigReal m_baseYieldSpeed;
	double m_progress = 0.0f;

	BigInt m_level = 0;
	BigReal m_exp = 0;

	bool m_isPressed = false;

	// Associates the upgrade names with a bool value depending on if they've been acquired
	BoolMap m_upgrades;		
};