#include "HexiiPlane.h"
#include "Console.h"
#include "Maths.h"
#include "Resources.h"
#include "JSON.hpp"

USING_NS_CC;
using namespace nlohmann;

HexiiPlane* HexiiPlane::m_instance = nullptr;

HexiiPlane::HexiiPlane(const float hexHeight) : m_hexHeight(hexHeight)
{}

HexiiPlane::HexiiPlane(const json& data) : m_hexHeight(data.at("hexHeight")) {}

bool HexiiPlane::init() {
    if (m_instance != nullptr) err("Hexii plane already exists");

    /// Declare listeners

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = CC_CALLBACK_1(HexiiPlane::onMousePressed, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(HexiiPlane::onMouseUp, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(HexiiPlane::onMouseMoved, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

#endif
    auto touchListener = EventListenerTouchOneByOne::create();

    touchListener->onTouchBegan = CC_CALLBACK_2(HexiiPlane::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HexiiPlane::onTouchEnded, this);

    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    /// Add event listeners

    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexYield", CC_CALLBACK_1(HexiiPlane::onHexYield, this)), this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexPurchase", CC_CALLBACK_1(HexiiPlane::onHexPurchase, this)), this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexiiFocus", CC_CALLBACK_1(HexiiPlane::onHexFocus, this)), this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onPinButtonPressed", CC_CALLBACK_1(HexiiPlane::onPinButtonPressed, this)), this);
   
    
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

bool HexiiPlane::init(const json& data) {
    if (!init()) return false;

    /// Resurrect the hexii
    
    json hexii = data.at("hexii");

    for (json::iterator it = hexii.begin(); it != hexii.end(); it++) {
        auto a = *it;
        Hexii* newHex = Hexii::create(a);

        placeHexAtPos(newHex, it->at("posAxial").get<cocos2d::Vec2>());
    }

    return true;
}

void HexiiPlane::update(float dt) {
    // Update all hexii
    for (auto it = m_hexMap.begin(); it != m_hexMap.end(); it++) it->second->update(dt);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    // If the camera is panning, simulate mouse movement
    if (m_camera->getActionByTag(ACTION_TAG_PAN_CAMERA)) onMouseMoved(nullptr);
#endif
}

void HexiiPlane::panCameraTo(Hexii* hex) {
    // Can't pan when pinned, or when the hex is inactive
    if (m_pinned || !hex->getActive()) return;

    // Pan over to the hex

    // Vec2 hexLocalPos = Hexagon::axialToPixel(posAxial, m_hexHeight);

    Vec2 hexLocalPos = hex->getPosition();

    auto* panAction = EaseQuadraticActionInOut::create(MoveTo::create(0.4, Vec3(hexLocalPos.x, hexLocalPos.y, m_camera->getPositionZ())));
    panAction->setTag(ACTION_TAG_PAN_CAMERA);

    // Stop any previous pan actions
    m_camera->stopActionByTag(ACTION_TAG_PAN_CAMERA);
    m_camera->runAction(panAction);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

void HexiiPlane::onMousePressed(cocos2d::EventMouse* mouse) {
    //static EventMouse::MouseButton lastClickType;// = EventMouse::MouseButton::BUTTON_UNSET;
    
    EventMouse::MouseButton currentClickType = mouse->getMouseButton();
    
    bool rightClick = currentClickType == EventMouse::MouseButton::BUTTON_RIGHT;

    // Pan the camera to the hex if it was right clicked
    if (m_mouseOverHex && rightClick) {
        panCameraTo(m_mouseOverHex);
    }
}

void HexiiPlane::onMouseUp(cocos2d::EventMouse* mouse) {}

void HexiiPlane::onMouseMoved(cocos2d::EventMouse* mouse) {
    // `mouse` can be nullptr if this is simulated, e.g. by the camera panning

    //cocos2d::Director::getInstance()->get

    Vec2 mousePos = mouse ? mouse->getLocationInView() : m_lastMousePos;
    Vec2 cameraMousePos = getCameraMousePos(mousePos);

    Hexii* previousMouseOverHex = m_mouseOverHex;

    m_mouseOverHex = getHexAtPos(Hexagon::pixelToAxial(cameraMousePos - getPosition(), m_hexHeight));

    // Detect a change in mouseover hex
    if (m_mouseOverHex != previousMouseOverHex) {
        // If the mouse moved off a hex
        if (previousMouseOverHex) previousMouseOverHex->onHoverEnd();

        // If the mouse moved onto a hex
        if (m_mouseOverHex) m_mouseOverHex->onHoverBegan();
    }

    m_lastMousePos = mousePos;
}

#endif

bool HexiiPlane::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* evnt) {
    static Timestamp lastClickTime = 0;
    static Vec2 lastTouchPos = getTouchPos(touch);

    Timestamp currentTime = timeSinceEpochMs();
    Vec2 touchPos = getTouchPos(touch);

    // Double taps (two taps within 0.3 seconds and at most 20 units apart)
    bool doubleTap = (currentTime < lastClickTime + 300 && (lastTouchPos - touchPos).getLengthSq() < (20 * 20));

    // Collide the touch with a hex
    Hexii* hexTouched = getHexAtPos(Hexagon::pixelToAxial(touchPos - getPosition(), m_hexHeight));

    if (hexTouched) {
        hexTouched->onTouchBegan();

        // Pan the camera to the hex if it was double tapped 
        if (doubleTap) panCameraTo(hexTouched);
    }

    // Update statics
    lastClickTime = currentTime;
    lastTouchPos = touchPos;

    return true;
}

void HexiiPlane::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* evnt) {}


