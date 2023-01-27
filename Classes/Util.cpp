#include <cstdio>
#include <fmt/format.h>

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

	throw std::runtime_error(msg);
}

std::string formatBigReal(BigReal val, bool floor, uint significantFigures, uint decimalPlaces) {
//#ifdef _FORMAT_
	if (floor) val = std::floor(val);
	std::string formatted;

	double exponent = log10(val);

	// 1,001,001 -> 1.001001e6 (7sf) 

	// Switch to exponential form after 10000
	if (exponent >= 4 && exponent >= significantFigures - 1) {
		formatted = fmt::format("{:.{}e}", val, significantFigures - 1);
		
		// The string before e+...
		std::string mantissaStr = formatted.substr(0, significantFigures + 1);
		// The string after e+
		std::string exponentStr = formatted.substr(significantFigures + 3);
		// Strip leading 0s
		exponentStr = exponentStr.erase(0, exponentStr.find_first_not_of('0'));

		formatted = mantissaStr + "e" + exponentStr;
	}
	else formatted = fmt::format("{:.{}f}", val, decimalPlaces);

	/*
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
		// TODO: This code fails for certain numbers so it clearly doesn't work the way I think it does\
		rewrite recommended.

		std::string exponentSuffix = "e" + std::format("{}", std::floor(exponent));

		// Formatting changes when exponent is greater than 16. Note significantfigures doesn't have a -1 because the '.' takes up an extra char 
		if(exponent >= 17) formatted = std::format("{}", val).substr(0, significantFigures + 1) + exponentSuffix;
		else {
			std::string processed = std::format("{}", val).substr(0, significantFigures);
			char mostSignificantBit = processed[0];
			formatted = std::string(1, mostSignificantBit) + "." + processed.substr(1) + exponentSuffix;
		}
	}	
	*/

	return formatted;
//#else
//	return std::to_string(val);
//#endif
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