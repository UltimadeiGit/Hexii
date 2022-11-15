#pragma once

#include "cocos2d.h"

constexpr float SQRT_3 = 1.73205f;
constexpr float RECIP_SQRT_3 = 0.57735;
constexpr float HALF_SQRT_3 = SQRT_3 * 0.5f;
constexpr float HEXAGON_HEIGHT_TO_WIDTH = (2.0f / 3.0f) * SQRT_3;
constexpr float HEXAGON_WIDTH_TO_HEIGHT = 1.5f * RECIP_SQRT_3;

// Rounds the x and y components of `target` to the nearest integer
extern cocos2d::Vec2 roundVec2(cocos2d::Vec2 target);