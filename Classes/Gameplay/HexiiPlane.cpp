#include "HexiiPlane.h"
#include "Console.h"
#include "Maths.h"
#include "Resources.h"
#include "JSON.hpp"
#include "HexiiScene.h"

USING_NS_CC;
using namespace nlohmann;

HexiiPlane* HexiiPlane::m_instance = nullptr;

//constexpr cocos2d::Vec2 PLANE_CENTRE_OFFSET = {0, -200};

HexiiPlane::HexiiPlane(const float hexHeight) : m_hexHeight(hexHeight)
{}

HexiiPlane::HexiiPlane(const json& data) : m_hexHeight(data.at("hexHeight")) {}

bool HexiiPlane::init() {
    if (m_instance != nullptr) err("Hexii plane already exists");
    m_instance = this;

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

    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexiiYield", CC_CALLBACK_1(HexiiPlane::onHexiiYield, this)), this);
    
    EventUtility::addGlobalEventListener("onHexiiPurchase", this, &HexiiPlane::onHexiiPurchase);
    EventUtility::addGlobalEventListener("onHexiiFocus", this, &HexiiPlane::onHexiiFocus);
    EventUtility::addGlobalEventListener(Hexii::EVENT_UPGRADE_PURCHASED, this, &HexiiPlane::onHexiiUpgradePurchased);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexiiPurchase", CC_CALLBACK_1(HexiiPlane::onHexiiPurchase, this)), this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexiiFocus", CC_CALLBACK_1(HexiiPlane::onHexiiFocus, this)), this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onPinButtonPressed", CC_CALLBACK_1(HexiiPlane::onPinButtonPressed, this)), this);

    EventUtility::addGlobalEventListener(GameplayCommon::GameEvent::EVENT_SACRIFICE_CONFIRMED, this, &HexiiPlane::onSacrificeConfirmed);

    setCameraMask((unsigned short)cocos2d::CameraFlag::USER1, false);

    // Prevents this plane from being autoreleased at any point
    this->retain();

    return true;
}

bool HexiiPlane::init(const json& data) {
    if (!init()) return false;

    /// Resurrect the hexii
    
    json hexii = data.at("hexii");

    for (json::iterator it = hexii.begin(); it != hexii.end(); it++) {
        auto hexiiData = *it;
        Hexii* newHex = Hexii::create(hexiiData);

        if(newHex->getStandardUpgradeTracker()->getState("GlobalPower") == Upgrade::State::OWNED)
            Resources::getInstance()->addGlobalPowerUpgradeBonus();

        if(newHex->getActive())
            Resources::getInstance()->addHexiiInlayer(newHex->getLayer());

        placeHexAtPos(newHex, newHex->getAxialPosition());
    }

    return true;
}

void HexiiPlane::update(float dt) {
    // Update all hexii
    for (auto it = m_hexMap.begin(); it != m_hexMap.end(); it++) it->second->update(dt);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    // If the camera is panning, simulate mouse movement
    if (HexiiScene::getHexiiCamera()->getActionByTag(ACTION_TAG_PAN_CAMERA)) onMouseMoved(nullptr);
#endif
}

