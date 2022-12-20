#pragma once

#include <variant>
#include "BigInt.hpp"

class HexUpgrade {
private:
	typedef std::variant<float, BigInt> vart;
public:
	HexUpgrade(float magnitude);
	HexUpgrade(BigInt magnitude);

	enum class Target {
		YIELD=1,
		SPEED=2
	} target;

	BigInt& actUpon(unsigned int level, BigInt& target);

private:
	vart m_magnitude;
};