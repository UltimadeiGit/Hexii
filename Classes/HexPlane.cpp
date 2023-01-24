#include "HexPlane.h"
#include "Console.h"
#include "Maths.h"

USING_NS_CC;

HexPlane::HexPlane(const float hexHeight) : m_hexHeight(hexHeight)
{}

Hex* HexPlane::getHexAtPos(Vec2 posAxial) const {
    posAxial = roundVec2(posAxial);

    auto it = m_hexMap.find(posAxial);
    if (it != m_hexMap.end()) return it->second;

    return nullptr;
}

std::vector<HexPlane::HexPosPair> HexPlane::getHexiiInLayer(uint layer) {
    std::vector<HexPosPair> hexii;

    for (auto it = m_hexMap.begin(); it != m_hexMap.end(); it++) {
        if (it->second->getLayer() != layer) continue;

        HexPosPair hex;
        hex.pos = it->first;
        hex.hex = it->second;
    
        hexii.push_back(hex);
    }

    return hexii;
}

Hex* HexPlane::placeHexAtPos(cocos2d::Vec2 posAxial) {
    Hex* ret = placeHexAtPos(posAxial, Hex::create(layerOf(posAxial)));
    ret->setPosition(localPositionOf(posAxial));
    return ret;
}

Hex* HexPlane::placeHexAtPos(Vec2 posAxial, Hex* hex) {
    // Use the nearest point to the given pos
    posAxial = round(posAxial);

    auto it = m_hexMap.find(posAxial);
    if (it != m_hexMap.end()) {
        warn("About to overwrite existing object at (" + std::to_string(posAxial.x) + "," + std::to_string(posAxial.y) + ")");
        
        // Cleanup the existing hex at that pos
        
        this->removeChild(it->second);
        m_hexMap.erase(it);
    }

    // Nullptr is treated as simply deleting the hex at `pos`, so no emplace needed
    if (hex == nullptr) return nullptr;    

    /*
    Size nodeSize = asNode->getContentSize();

    auto dbgLabel = Label::createWithTTF("(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")", "fonts/arial.ttf", 20.0f);
    dbgLabel->setPosition(nodeSize.width * 0.5f, nodeSize.height * 0.5f);
    dbgLabel->setTextColor(Color4B(127, 127, 127, 255));
    asNode->addChild(dbgLabel);
    */

    m_hexMap.emplace(std::make_pair(posAxial, hex));
    this->addChild(hex);

    return hex;
}


cocos2d::Vec2 HexPlane::localPositionOf(const Vec2& posAxial) const {
    // The y axis is just vertical, the x axis looks like a skew diagonal pointed upwards
    // therefore each 1 local unit in x corresponds to 0.75 widths AND 0.5 heights

    float localx = posAxial.x * 0.75f * m_hexHeight * HEXAGON_HEIGHT_TO_WIDTH;
    float localy = (posAxial.y + (posAxial.x * 0.5f)) * m_hexHeight;

    return Vec2(localx, localy);
}


cocos2d::Vec2 HexPlane::axialPositionOf(cocos2d::Vec2 posLocal) const {
    // Formulae derived from the reverse of localPositionOf

    const float recipNodeLength = 1.0f / m_hexHeight;

    float hexx = posLocal.x * 2 * RECIP_SQRT_3 * recipNodeLength;
    float hexy = (posLocal.y - (m_hexHeight * 0.5f) * hexx) * recipNodeLength;

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

std::vector<HexPlane::HexPosPair> HexPlane::neighborsOf(cocos2d::Vec2 posAxial, bool activeOnly) {
    // Enforce integer coords
    posAxial = round(posAxial);
    
    std::vector<HexPosPair> neighbors;

    // I've opted for hard code rather than programatically calculating the positions because this is so much cleaner
    Vec2 neighborOffsets[6]{
        // Starts vertically above and rotates clockwise
        Vec2(0, 1), Vec2(1, 0), Vec2(1, -1), Vec2(0, -1), Vec2(-1, 0), Vec2(-1, 1)
    };

    for (unsigned int i = 0; i < 6; i++) {
        HexPosPair neighbor;
        neighbor.pos = Vec2(posAxial.x + neighborOffsets[i].x, posAxial.y + neighborOffsets[i].y);
        neighbor.hex = getHexAtPos(neighbor.pos);

        // If only nodes with hexii on them are permitted (`activeOnly`) then only append this neighbor if it is both non-null and active
        if (!activeOnly || (neighbor.hex != nullptr && neighbor.hex->getActive())) neighbors.push_back(neighbor);
    }

    return neighbors;
}

void HexPlane::update(float dt) {
    // Update all hexii
    auto it = m_hexMap.begin();
    while (it != m_hexMap.end()) {
        it->second->update(dt);
        it++;
    }
}