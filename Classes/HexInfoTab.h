#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIScrollView.h>

#include "BigInt.hpp"
#include "Hex.h"
#include "ProgressBar.h"
#include "CompoundLabel.h"

class HexInfoTab : public cocos2d::Layer {
private:
    HexInfoTab();
public:
    bool init();
    virtual void update(float dt) override;

    void setFocus(Hex* focus);

    CREATE_FUNC(HexInfoTab);

private:
    // The hex whose details are listed in this tab
    Hex* m_focus = nullptr;

    cocos2d::ui::ScrollView* m_upgradesList;

    ProgressBar* m_hexEXPBar;
    // Sprite displaying the focused hex
    cocos2d::Sprite* m_focusSprite;
    cocos2d::Label* m_layerLabel;
    CompoundLabel* m_levelLabel;
    CompoundLabel* m_expLabel;
    CompoundLabel* m_yieldLabel;
};