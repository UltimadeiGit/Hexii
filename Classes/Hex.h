#pragma once

#include "Memory.h"
#include "SimpleShader.h"
#include "BigInt.hpp"

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

	// TODO: Upgrades here

	// arg1: Layer
	CREATE_FUNC_WITH_CTOR_1(Hex, const unsigned int);

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

	cocos2d::Texture2D* getShadedRenderTexture() const { return m_shaded->getSprite()->getTexture(); }

	virtual void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

	std::function<void(Hex*)> yieldFunction;

private:
	const unsigned int m_layer;

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

	// Hexagon sprite
	cocos2d::Sprite* m_hex;

	// A shader will apply to the whole Hex after the progress outline to do a relief effect
	cocos2d::RenderTexture* m_shaded;
};