void HexiiPlane::onHexYield(EventCustom* evnt) {
    Hexii::EventHexiiYieldData* data = static_cast<Hexii::EventHexiiYieldData*>(evnt->getUserData());

    // TODO: Remove
    //auto neighbors = neighborsOf(data->posAxial, true);
    //for (auto& neighbor : neighbors) if (neighbor.hex->getDistrict() < data->District) neighbor.hex->addEXP(data->yield);
}

void HexiiPlane::onHexPurchase(cocos2d::EventCustom* evnt) {
    Hexii::EventHexiiPurchaseData* data = static_cast<Hexii::EventHexiiPurchaseData*>(evnt->getUserData());

    placeHexAtPos(data->subject, data->posAxial);
}

void HexiiPlane::onHexFocus(cocos2d::EventCustom* evnt) {
    // Hexii::EventHexiiFocusData* data = static_cast<Hexii::EventHexiiFocusData*>(evnt->getUserData());
}

void HexiiPlane::onPinButtonPressed(cocos2d::EventCustom* evnt) {
    bool pressed = *static_cast<bool*>(evnt->getUserData());

    // Toggle pinned state
    m_pinned = !m_pinned;
}

Hexii* HexiiPlane::placeHexAtPos(cocos2d::Vec2 posAxial) {
    // Use the nearest point to the given pos
    posAxial = Hexagon::axialRound(posAxial);

    Hexii* newHex = Hexii::create(Hexagon::layerOf(posAxial), posAxial);

    return placeHexAtPos(newHex, posAxial);
}