void HexiiPlane::panCameraTo(Hexii* hex) {
    // Can't pan when pinned, or when the hex is inactive
    if (m_pinned || !hex->getActive()) return;

    // Pan over to the hex

    // Vec2 hexLocalPos = Hexagon::axialToPixel(posAxial, m_hexHeight);

    Vec2 hexLocalPos = hex->getPosition();
    cocos2d::Camera* camera = HexiiScene::getHexiiCamera();

    auto* panAction = EaseQuadraticActionInOut::create(
        MoveTo::create(0.4, Vec3(hexLocalPos.x, hexLocalPos.y, camera->getPositionZ())));
    panAction->setTag(ACTION_TAG_PAN_CAMERA);

    // Stop any previous pan actions
    camera->stopActionByTag(ACTION_TAG_PAN_CAMERA);
    camera->runAction(panAction);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

void HexiiPlane::onMousePressed(cocos2d::EventMouse* mouse) {
    //static EventMouse::MouseButton lastClickType;// = EventMouse::MouseButton::BUTTON_UNSET;
    
    EventMouse::MouseButton currentClickType = mouse->getMouseButton();
    
    bool rightClick = currentClickType == EventMouse::MouseButton::BUTTON_RIGHT;

    // Pan the camera to the hex if it was right clicked
    if (m_mouseOverHex && rightClick) {
        m_mouseOverHex->focus();
        panCameraTo(m_mouseOverHex);
    }
}

void HexiiPlane::onMouseUp(cocos2d::EventMouse* mouse) {}

void HexiiPlane::onMouseMoved(cocos2d::EventMouse* mouse) {
    // `mouse` can be nullptr if this is simulated, e.g. by the camera panning

    //cocos2d::Director::getInstance()->get

    Vec2 mousePos = mouse ? mouse->getLocationInView() : m_lastMousePos;
    Vec2 cameraMousePos = CocosUtility::getCameraMousePos(HexiiScene::getHexiiCamera(), mousePos);

    Hexii* previousMouseOverHex = m_mouseOverHex;

    m_mouseOverHex = getHexiiAtPos(Hexagon::pixelToAxial(cameraMousePos - getPosition(), m_hexHeight));

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
    static Vec2 lastTouchPos = getTouchPos(HexiiScene::getHexiiCamera(), touch);
    static Hexii* lastHexiiTouched = nullptr;

    Timestamp currentTime = timeSinceEpochMs();
    Vec2 touchPos = getTouchPos(HexiiScene::getHexiiCamera(), touch);
   
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    constexpr float DOUBLE_TAP_MAX_TOUCH_DISTANCE = 20;
    constexpr int DOUBLE_TAP_MAX_TIME = 300;
#else 
    constexpr int DOUBLE_TAP_MAX_TIME = 500;
#endif

    // Collide the touch with a hex
    Hexii* hexiiTouched = getHexiiAtPos(Hexagon::pixelToAxial(touchPos - getPosition(), m_hexHeight));

    // Double taps (two taps on the same hexii within 0.3 seconds and at most 20 units apart)
    // On mobile, a double tap has no such distance restriction
    bool doubleTap = lastHexiiTouched == hexiiTouched && currentTime < lastClickTime + DOUBLE_TAP_MAX_TIME 
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
        && (lastTouchPos - touchPos).getLengthSq() < (DOUBLE_TAP_MAX_TOUCH_DISTANCE * DOUBLE_TAP_MAX_TOUCH_DISTANCE)
#endif
        ;
    

    if (hexiiTouched) {
        hexiiTouched->onTouchBegan();

        // Pan the camera to the hex if it was double tapped 
        if (doubleTap) {
            hexiiTouched->focus();
            panCameraTo(hexiiTouched);
        }
    }

    // Update statics
    lastClickTime = currentTime;
    lastTouchPos = touchPos;
    lastHexiiTouched = hexiiTouched;

    return true;
}

void HexiiPlane::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* evnt) {}


void HexiiPlane::onHexiiYield(EventCustom* evnt) {
    Hexii::EventHexiiYieldData* data = static_cast<Hexii::EventHexiiYieldData*>(evnt->getUserData());

    // TODO: Remove
    //auto neighbors = neighborsOf(data->posAxial, true);
    //for (auto& neighbor : neighbors) if (neighbor.hex->getLayer() < data->layer) neighbor.hex->addEXP(data->yield);
}

void HexiiPlane::onHexiiPurchase(cocos2d::EventCustom* evnt) {
    Hexii::EventHexiiPurchaseData* data = static_cast<Hexii::EventHexiiPurchaseData*>(evnt->getUserData());

    placeHexAtPos(data->subject, data->posAxial);
    data->subject->focus();
    panCameraTo(data->subject);
}

void HexiiPlane::onHexiiFocus(cocos2d::EventCustom* evnt) {
    // Hexii::EventHexiiFocusData* data = static_cast<Hexii::EventHexiiFocusData*>(evnt->getUserData());
}

void HexiiPlane::onHexiiUpgradePurchased(cocos2d::EventCustom* evnt) {
    auto dat = EventUtility::getEventData<Hexii::EventHexiiUpgradePurchasedData>(evnt);

    // Support upgrade requires setting adjacent hexii yields dirty
    if (dat->upgrade->name == "Support1") {
        auto neighbors = neighborsOf(dat->subject->getAxialPosition(), true);
        for (auto& neighbor : neighbors) neighbor.hex->markYieldDirty();
    }

    // Global power upgrade requires setting all hexii yields dirty, and updating the global power count
    if (dat->upgrade->name == "GlobalPower") {
        Resources::getInstance()->addGlobalPowerUpgradeBonus();
		for (auto& posHexPair : m_hexMap) posHexPair.second->markYieldDirty();
	}
}

void HexiiPlane::onPinButtonPressed(cocos2d::EventCustom* evnt) {
    bool pressed = *static_cast<bool*>(evnt->getUserData());

    // Toggle pinned state
    m_pinned = !m_pinned;
}

