#pragma once

#include "Maths.h"
#include <vector>

class Currencies {
public:
	static constexpr uint MAX_LAYERS = 6;

private:
	// Singleton pattern

	Currencies();
	Currencies(const Currencies&) = delete;
	Currencies(Currencies&&) = delete;

public:
	static Currencies* instance();

	/// Green matter

	inline static BigReal getGreenMatter() { return instance()->m_greenMatter; }
	// Adds op to green matter count. Throws if op is unaffordable
	void addGreenMatter(BigReal op);

	/// Hexii counts

	inline static uint getHexiiCountInLayer(uint layer) { return layer >= Currencies::MAX_LAYERS ? 0 : instance()->m_hexiiCountPerLayer[layer]; }
	// Adds to the count of the hexii in `layer`
	void addHexInLayer(uint layer);

private:
	static Currencies* m_instance;

	BigReal m_greenMatter;
	uint m_hexiiCountPerLayer[MAX_LAYERS];
};