Hexii* HexiiPlane::placeHexAtPos(Hexii* hex, cocos2d::Vec2 posAxial) {
    hex->setPosition(Hexagon::axialToPixel(posAxial, m_hexHeight));
    hex->setCameraMask((unsigned short)m_camera->getCameraFlag(), true);

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
    // Add the new hex to the map, only if it really is a new hex
    else {
        m_hexMap.emplace(std::make_pair(posAxial, hex));
        this->addChild(hex);
    }

    // If `hex` is active, then any neighboring positions that don't yet have an inactive hex need to have one so they can be purchased

    if (hex->getActive()) {
        auto neighbors = neighborsOf(posAxial, false);
        uint District = Hexagon::layerOf(posAxial);

        for (auto& neighbor : neighbors) {
            // For each of the neighbors, one of five things will happen \
            Case 0: The neighbor is of the same District. Nothing should happen in this case \
            Case 1: The neighbor does not exist yet and is of a lower District. Nothing should happen in this case \
            Case 2: The neighbor does not exist yet and is of a higher District, meaning an inactive hex should be set at that position \
            Case 3: The neighbor does exist and is of a lower District, meaning this hex being purchase should have that neighbor added as a yield target \
            Case 4: The neighbor does exist and is of a higher District, meaning that neighbor should add this hex being purchased as a yield target

            uint neighborDistrict = Hexagon::layerOf(neighbor.pos);

            // Case 0
            if (neighborDistrict == District) continue;

            // Cases 1 & 2
            if (neighbor.hex == nullptr || !neighbor.hex->getActive()) {
                // Case 1
                if (neighborDistrict < District) continue;

                // Case 2
                else if (neighbor.hex == nullptr && neighborDistrict < Resources::MAX_DISTRICTS) neighbor.hex = placeHexAtPos(neighbor.pos);
            }
            // Cases 3 & 4
            else {
                // TODO: Probably a smarter, faster way of calculating this angle using hexagon geometry?

                // Case 3
                if (neighborDistrict < District)
                    hex->addYieldTarget(neighbor.hex, CC_RADIANS_TO_DEGREES(Vec2(Hexagon::axialToPixel(neighbor.pos, m_hexHeight) - Hexagon::axialToPixel(posAxial, m_hexHeight)).getAngle()));

                // Case 4
                else
                    neighbor.hex->addYieldTarget(hex, CC_RADIANS_TO_DEGREES(Vec2(Hexagon::axialToPixel(posAxial, m_hexHeight) - Hexagon::axialToPixel(neighbor.pos, m_hexHeight)).getAngle()));
            }
        }
    }    

    return hex;
}

Hexii* HexiiPlane::getHexAtPos(Vec2 posAxial) const {
    posAxial = Hexagon::axialRound(posAxial);

    auto it = m_hexMap.find(posAxial);
    if (it != m_hexMap.end()) return it->second;

    return nullptr;
}

std::vector<HexiiPlane::HexiiPosPair> HexiiPlane::getHexiiInDistrict(uint District) const {
    std::vector<HexiiPosPair> hexii;

    // TODO: Optimize
    for (auto it = m_hexMap.begin(); it != m_hexMap.end(); it++) {
        if (it->second->getDistrict() != District) continue;

        HexiiPosPair hex;
        hex.pos = it->first;
        hex.hex = it->second;
    
        hexii.push_back(hex);
    }

    return hexii;
}

std::vector<HexiiPlane::HexiiPosPair> HexiiPlane::neighborsOf(cocos2d::Vec2 posAxial, bool activeOnly) const {
    // Enforce integer coords
    posAxial = Hexagon::axialRound(posAxial);

    std::vector<HexiiPosPair> neighbors;

    // I've opted for hard code rather than programatically calculating the positions because this is so much cleaner
    Vec2 neighborOffsets[6]{
        // Starts vertically above and rotates clockwise
        Vec2(0, 1), Vec2(1, 0), Vec2(1, -1), Vec2(0, -1), Vec2(-1, 0), Vec2(-1, 1)
    };

    for (unsigned int i = 0; i < 6; i++) {
        HexiiPosPair neighbor;
        neighbor.pos = Vec2(posAxial.x + neighborOffsets[i].x, posAxial.y + neighborOffsets[i].y);
        neighbor.hex = getHexAtPos(neighbor.pos);

        // If only nodes with hexii on them are permitted (`activeOnly`) then only append this neighbor if it is both non-null and active
        if (!activeOnly || (neighbor.hex != nullptr && neighbor.hex->getActive())) neighbors.push_back(neighbor);
    }

    return neighbors;
}

BigReal HexiiPlane::getAdjacencyBonuses(const Hexii* target) const {
    auto neighbors = this->neighborsOf(target->getAxialPosition(), true);
    BigReal bonus = 0;
    for (uint i = 0; i < neighbors.size(); i++) {
        Hexii* hex = neighbors[i].hex;

        bonus += hex->getAdjacencyYieldMultiplier() - 1;
    }

    return bonus;
}

void to_json(nlohmann::json& j, const HexiiPlane& plane) {
    json hexii = json::array();

    // Adds the hexii ordered by District
    for (uint i = 0; i < Resources::MAX_DISTRICTS; i++) {
        for (auto hexPosPair : plane.getHexiiInDistrict(i)) {
            hexii.push_back(*hexPosPair.hex);
        }
    }    

    j = json{
        {"hexHeight", plane.getHexHeight() },
        {"hexii", hexii}
    };
}
