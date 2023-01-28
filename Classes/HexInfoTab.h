#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include <cocos/ui/UIScrollView.h>

#include "Maths.h"
#include "Hex.h"
#include "ProgressBar.h"
#include "Upgrades.h"
#include "CompoundLabel.h"
#include "HexUpgradeBox.h"

class HexInfoTab : 
    public cocos2d::Layer {
private:
    HexInfoTab();
    HexInfoTab(const HexInfoTab&) = delete;
    HexInfoTab(HexInfoTab&&) = delete;

    bool init();
public:
    virtual void update(float dt) override;

    void setFocus(Hex* focus);    

    CREATE_FUNC(HexInfoTab);

private:
    /// Events

    void onHexLevelUp(cocos2d::EventCustom* evnt);
    // Whenever a hex is clicked, possibly switch focus to that
    void onHexFocus(cocos2d::EventCustom* evnt);
    void onHexUpgradePurchase(cocos2d::EventCustom* evnt);

    void onPinButtonPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType);

    // Called whenever focus changes or hex levels up
    void updateUpgradesList(BigInt levelBefore, BigInt level);
    HexUpgradeBox* addUpgradeToList(UpgradePtr upgrade);

    void purchaseEXP();

    // The hex whose details are listed in this tab
    Hex* m_focus = nullptr;

    // The amount of EXP the purchase button will give
    BigReal m_purchaseEXPButtonDesiredEXP = 0;

    // The locked ones need to be kept track of in order to reveal them
    std::vector<HexUpgradeBox*> m_previewUpgradeBoxes;

    // If pinned, touching a hex won't change this info tab's focus
    bool m_pinned = false;

    ProgressBar* m_hexEXPBar = nullptr;
    // Sprite displaying the focused hex
    cocos2d::Sprite* m_focusSprite = nullptr;
    cocos2d::Label* m_layerLabel = nullptr;
    CompoundLabel* m_levelLabel = nullptr;
    CompoundLabel* m_expLabel = nullptr;
    CompoundLabel* m_yieldLabel = nullptr;
    cocos2d::ui::Button* m_purchaseEXPButton = nullptr;
    cocos2d::ui::Button* m_pinButton = nullptr;
    cocos2d::ui::ScrollView* m_upgradeScrollView = nullptr;
};