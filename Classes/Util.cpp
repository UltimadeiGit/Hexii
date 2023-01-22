#include <cstdio>

#include "Console.h"
#include "Maths.h"

USING_NS_CC;

// cpp file for all util headers

void warn(const std::string& msg) {
	printf("Warning: %s\n", msg.c_str());
}

void err(const std::string& msg) {
	printf("Error: %s\n", msg.c_str());
	ErrFlag = 1;
}

std::string formatBigReal(BigReal val, bool floor, uint significantFigures, uint decimalPlaces) {
	if (floor) val = std::floor(val);
	std::string formatted;

	double exponent = log10(val);

	// +2 is to account for the extra characters 'e' and the exponent being added in scientific notation
	if (exponent < significantFigures + 2) {
		// This is as annoying to write as it is to read, trust me
		switch (decimalPlaces) {
		default:
		case 0:
			formatted = std::format("{:.0f}", val);
			break;
		case 1:
			formatted = std::format("{:.1f}", val);
			break;
		case 2:
			formatted = std::format("{:.2f}", val);
			break;
		case 3:
			formatted = std::format("{:.3f}", val);
			break;
		}
	}
	else {
		// Use scientific notation
		// sf = 3
		// 12345 -> 1.23e4
		// First `sf` characters, plus the string "eX" where X is floor(exponent)

		formatted = std::format("{}", val).substr(0, significantFigures) + "e" + std::format("{}", std::floor(exponent));
	}	

	return formatted;
}

std::string formatBigInt(BigInt val, uint significantFigures) {
	return formatBigReal(val, true, significantFigures, 0);
}

Vec2 roundVec2(const Vec2& target) {
	return Vec2(roundf(target.x), roundf(target.y));
}

Vec3 roundVec3(const Vec3& target) {
	return Vec3(roundf(target.x), roundf(target.y), roundf(target.z));
}

Vec2 absVec2(const cocos2d::Vec2& target) {
	return Vec2(abs(target.x), abs(target.y));
}

Vec3 absVec3(const Vec3& target) {
	return Vec3(abs(target.x), abs(target.y), abs(target.z));
}