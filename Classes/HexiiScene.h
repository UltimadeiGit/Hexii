#pragma once

#include <cocos2d.h>
#include "HexPlane.h"
#include "Sidebar.h"
#include "CurrencyHUD.h"

class HexiiScene :
    public cocos2d::Scene {
private:
    bool init();

public:
    static cocos2d::Scene* createScene();    

    CREATE_FUNC(HexiiScene);

    void update(float dt) override;

private:
    /// UI & HUD

    Sidebar* m_sidebar;
    CurrencyHUD* m_currencyHUD;

    cocos2d::DrawNode* m_debugNodes[10];
    cocos2d::Label* m_debugLabel;

    cocos2d::Vec2 m_mousePos;

    HexPlane* m_plane;
};

