#include <math.h>

#include "HexagonCoordinatePlane.h"
#include "Console.h"
#include "Maths.h"

USING_NS_CC;

template <typename _Ty>
std::shared_ptr<_Ty> HexagonCoordinatePlane<_Ty>::get(Vec2 pos) const {
    pos = roundVec2(pos);

    auto it = m_inner.find(pos);
    if (it != m_inner.end()) return it->second;

    return nullptr;
}

template<typename _Ty>
std::shared_ptr<_Ty> HexagonCoordinatePlane<_Ty>::set(cocos2d::Vec2 pos) {
    return _TyPtr();
}

template <typename _Ty>
std::shared_ptr<_Ty> HexagonCoordinatePlane<_Ty>::set(Vec2 pos, _TyPtr obj) {
    pos = roundVec2(pos);

    auto it = m_inner.find(pos);
    if (it != m_inner.end()) {
        warn("About to overwrite existing object at (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ")");
        m_inner.erase(it);
    }

    // Nullptr is treated as simply clearing the obj at `pos`, so no emplace needed
    if (obj != nullptr) {
        m_inner.emplace(std::make_pair(pos, obj));
        Node* asNode = reinterpret_cast<Node*>(obj.get());
        asNode->setPosition(worldPositionOf(pos));
        this->addChild(asNode);
    }

    return obj;
}

template<typename _Ty>
cocos2d::Vec2 HexagonCoordinatePlane<_Ty>::worldPositionOf(const cocos2d::Vec2& pos) const {
    // The y axis is just vertical, the x axis looks like a skew diagonal pointed upwards
    // therefore each 1 unit in x corresponds 
        
    // worldx = pos.x * m_nodeLength * HEXAGON_HEIGHT_TO_WIDTH;
    // worldy = (pos.y + (pos.x * 0.5f)) * m_nodeLength;

    return cocos2d::Vec2(
        pos.x * m_nodeLength * HEXAGON_HEIGHT_TO_WIDTH, 
        (pos.y + (pos.x * 0.5f)) * m_nodeLength
    );
}

template <typename _Ty>
unsigned int HexagonCoordinatePlane<_Ty>::layerOf(const Vec2& pos) {
    // In an x,y,z system, layer is simply max(|x|, |y|, |z|)
    // Additionally, x + y + z = 0 so z = -x - y
    // We use an x,y system so z can be derived and layer from that

    return std::max(std::max(std::abs((int)pos.x), std::abs((int)pos.y)), std::abs(-(int)pos.x - (int)pos.y));
}

class Hex;

template std::shared_ptr<Hex> HexagonCoordinatePlane<Hex>::get(Vec2) const;
template bool HexagonCoordinatePlane<Hex>::initWithSize(float);
template Vec2 HexagonCoordinatePlane<Hex>::worldPositionOf(const Vec2& pos) const;
template unsigned int HexagonCoordinatePlane<Hex>::layerOf(const Vec2&);
template std::shared_ptr<Hex> HexagonCoordinatePlane<Hex>::set(Vec2);
template std::shared_ptr<Hex> HexagonCoordinatePlane<Hex>::set(Vec2, std::shared_ptr<Hex>);