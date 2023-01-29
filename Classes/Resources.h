#pragma once

#include "Maths.h"
#include "JSON_FWD.hpp"
#include <vector>

class Resources {
public:
	static constexpr uint MAX_LAYERS = 7;

private:
	// Singleton pattern

	Resources();
	Resources(const Resources&) = delete;
	Resources(Resources&&) = delete;

public:
	static Resources* getInstance();

	bool load(const nlohmann::json& data);

	/// Green matter

	inline BigReal getGreenMatter() const { return m_greenMatter; }
	// Adds op to green matter count. Throws if op is unaffordable
	void addGreenMatter(BigReal op);

	/// Hexii counts

	inline uint getHexiiCountInLayer(uint layer) const { return layer >= Resources::MAX_LAYERS ? 0 : m_hexiiCountPerLayer[layer]; }
	// Adds to the count of the hexii in `layer`
	void addHexInLayer(uint layer);

	/// Sidebar tabs

	inline bool getTabEnabled(uint tabNumber) const { CC_ASSERT(tabNumber < 5); return m_tabsEnabled[tabNumber]; }
	inline void setTabEnabled(uint tabNumber, bool enabled) { CC_ASSERT(tabNumber < 5); m_tabsEnabled[tabNumber] = enabled; }

private:
	static Resources* m_instance;

	BigReal m_greenMatter = 6;
	uint m_hexiiCountPerLayer[MAX_LAYERS] = { 0 };
	bool m_tabsEnabled[5] = { false };
};

extern void to_json(nlohmann::json& j, const Resources& resources);