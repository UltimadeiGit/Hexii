#include "HexPlane.h"
#include "Console.h"
#include "Maths.h"
#include "Resources.h"
#include "JSON.hpp"

USING_NS_CC;
using namespace nlohmann;

HexPlane* HexPlane::m_instance = nullptr;

HexPlane::HexPlane(const float hexHeight) : m_hexHeight(hexHeight)
{}

HexPlane::HexPlane(const json& data) : m_hexHeight(data.at("hexHeight")) {}

bool HexPlane::init() {
    if (m_instance != nullptr) err("Hex plane already exists");

    /// Declare listeners

    auto touchListener = EventListenerTouchOneByOne::create();

    touchListener->onTouchBegan = CC_CALLBACK_2(HexPlane::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HexPlane::onTouchEnded, this);

    touchListener->setSwallowTouches(true);

#ifdef CC_PLATFORM_PC
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = CC_CALLBACK_1(HexPlane::onMousePressed, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(HexPlane::onMouseUp, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(HexPlane::onMouseMoved, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
#endif

    /// Add the listeners

    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexYield", CC_CALLBACK_1(HexPlane::onHexYield, this)), this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexPurchase", CC_CALLBACK_1(HexPlane::onHexPurchase, this)), this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexFocus", CC_CALLBACK_1(HexPlane::onHexFocus, this)), this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onPinButtonPressed", CC_CALLBACK_1(HexPlane::onPinButtonPressed, this)), this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    /// Init the camera

    m_camera = Camera::create();
    m_camera->setPosition(getPosition());
    m_camera->setDepth(-1);
    m_camera->setCameraFlag(CameraFlag::USER1);
    //m_camera->runAction(RepeatForever::create(MoveBy::create(0.1, Vec2(-5.0, 0))));

    setCameraMask((unsigned short)m_camera->getCameraFlag(), false);

    this->addChild(m_camera);

    m_instance = this;

    // Prevents this plane from being autoreleased at any point
    this->retain();

    return true;
}

bool HexPlane::init(const json& data) {
    if (!init()) return false;

    /// Resurrect the hexii
    
    json hexii = data.at("hexii");
    uint dbgc = 0;

    for (json::iterator it = hexii.begin(); it != hexii.end(); it++) {
        auto a = *it;
        Hex* newHex = Hex::create(a);

        placeHexAtPos(newHex, it->at("posAxial").get<cocos2d::Vec2>());
        dbgc++;
    }

    /*

    dbgc = 0;

    for (json::iterator it = hexii.begin(); it != hexii.end(); it++) {
        Hex* hex = getHexAtPos(it->at("posAxial").get<cocos2d::Vec2>());

        // Add their yield targets
    
        json yieldTargets = it->at("yieldTargets");
        for (json::iterator it = yieldTargets.begin(); it != yieldTargets.end(); it++) {
            Vec2 targetPos = it->at("posAxial").get<cocos2d::Vec2>();

            hex->addYieldTarget(getHexAtPos(targetPos), CC_RADIANS_TO_DEGREES(Vec2(localPositionOf(targetPos) - localPositionOf(hex->getAxialPosition())).getAngle()));
        }
        dbgc++;
    }
    */

    return true;
}

void HexPlane::update(float dt) {
    // Update all hexii
    for (auto it = m_hexMap.begin(); it != m_hexMap.end(); it++) it->second->update(dt);
}

bool HexPlane::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* evnt) {
    Vec2 touchPos = getTouchPos(touch);

    // Collide the touch with a hex
    Hex* hexTouched = getHexAtPos(axialPositionOf(touchPos - getPosition()));

    // Nothing left to do if not propagate to a hex
    if (hexTouched == nullptr) return false;

    hexTouched->onTouchBegan();

    return true;
}

void HexPlane::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* evnt) {}

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID

void HexPlane::onMousePressed(cocos2d::EventMouse* mouse) {
    EventMouse::MouseButton pressType = mouse->getMouseButton();

    // Right clicks
    if (pressType == EventMouse::MouseButton::BUTTON_RIGHT) {
        // TODO: Implement a camera that can pan
    }
}

void HexPlane::onMouseUp(cocos2d::EventMouse* mouse) {}

void HexPlane::onMouseMoved(cocos2d::EventMouse* mouse) {
    Vec2 mousePos = getMousePos(mouse);

    Hex* previousMouseOver = m_mouseOverHex;

    m_mouseOverHex = getHexAtPos(round(axialPositionOf(mousePos - getPosition())));

    // Detect a change in mouseover hex
    if (m_mouseOverHex != previousMouseOver) {
        // If the mouse moved off a hex
        if (previousMouseOver) previousMouseOver->onHoverEnd();

        // If the mouse moved onto a hex
        if (m_mouseOverHex) m_mouseOverHex->onHoverBegan();
    }
}

#endif

void HexPlane::onHexYield(EventCustom* evnt) {
    Hex::EventHexYieldData* data = static_cast<Hex::EventHexYieldData*>(evnt->getUserData());

    // TODO: Remove
    //auto neighbors = neighborsOf(data->posAxial, true);
    //for (auto& neighbor : neighbors) if (neighbor.hex->getLayer() < data->layer) neighbor.hex->addEXP(data->yield);
}

void HexPlane::onHexPurchase(cocos2d::EventCustom* evnt) {
    Hex::EventHexPurchaseData* data = static_cast<Hex::EventHexPurchaseData*>(evnt->getUserData());

    placeHexAtPos(data->subject, data->posAxial);
}

