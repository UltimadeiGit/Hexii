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

    ProgressBar* m_hexEXPBar;
    // Sprite displaying the focused hex
    cocos2d::Sprite* m_focusSprite;
    cocos2d::Label* m_layerLabel;
    CompoundLabel* m_levelLabel;
    CompoundLabel* m_expLabel;
    CompoundLabel* m_yieldLabel;
    cocos2d::ui::Button* m_purchaseEXPButton;
    cocos2d::ui::ScrollView* m_upgradeScrollView;
};