#pragma once

#include "cocos2d.h"
#include <math.h>

typedef long double BigReal;
typedef unsigned long long BigInt;
typedef unsigned int uint;

constexpr float SQRT_3 = 1.73205f;
constexpr float RECIP_SQRT_3 = 0.57735;
constexpr float HALF_SQRT_3 = SQRT_3 * 0.5f;
constexpr float PI = M_PI;
constexpr float HALF_PI = PI * 0.5f;
constexpr float PI_THIRDS = PI / 3.0f;
constexpr float PI_SIXTHS = PI / 6.0f;
constexpr float TWO_PI = PI * 2.0f;
constexpr float HEXAGON_HEIGHT_TO_WIDTH = (2.0f / 3.0f) * SQRT_3;
constexpr float HEXAGON_WIDTH_TO_HEIGHT = 1.5f * RECIP_SQRT_3;

extern std::string formatBigReal(BigReal val, bool floor = true, uint significantFigures = 4, uint decimalPlaces = 0);
extern std::string formatBigInt(BigInt val, uint significantFigures = 4);

// Rounds the x and y components of `target` to the nearest integer
extern cocos2d::Vec2 roundVec2(const cocos2d::Vec2& target);
extern cocos2d::Vec3 roundVec3(const cocos2d::Vec3& target);
extern cocos2d::Vec2 absVec2(const cocos2d::Vec2& target);
extern cocos2d::Vec3 absVec3(const cocos2d::Vec3& target);