void HexiiPlane::onSacrificeConfirmed(cocos2d::EventCustom* evnt) {
    // Sacrifice the board and modify the appropriate resources

    // Clear green matter
    Resources::getInstance()->addGreenMatter(-Resources::getInstance()->getGreenMatter());
    Resources::getInstance()->addGreenMatter(6);
    Resources::getInstance()->addToSacrificeCount();

    // Remove all hexii
    for (auto& hexPosPair : m_hexMap) {
        auto* hex = hexPosPair.second;

        if(hex->getActive()) Resources::getInstance()->addHexiiInlayer(hex->getLayer(), -1);
        // Clear global power bonus if applicable
        if(hex->getStandardUpgradeTracker()->getState("GlobalPower") == Upgrade::State::OWNED) Resources::getInstance()->addGlobalPowerUpgradeBonus(-1);

        this->removeChild(hex);
    }

    m_hexMap.clear();
    placeHexAtPos({ 0, 0 });
}

Hexii* HexiiPlane::placeHexAtPos(cocos2d::Vec2 posAxial) {
    // Use the nearest point to the given pos
    posAxial = Hexagon::axialRound(posAxial);

    Hexii* newHex = Hexii::create(Hexagon::layerOf(posAxial), posAxial);

    return placeHexAtPos(newHex, posAxial);
}

Hexii* HexiiPlane::placeHexAtPos(Hexii* hex, cocos2d::Vec2 posAxial) {
    hex->setPosition(Hexagon::axialToPixel(posAxial, m_hexHeight));
    hex->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1, true);

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
        uint layer = Hexagon::layerOf(posAxial);

        for (auto& neighbor : neighbors) {
            // For each of the neighbors, one of five things will happen \
            Case 0: The neighbor is of the same layer. Nothing should happen in this case \
            Case 1: The neighbor does not exist yet and is of a lower layer. Nothing should happen in this case \
            Case 2: The neighbor does not exist yet and is of a higher layer, meaning an inactive hex should be set at that position \
            Case 3: The neighbor does exist and is of a lower layer, meaning this hex being purchase should have that neighbor added as a yield target \
            Case 4: The neighbor does exist and is of a higher layer, meaning that neighbor should add this hex being purchased as a yield target

            uint neighborlayer = Hexagon::layerOf(neighbor.pos);

            // Case 0
            if (neighborlayer == layer) continue;

            // Cases 1 & 2
            if (neighbor.hex == nullptr || !neighbor.hex->getActive()) {
                // Case 1
                if (neighborlayer < layer) continue;

                // Case 2
                else if (neighbor.hex == nullptr && neighborlayer < GameplayCommon::MAX_LAYER) neighbor.hex = placeHexAtPos(neighbor.pos);
            }
            // Cases 3 & 4
            else {
                // TODO: Probably a smarter, faster way of calculating this angle using hexagon geometry?

                // Case 3
                if (neighborlayer < layer)
                    hex->addYieldTarget(neighbor.hex, CC_RADIANS_TO_DEGREES(Vec2(Hexagon::axialToPixel(neighbor.pos, m_hexHeight) - Hexagon::axialToPixel(posAxial, m_hexHeight)).getAngle()));

                // Case 4
                else
                    neighbor.hex->addYieldTarget(hex, CC_RADIANS_TO_DEGREES(Vec2(Hexagon::axialToPixel(posAxial, m_hexHeight) - Hexagon::axialToPixel(neighbor.pos, m_hexHeight)).getAngle()));
            }
        }
    }    

    return hex;
}

Hexii* HexiiPlane::getHexiiAtPos(Vec2 posAxial) const {
    posAxial = Hexagon::axialRound(posAxial);

    auto it = m_hexMap.find(posAxial);
    if (it != m_hexMap.end()) return it->second;

    return nullptr;
}

std::vector<HexiiPlane::HexiiPosPair> HexiiPlane::getHexiiInlayer(uint layer) const {
    std::vector<HexiiPosPair> hexii;

    // TODO: Optimize
    for (auto it = m_hexMap.begin(); it != m_hexMap.end(); it++) {
        if (it->second->getLayer() != layer) continue;

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
        neighbor.hex = getHexiiAtPos(neighbor.pos);

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

    // Adds the hexii ordered by layer
    for (uint i = 0; i < GameplayCommon::MAX_LAYER; i++) {
        for (auto hexPosPair : plane.getHexiiInlayer(i)) {
            hexii.push_back(*hexPosPair.hex);
        }
    }    

    j = json{
        {"hexHeight", plane.getHexHeight() },
        {"hexii", hexii}
    };
}
