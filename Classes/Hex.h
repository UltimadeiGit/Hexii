#pragma once

#include "Memory.h"
#include "SimpleShader.h"
#include "BigInt.hpp"

#include <functional>

// TODO: I plan to make Hex a node instead of a sprite, then it can rendedr to a RenderTexture for the relief shader I plan to write
class Hex : protected cocos2d::Sprite {
private:
	
	bool init();

public:
	// The 'role' of a Hex (determined by its universe) informs how its yield should be used
	enum class Role {
		HOME_L0,
		HOME
	} const role;

	// TODO: Upgrades here

	CREATE_FUNC_WITH_CTOR_2(Hex, const unsigned int, const cocos2d::Vec2&);

	inline const cocos2d::Vec2& getPosition() const override { return Sprite::getPosition(); }

	inline unsigned int getLayer() const { return m_layer; }
	
	inline unsigned int getEXP() const { return m_exp; }
	void addEXP(unsigned int exp);
	inline unsigned int getLevel() const { return m_level; }
	
	inline BigInt getYield() const { return m_yield; }

	void onTouchBegan();
	void onTouchEnded();

	inline bool getActive() const { return m_active; }
	void setActive(bool active);

	inline unsigned int getEXPRequiredForLevel(unsigned int level) { return getEXPRequiredForLevel(m_layer, level); }
	static unsigned int getEXPRequiredForLevel(unsigned int layer, unsigned int level);

	void update(float dt) override;

	cocos2d::Node* asNode() { return this; }

	std::function<void(Hex*)> yieldFunction;

private:
	const unsigned int m_layer;
	const cocos2d::Vec2 m_pos;

	cocos2d::Label* m_debugLabel = nullptr;

	BigInt m_yield = 1;
	BigInt m_baseYield = 1;
	// Speed = yields per second = 1 / baseYieldDelay
	float m_baseYieldDelay;
	float m_progress = 0.0f;
	unsigned int m_exp = 0;
	unsigned int m_level = 0;

	const static unsigned int MAX_UPGRADES = 10;
	// See design docs for info about which upgrades are which. This is just an efficient way of tracking them
	unsigned int m_upgrades[MAX_UPGRADES];

	SimpleShader* m_shader = nullptr;

	bool m_isPressed = false;
	
	// An inactive hex is just a border
	bool m_active;
};