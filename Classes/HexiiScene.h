#pragma once

#include "cocos2d.h"
#include "HexPlane.h"

class HexiiScene :
    public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(HexiiScene);
private:

    HexPlanePtr m_plane;
};

