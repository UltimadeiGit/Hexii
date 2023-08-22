#pragma once

#include <cocos2d.h>
#include "HexiiPlane.h"
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

    cocos2d::Camera* m_backgroundCamera;

    Dock* m_dock;
    CurrencyHUD* m_currencyHUD;

    HexiiPlane* m_plane;
};

