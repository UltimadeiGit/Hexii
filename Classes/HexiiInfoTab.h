#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include <cocos/ui/UIScrollView.h>

#include "Maths.h"
#include "Hexii.h"
#include "ProgressBar.h"
#include "Upgrade.h"
#include "CompoundLabel.h"
#include "BeeGroupUpgradeBox.h"

class HexiiInfoTab : 
    public cocos2d::Layer {
private:
    HexiiInfoTab();
    HexiiInfoTab(const HexiiInfoTab&) = delete;
    HexiiInfoTab(HexiiInfoTab&&) = delete;

    bool init();
public:
    virtual void update(float dt) override;

    CREATE_FUNC(HexiiInfoTab);

private:

    /// Overhaul
    
    void setFocusHexii(Hexii* focus);
    void setFocusBeeGroup(BeeGroup* focus);

    void updateHexiiLabels(bool nameLabel, bool yieldLabel, bool expLabels);
    void updateBeeGroupLabels(bool nameLabel, bool countLabel);
    void updateScrollView();

    /// Events

    // Whenever a hex is clicked, possibly switch focus to that
    void onHexiiFocus(cocos2d::EventCustom* evnt);
    // When one of the bee group tabs is pressed, this callback switches focus to that bee group
    void onBeeGroupFocus(Ref*, cocos2d::ui::Widget::TouchEventType evntType, uint tabNumber);

    void onBeeGained(cocos2d::EventCustom* evnt);
    void onPurchaseEXPButtonPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType);
    void purchaseEXP();

    // The hexii whose details are listed in this tab
    Hexii* m_focusHexii = nullptr;
    // The bee group whose upgrades are listed in this tab
    BeeGroup* m_focusBeeGroup = nullptr;

    cocos2d::EventListenerCustom* m_beeGainedListener = nullptr;

    // The amount of EXP the purchase button will give
    BigReal m_purchaseEXPButtonDesiredEXP = 0;
    // The amount of nectar the exp purchase will cost
    BigReal m_costToPurchaseDesiredEXP = 0;

    std::vector<cocos2d::ui::Button*> m_beeGroupTabButtons;

    cocos2d::Sprite* m_backgroundOverlay = nullptr;
    // Sprite displaying the focused hex
    cocos2d::Sprite* m_focusSprite = nullptr;
    cocos2d::Label* m_hexiiNameLabel = nullptr;
    cocos2d::Label* m_hexiiDistrictLabel = nullptr;
    CompoundLabel* m_expLabel = nullptr;
    CompoundLabel* m_yieldLabel = nullptr;
    cocos2d::Label* m_yieldSpeedLabel = nullptr;
    CompoundLabel* m_nextBeeLabel = nullptr;
    cocos2d::Label* m_beeNameLabel = nullptr;
    cocos2d::Label* m_beeCountLabel = nullptr;

    cocos2d::ui::Button* m_purchaseEXPButton = nullptr;
    CompoundLabel* m_purchaseEXPNectarCostLabel = nullptr;
    CompoundLabel* m_purchaseEXPDesiredEXPLabel = nullptr;

    cocos2d::ui::ScrollView* m_upgradeScrollView = nullptr;
    std::vector<BeeGroupUpgradeBox*> m_upgradeBoxes;
};