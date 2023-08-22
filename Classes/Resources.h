#pragma once

#include "Maths.h"
#include "JSON_FWD.hpp"
#include <vector>

class Resources {
public:
	static constexpr uint MAX_DISTRICTS = 3;

private:
	// Singleton pattern

	Resources();
	Resources(const Resources&) = delete;
	Resources(Resources&&) = delete;

public:
	static Resources* getInstance();

	bool load(const nlohmann::json& data);

	/// nectar

	inline BigReal getNectar() const { return m_nectar; }
	// Adds op to nectar count. Throws if op is unaffordable
	void addNectar(BigReal op);

	/// Hexii counts

	inline uint getHexiiCountInDistrict(uint District) const { return District >= Resources::MAX_DISTRICTS ? 0 : m_hexiiCountPerDistrict[District]; }
	// Adds to the count of the hexii in `District`
	void addHexiiInDistrict(uint District);

	/// Global power

	inline BigReal getUnity2UpgradeBonus() const { return m_Unity2UpgradeBonus; }
	void addUnity2UpgradeBonus() { m_Unity2UpgradeBonus += 1; }

	/// Dock tabs

	inline bool getTabEnabled(uint tabNumber) const { CC_ASSERT(tabNumber < 5); return m_tabsEnabled[tabNumber]; }
	inline void setTabEnabled(uint tabNumber, bool enabled) { CC_ASSERT(tabNumber < 5); m_tabsEnabled[tabNumber] = enabled; }

private:
	static Resources* m_instance;

	BigReal m_nectar = 6;
	uint m_hexiiCountPerDistrict[MAX_DISTRICTS] = { 0 };
	BigReal m_Unity2UpgradeBonus = 0;
	bool m_tabsEnabled[5] = { false };
};

extern void to_json(nlohmann::json& j, const Resources& resources);