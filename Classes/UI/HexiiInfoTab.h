#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include <cocos/ui/UIScrollView.h>
#include <cocos/ui/UILayout.h>

#include "Maths.h"
#include "Hexii.h"
#include "ProgressBar.h"
#include "Upgrade.h"
#include "CompoundLabel.h"
#include "UpgradeBox.h"
#include "HexiiEXPTab.h"
#include "HexiiUpgradesTab.h"
#include "TabbedWidget.h"

#define HEXII_INFO_TAB_COUNT 2

// Though this is a tab in the dock, it itself is a tabbed widget
class HexiiInfoTab : 
    public TabbedWidget<HEXII_INFO_TAB_COUNT> {
private:

    HexiiInfoTab();
    HexiiInfoTab(const HexiiInfoTab&) = delete;
    HexiiInfoTab(HexiiInfoTab&&) = delete;

    bool init();
public:
    virtual void update(float dt) override;

    void setFocusHexii(Hexii* focus);
    
    CREATE_FUNC(HexiiInfoTab);

private:    
    void updateNameLabel();
    void updateStatLabels();
    //void updateSpecializationLabels(bool nameLabel, bool countLabel);
    // void updateScrollView();

    /// Events

    // Whenever a hex is clicked, possibly switch focus to that
    void onHexiiFocus(cocos2d::EventCustom* evnt);
    void switchTab(unsigned short tab) override;

    void onLevelGained(cocos2d::EventCustom* evnt);

    /// Tabs

    inline HexiiTab* getCurrentTab() { return dynamic_cast<HexiiTab*>(m_tabs[m_currentTab]); }
    void initUpgradesTab(unsigned short tabNumber, HexiiUpgradesTab* tab, const std::string& pathName);
    inline bool isCurrentTabAnUpgradesTab() { return m_currentTab != 0; }

    /// Members

    // Sub tabs

    HexiiEXPTab* m_expInfoTab = nullptr;
    HexiiUpgradesTab* m_standardL0UpgradesTab = nullptr;
    HexiiUpgradesTab* m_standardUpgradesTab = nullptr;

    // The hexii whose details are listed in this tab
    Hexii* m_focusHexii = nullptr;

    cocos2d::EventListenerCustom* m_levelGainedListener = nullptr;

    cocos2d::Sprite* m_backgroundOverlay = nullptr;
    // Sprite displaying the focused hex
    cocos2d::Sprite* m_focusSprite = nullptr;
    cocos2d::Label* m_hexiiNameLabel = nullptr;
    cocos2d::Label* m_hexiilayerLabel = nullptr;
    // Labels within the info plate

    // Level labels (In aggregate, says "Level [TOTAL LEVEL] ([RAW LEVEL] + [LEVELS FROM RED MATTER])")
    // Says Level [TOTAL LEVEL]
    CompoundLabel* m_levelLabel = nullptr;
    // Says ([RAW LEVEL]
    CompoundLabel* m_rawLevelLabel = nullptr;
    // Says + [LEVELS FROM RED MATTER]
    CompoundLabel* m_levelsFromRedMatterLabel = nullptr;
    // For the closing bracket
    cocos2d::Label* m_levelLabelRight = nullptr;

    CompoundLabel* m_expLabel = nullptr;
    CompoundLabel* m_nextLevelLabel = nullptr;
    //cocos2d::Label* m_nextLevelRight = nullptr;
    CompoundLabel* m_yieldLabel = nullptr;
    cocos2d::Label* m_yieldTimeLeft = nullptr;
    cocos2d::Label* m_yieldTimeRight = nullptr;
    cocos2d::Label* m_yieldSpeedLabel = nullptr;
    //CompoundLabel* m_specNameLabel = nullptr;

    // Level scaling labels
    // Says "Level Scale: [MULTIPLIER]"
    CompoundLabel* m_scalingLabel = nullptr;
    // Says "from [RED MATTER INVESTED]"
    CompoundLabel* m_redMatterLabel = nullptr;


    friend class HexiiFirstUpgradePurchasedProgressionEvent;
};