#pragma once

#include <cocos2d.h>
#include "HexiiPlane.h"
#include "Dock.h"
#include "Sidebar.h"
#include "CurrencyHUD.h"
#include "HexiiSacrificeHUD.h"

class HexiiScene :
    public cocos2d::Scene {
private:
    bool init();

public:
    static cocos2d::Scene* createScene();

    // TODO: Add type checking for the running scene
    static Dock* getDock() { return m_instance->m_dock; }
    static cocos2d::Camera* getHexiiCamera() { return m_instance->m_hexiiCamera; }

    CREATE_FUNC(HexiiScene);

    void update(float dt) override;

private:
    static HexiiScene* m_instance;

    void onSacrificeInitiated(cocos2d::EventCustom* evnt);
    void onSacrificeCancelled(cocos2d::EventCustom* evnt);
    void onSacrificeConfirmed(cocos2d::EventCustom* evnt);

    void closeSacrificeHUD();

    // Dbg
    void onProgression(cocos2d::EventCustom* evnt);

    /// UI & HUD

    cocos2d::Camera* m_hexiiCamera;

    Dock* m_dock;
    Sidebar* m_sidebar;

    CurrencyHUD* m_greenMatterHUD;
    CurrencyHUD* m_redMatterHUD;
    HexiiSacrificeHUD* m_sacrificeHUD = nullptr;

    HexiiPlane* m_plane;
};

