#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIScrollView.h>

#include "BigInt.hpp"
#include "Hex.h"
#include "ProgressBar.h"

class HexInfoTab : public cocos2d::Layer {
public:
    bool init();
    virtual void update(float dt) override;

    inline void setFocus(Hex* focus) { m_focus = focus; }

    CREATE_FUNC(HexInfoTab);
private:
    cocos2d::ui::ScrollView* m_upgradesList;

    ProgressBar* m_hexEXPBar;

    // The hex whose details are listed in this tab
    Hex* m_focus = nullptr;

    /// TODO: Remove
    // Offset of the gm box (where the top left starts)
    // const cocos2d::Vec2 m_greenMatterBoxAnchor = cocos2d::Vec2(15, -70);
};