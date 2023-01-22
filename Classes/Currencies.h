#pragma once

#include "Maths.h"
#include <vector>

class Currencies {
private:
	// Singleton pattern

	Currencies();
	Currencies(const Currencies&) = delete;
	Currencies(Currencies&&) = delete;

public:
	static Currencies* instance();

	inline static BigReal getGreenMatter() { return instance()->m_greenMatter; }
	// Adds op to green matter count.
	// If op was negative (a transaction), returns false if it was unaffordable. Always true otherwise
	bool addGreenMatter(BigReal op);

private:
	static Currencies* m_instance;

	BigReal m_greenMatter;
};