#include "HexPlane.h"
#include "Console.h"
#include "Maths.h"

USING_NS_CC;

Hex* HexPlane::get(Vec2 pos) const {
    pos = roundVec2(pos);

    auto it = m_inner.find(pos);
    if (it != m_inner.end()) return it->second;

    return nullptr;
}

Hex* HexPlane::set(cocos2d::Vec2 pos) {
    return set(pos, Hex::create(layerOf(pos), localPositionOf(pos)));
}

Hex* HexPlane::set(Vec2 pos, Hex* hex) {
    pos = round(pos);

    auto it = m_inner.find(pos);
    if (it != m_inner.end()) {
        warn("About to overwrite existing object at (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ")");
        m_inner.erase(it);
    }

    // Nullptr is treated as simply deleting the hex at `pos`, so no emplace needed
    if (hex != nullptr) {
        m_inner.emplace(std::make_pair(pos, hex));

        /*
        Size nodeSize = asNode->getContentSize();

        auto dbgLabel = Label::createWithTTF("(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")", "fonts/arial.ttf", 20.0f);
        dbgLabel->setPosition(nodeSize.width * 0.5f, nodeSize.height * 0.5f);
        dbgLabel->setTextColor(Color4B(127, 127, 127, 255));
        asNode->addChild(dbgLabel);
        */

        this->addChild(hex->asNode());
    }

    return hex;
}


cocos2d::Vec2 HexPlane::localPositionOf(const Vec2& pos) const {
    // The y axis is just vertical, the x axis looks like a skew diagonal pointed upwards
    // therefore each 1 local unit in x corresponds to 0.75 widths AND 0.5 heights
        
    // worldx = pos.x * m_nodeLength * HEXAGON_HEIGHT_TO_WIDTH;
    // worldy = (pos.y + (pos.x * 0.5f)) * m_nodeLength;

    float gamex = pos.x * 0.75f * m_nodeLength * HEXAGON_HEIGHT_TO_WIDTH;
    float gamey = (pos.y + (pos.x * 0.5f)) * m_nodeLength;

    return Vec2(gamex, gamey);
}


cocos2d::Vec2 HexPlane::hexPositionOf(cocos2d::Vec2 pos) const
{
    // Formulae derived from the reverse of localPositionOf

    const float recipNodeLength = 1.0f / m_nodeLength;

    float hexx = pos.x * 2 * RECIP_SQRT_3 * recipNodeLength;
    float hexy = (pos.y - (m_nodeLength * 0.5f) * hexx) * recipNodeLength;

    return Vec2(hexx, hexy);
}


float HexPlane::layerOf(const Vec2& pos) {
    // In a cube coord system, layer is simply max(|x|, |y|, |z|)
    // Additionally, x + y + z = 0 so z = -x - y
    // We use an axial system so z can be derived and layer from that

    return std::max(std::max(std::abs(pos.x), std::abs(pos.y)), std::abs(-pos.x - pos.y));
}


cocos2d::Vec2 HexPlane::round(cocos2d::Vec2 pos) {
    Vec3 cubeCoords(pos.x, pos.y, -pos.x - pos.y);
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

std::vector<HexPlane::HexPosPair> HexPlane::neighborsOf(cocos2d::Vec2 pos, bool activeOnly) {
    // Enforce integer coords
    pos = round(pos);
    
    std::vector<HexPosPair> neighbors;

    for (int x = -1; x <= 1; x++) {
        for (int j = 0; j <= 1; j++) {
            // Requirements:
            // x: -1, -1,  0, 0,  1, 1
            // y:  1,  0, -1, 1, -1, 0
            // This line ensures that the requirements are met
            // TODO: Figure out how to get rid of the if statement (ternary operator)
            int y = x == 0 ? (2 * j) - 1 : -j * x;
            HexPosPair neighbor;
            neighbor.pos = Vec2(pos.x + x, pos.y + y);
            neighbor.hex = get(neighbor.pos);
            if (!activeOnly || (neighbor.hex != nullptr && neighbor.hex->getActive())) neighbors.push_back(neighbor);
        }        
    }

    return neighbors;
}

void HexPlane::update(float dt) {
    auto it = m_inner.begin();
    while (it != m_inner.end()) {
        it->second->update(dt);
        it++;
    }
}