void HexPlane::onHexFocus(cocos2d::EventCustom* evnt) {
    Hex::EventHexFocusData* data = static_cast<Hex::EventHexFocusData*>(evnt->getUserData());

    // Can't pan when pinned
    if (m_pinned) return;

    // Pan over to the hex

    Vec2 hexLocalPos = localPositionOf(data->posAxial);

    auto* panAction = EaseQuadraticActionInOut::create(MoveTo::create(0.4, Vec3(hexLocalPos.x, hexLocalPos.y, m_camera->getPositionZ())));
    panAction->setTag(ACTION_TAG_PAN_CAMERA);

    m_camera->stopActionByTag(ACTION_TAG_PAN_CAMERA);
    m_camera->runAction(panAction);
}

void HexPlane::onPinButtonPressed(cocos2d::EventCustom* evnt) {
    bool pressed = *static_cast<bool*>(evnt->getUserData());

    // Toggle pinned state
    m_pinned = !m_pinned;
}

Hex* HexPlane::placeHexAtPos(cocos2d::Vec2 posAxial) {
    // Use the nearest point to the given pos
    posAxial = round(posAxial);

    Hex* newHex = Hex::create(layerOf(posAxial), posAxial);

    return placeHexAtPos(newHex, posAxial);
}

Hex* HexPlane::placeHexAtPos(Hex* hex, cocos2d::Vec2 posAxial) {
    hex->setPosition(localPositionOf(posAxial));
    hex->setCameraMask((unsigned short)m_camera->getCameraFlag(), false);

    // Verify a different hex does not already exist at the given position
    auto it = m_hexMap.find(posAxial);
    if (it != m_hexMap.end()) {
        // Cleanup the existing hex at that pos, only if these aren't the same objects
        if (it->second != hex) {
            warn("About to overwrite existing object at (" + std::to_string(posAxial.x) + "," + std::to_string(posAxial.y) + ")");

            this->removeChild(it->second);
            m_hexMap.erase(it);

            m_hexMap.emplace(std::make_pair(posAxial, hex));
            this->addChild(hex);
        }
    }
    // Add the new hex the map, only if it really is a new hex
    else {
        m_hexMap.emplace(std::make_pair(posAxial, hex));
        this->addChild(hex);
    }

    // If `hex` is active, then any neighboring positions that don't yet have an inactive hex need to have one so they can be purchased

    if (hex->getActive()) {
        auto neighbors = neighborsOf(posAxial, false);
        uint layer = layerOf(posAxial);

        for (auto& neighbor : neighbors) {
            // For each of the neighbors, one of five things will happen \
            Case 0: The neighbor is of the same layer. Nothing should happen in this case \
            Case 1: The neighbor does not exist yet and is of a lower layer. Nothing should happen in this case \
            Case 2: The neighbor does not exist yet and is of a higher layer, meaning an inactive hex should be set at that position \
            Case 3: The neighbor does exist and is of a lower layer, meaning this hex being purchase should have that neighbor added as a yield target \
            Case 4: The neighbor does exist and is of a higher layer, meaning that neighbor should add this hex being purchased as a yield target

            uint neighborLayer = layerOf(neighbor.pos);

            // Case 0
            if (neighborLayer == layer) continue;

            // Cases 1 & 2
            if (neighbor.hex == nullptr || !neighbor.hex->getActive()) {
                // Case 1
                if (neighborLayer < layer) continue;

                // Case 2
                else if (neighbor.hex == nullptr && neighborLayer < Resources::MAX_LAYERS) neighbor.hex = placeHexAtPos(neighbor.pos);
            }
            // Cases 3 & 4
            else {
                // TODO: Probably a smarter, faster way of calculating this angle using hexagon geometry?

                // Case 3
                if (neighborLayer < layer)
                    hex->addYieldTarget(neighbor.hex, CC_RADIANS_TO_DEGREES(Vec2(localPositionOf(neighbor.pos) - localPositionOf(posAxial)).getAngle()));

                // Case 4
                else
                    neighbor.hex->addYieldTarget(hex, CC_RADIANS_TO_DEGREES(Vec2(localPositionOf(posAxial) - localPositionOf(neighbor.pos)).getAngle()));
            }
        }
    }    

    return hex;
}

Hex* HexPlane::getHexAtPos(Vec2 posAxial) const {
    posAxial = round(posAxial);

    auto it = m_hexMap.find(posAxial);
    if (it != m_hexMap.end()) return it->second;

    return nullptr;
}

std::vector<HexPlane::HexPosPair> HexPlane::getHexiiInLayer(uint layer) const {
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

std::vector<HexPlane::HexPosPair> HexPlane::neighborsOf(cocos2d::Vec2 posAxial, bool activeOnly) const {
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

BigReal HexPlane::getAdjacencyBonuses(const Hex* target) const {
    auto neighbors = this->neighborsOf(target->getAxialPosition(), true);
    BigReal bonus = 0;
    for (uint i = 0; i < neighbors.size(); i++) {
        bonus += neighbors[i].hex->getAdjacencyBonusFromSupportUpgrade();
    }

    return bonus;
}

void to_json(nlohmann::json& j, const HexPlane& plane) {
    json hexii = json::array();

    // Adds the hexii ordered by layer
    for (uint i = 0; i < Resources::MAX_LAYERS; i++) {
        for (auto hexPosPair : plane.getHexiiInLayer(i)) {
            hexii.push_back(*hexPosPair.hex);
        }
    }    

    j = json{
        {"hexHeight", plane.getHexHeight() },
        {"hexii", hexii}
    };
}
