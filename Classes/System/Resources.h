#pragma once

#include "Maths.h"
#include "JSON_FWD.hpp"
#include "Console.h"
#include "GameplayCommon.h"
#include <vector>

class Resources {
private:
	// Singleton pattern

	Resources();
	Resources(const Resources&) = delete;
	Resources(Resources&&) = delete;

public:
	static Resources* getInstance();

	bool load(const nlohmann::json& data);

	/// greenMatter

	inline BigReal getGreenMatter() const { return m_greenMatter; }
	inline BigReal getRedMatter() const { return m_redMatter; }
	
	// Adds `amount` to greenMatter count. Throws if unaffordable
	inline void addGreenMatter(BigReal amount) { addResource(m_greenMatter, amount); }
	// Adds `amount` to redMatter count. Throws if unaffordable
	inline void addRedMatter(BigReal amount) { addResource(m_redMatter, amount); }

	/// Hexii counts

	inline uint getHexiiCountInlayer(uint layer) const { return layer >= GameplayCommon::MAX_LAYER ? 0 : m_hexiiCountPerlayer[layer]; }
	// Adds to the count of the hexii in `layer`
	void addHexiiInlayer(uint layer, int num = 1);

	/// Global power

	inline BigReal getGlobalPowerUpgradeBonus() const { return m_globalPowerUpgradeBonus; }
	void addGlobalPowerUpgradeBonus(int num = 1) { m_globalPowerUpgradeBonus += num; }

	// TODO: Find a better way of doing this
	/// Dock tabs

	inline bool getTabEnabled(uint tabNumber) const { CC_ASSERT(tabNumber < 5); return m_tabsEnabled[tabNumber]; }
	inline void setTabEnabled(uint tabNumber, bool enabled) { CC_ASSERT(tabNumber < 5); m_tabsEnabled[tabNumber] = enabled; }

private:
	template <typename _Ty>
	inline void addResource(_Ty& resource, _Ty amount) {
		if (amount > 0 || resource >= std::abs(amount)) resource += amount;
		else err("Resource cost was unaffordable");
	}

	static Resources* m_instance;

	BigReal m_greenMatter = 6;
	BigReal m_redMatter = 0;
	uint m_hexiiCountPerlayer[GameplayCommon::MAX_LAYER] = { 0 };
	BigReal m_globalPowerUpgradeBonus = 0;
	bool m_tabsEnabled[5] = { false };
};

extern void to_json(nlohmann::json& j, const Resources& resources);