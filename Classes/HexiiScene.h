#pragma once

#include "cocos2d.h"
#include "HexPlane.h"

class HexiiScene :
    public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* ev) {}
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev) {}
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* ev) {}

    virtual void onMouseMoved(cocos2d::EventMouse* mouse);

    virtual void update(float dt) override;

    CREATE_FUNC(HexiiScene);
private:

    cocos2d::DrawNode* m_debugNodes[10];
    cocos2d::Vec2 m_mousePos;
    cocos2d::Label* m_debugLabel;

    HexPlane* m_plane;
};

