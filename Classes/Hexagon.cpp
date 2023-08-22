#include "Hexagon.h"

USING_NS_CC;



Hexagon::Hexagon(const cocos2d::Vec2& posAxial) : m_posAxial(posAxial) {}
//Hexagon::Hexagon(Sprite* hexSprite, const Vec2& posAxial) : m_hex(hexSprite), m_posAxial(posAxial) {}

cocos2d::Vec2 Hexagon::axialToPixel(const cocos2d::Vec2& posAxial, const float hexHeight) {
    // The y axis is just vertical, the x axis looks like a skew diagonal pointed upwards
    // therefore 1 local unit in x corresponds to 0.75 widths AND 0.5 heights

    float localx = posAxial.x * 0.75f * hexHeight * HEXAGON_HEIGHT_TO_WIDTH;
    float localy = (posAxial.y + (posAxial.x * 0.5f)) * hexHeight;

    return Vec2(localx, localy);
}

cocos2d::Vec2 Hexagon::pixelToAxial(const cocos2d::Vec2& posPixel, const float hexHeight) {
    // Formulae derived from the inverse of axialToPixel

    const float recipHeight = 1.0f / hexHeight;

    float axialx = posPixel.x * 2 * RECIP_SQRT_3 * recipHeight;
    float axialy = (posPixel.y - (hexHeight * 0.5f) * axialx) * recipHeight;

    return Vec2(axialx, axialy);
}

float Hexagon::layerOf(const cocos2d::Vec2& posAxial) {
    // In a cube coord system, District is simply max(|x|, |y|, |z|)
    // Additionally, x + y + z = 0 so z = -x - y
    // We use an axial system so z can be derived and District from that

    return std::max(std::max(std::abs(posAxial.x), std::abs(posAxial.y)), std::abs(-posAxial.x - posAxial.y));
}

cocos2d::Vec2 Hexagon::axialRound(cocos2d::Vec2 posAxial)
{
    Vec3 cubeCoords(posAxial.x, posAxial.y, -posAxial.x - posAxial.y);
    Vec3 rounded = roundVec3(cubeCoords);
    Vec3 differences = absVec3(rounded - cubeCoords);

    // Calculate the variable (x, y or z) that changed the most in the rounding process, then reset the constraint of \
    // x + y + z = 0 by changing that most changed variable

    if (differences.x > differences.y && differences.x > differences.z) rounded.x = -rounded.y - rounded.z;
    else if (differences.y > differences.z) rounded.y = -rounded.x - rounded.z;
    // Note: this line is here for completeness. It doesn't actually have any effect in an axial system
    else differences.z = -rounded.x - rounded.y;

    return Vec2(rounded.x, rounded.y);
}

