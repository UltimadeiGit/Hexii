#pragma once

#include <cocos2d.h>

#include "BigInt.hpp"
#include "Hex.h"
#include "ProgressBar.h"

class InfoBox :
    public cocos2d::Sprite {
public:
    bool init();
    virtual void update(float dt) override;

    void updateGreenMatterAmount(const BigInt& amount);
    void updateGreenMatterIncreaseAmount(const BigInt& amount);

    inline void setFocus(Hex* focus) { m_focus = focus; }

    CREATE_FUNC(InfoBox);
private:
    cocos2d::Label* m_greenMatterAmount;
    // Amount gm is increasing per second
    cocos2d::Label* m_greenMatterIncreaseAmount;
    cocos2d::Label* m_perSecLabel;

    cocos2d::Menu* m_upgradesMenu;

    ProgressBar* m_hexEXPBar;

    // The hex whose details are listed in this info box
    Hex* m_focus = nullptr;

    // Offset of the gm box (where the top left starts)
    const cocos2d::Vec2 m_greenMatterBoxAnchor = cocos2d::Vec2(15, -70);
};