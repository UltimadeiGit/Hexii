#pragma once

#include <cocos2d.h>
#include "HexPlane.h"
#include "Sidebar.h"
#include "CurrencyHUD.h"

class HexiiScene :
    public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

#ifdef CC_PLATFORM_MOBILE
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* ev);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev) {}
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* ev) {}
#endif

#ifdef CC_PLATFORM_PC
    void onMousePressed(cocos2d::EventMouse* mouse);
    void onMouseUp(cocos2d::EventMouse* mouse);
    void onMouseMoved(cocos2d::EventMouse* mouse);
#endif

    void update(float dt) override;

    CREATE_FUNC(HexiiScene);
private:
    void onHexYield(Hex* hex);

    BigReal getHexPurchaseCost(uint layer);
    void tryPurchaseHex(Hex* target);

    // Used to fix position vectors whose y component is flipped (i.e from a touch)
    cocos2d::Vec2 correctInvertedYVec(const cocos2d::Vec2& vec) const;

    uint m_hexiiCountPerLayer[6];

    /// UI & HUD

    Sidebar* m_sidebar;
    CurrencyHUD* m_currencyHUD;

    cocos2d::DrawNode* m_debugNodes[10];
    cocos2d::Label* m_debugLabel;

    cocos2d::Vec2 m_mousePos;

#ifdef CC_PLATFORM_PC
    Hex* m_mouseOverHex = nullptr;
#endif

    HexPlane* m_plane;
};

