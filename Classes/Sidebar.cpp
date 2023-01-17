#include "Sidebar.h"
#include "HexInfoTab.h"
#include "SettingsTab.h"

USING_NS_CC;

bool Sidebar::init() {
    m_background = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("SidebarBackground.png"));
    m_background->setAnchorPoint(Vec2(0.0, 0.0));

    this->addChild(m_background);

    // Tab 1
    m_tabs[0] = HexInfoTab::create();
    
    // Middle tabs

    m_tabs[1] = Layer::create();
    m_tabs[2] = Layer::create();
    m_tabs[3] = Layer::create();
    m_tabs[4] = Layer::create();
    m_tabs[5] = Layer::create();

    // Tab 6
    m_tabs[5] = SettingsTab::create();

    for (unsigned int i = 0; i < 6; i++) {
        // Set the size and position for all tabs (layers)
        m_tabs[i]->setAnchorPoint(Vec2(0.0, 0.0));
        m_tabs[i]->setContentSize(Size(420, 1320));
        m_tabs[i]->setPosition(Vec2(0, 25));
        m_tabs[i]->setVisible(false);

        const std::string istr = std::to_string(i + 1);

        m_tabButtons[i] = ui::Button::create(
            "tabs/Tab" + istr + "Neutral.png",
            "tabs/Tab" + istr + "Selected.png",
            "tabs/TabLocked.png"
        );

        m_tabButtons[i]->setAnchorPoint(Vec2(0.0, 0.0));
        m_tabButtons[i]->setPosition(Vec2(69 * i, 25));

        if (i != 0 && i != 5) {
            m_tabButtons[i]->setEnabled(false);
            m_tabButtons[i]->setBright(false);
        }
        
        // m_tabButtons[i]->addTouchEventListener

        this->addChild(m_tabs[i]);
        this->addChild(m_tabButtons[i]);
    }

    // Start with tab 1 already active

    switchTab(0);

    return true;
}

void Sidebar::update(float dt) {
    for (unsigned int i = 0; i < 6; i++) {
        // Only update a tab if it's visible (aka active)
        if(m_tabs[i]->isVisible()) m_tabs[i]->update(dt);
    }
}

void Sidebar::switchTab(unsigned short tab) {
    // Some tabs not yet implemented
    if (!m_tabButtons[tab]->isEnabled()) return;

    m_tabs[m_currentTab]->setVisible(false);
    m_tabButtons[m_currentTab]->loadTextureNormal("tabs/Tab" + std::to_string(m_currentTab + 1) + "Neutral.png");
    m_tabButtons[m_currentTab]->loadTexturePressed("tabs/Tab" + std::to_string(m_currentTab + 1) + "Selected.png");
    m_tabButtons[m_currentTab]->setBright(false);

    m_currentTab = tab;
    m_tabs[tab]->setVisible(true);
    // There are four textures: tab is locked, tab is unpressed and not active, tab is pressed and not active or tab is active
    m_tabButtons[tab]->loadTextureNormal("tabs/Tab" + std::to_string(tab + 1) + "Active.png");
    m_tabButtons[tab]->loadTexturePressed("tabs/Tab" + std::to_string(tab + 1) + "Active.png");
    m_tabButtons[tab]->setBright(true);
}
