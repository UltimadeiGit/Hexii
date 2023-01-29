#include "Sidebar.h"
#include "HexInfoTab.h"
#include "SettingsTab.h"
#include "Console.h"
#include "Resources.h"

USING_NS_CC;

bool Sidebar::init() {
    m_background = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("components/SidebarBackground.png"));
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
        m_tabs[i]->setPosition(Vec2(0, 0));
        m_tabs[i]->setVisible(false);

        const std::string istr = std::to_string(i + 1);

        m_tabButtons[i] = ui::Button::create(
            "widgets/tabs/Tab" + istr + "Neutral.png",
            "widgets/tabs/Tab" + istr + "Selected.png",
            "widgets/tabs/TabLocked.png"
        );

        m_tabButtons[i]->setAnchorPoint(Vec2(0.0, 0.0));
        m_tabButtons[i]->setPosition(Vec2(69 * i, 4));

        if (i != 5) {
            m_tabButtons[i]->setEnabled(false);
            m_tabButtons[i]->setBright(false);

            if (Resources::getInstance()->getTabEnabled(i)) enableTab(nullptr, i);
        }
        
        m_tabButtons[i]->addTouchEventListener(CC_CALLBACK_2(Sidebar::onTabPressed, this, i));

        this->addChild(m_tabs[i]);
        this->addChild(m_tabButtons[i]);
    }

    /// Listeners

    m_unlockListeners[0] = EventListenerCustom::create("onHexPurchase", CC_CALLBACK_1(Sidebar::enableTab, this, 0));

    for (uint i = 0; i < 5; i++) if (m_unlockListeners[i] != nullptr) _eventDispatcher->addEventListenerWithSceneGraphPriority(m_unlockListeners[i], this);

    setContentSize(m_background->getContentSize());
    printf("Size of frame: %.3f %.3f\n", Director::getInstance()->getOpenGLView()->getFrameSize().width, Director::getInstance()->getOpenGLView()->getFrameSize().height);
   
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    // Sidebar wants to fit in 95% of the height
    setScale((Director::getInstance()->getOpenGLView()->getFrameSize().height * 0.95) / _contentSize.height);
#else
    setScale((Director::getInstance()->getOpenGLView()->getFrameSize().height * 0.8) / _contentSize.height);
#endif

    setFocusEnabled(false);
    setSwallowTouches(true);

    ui::Widget::setTouchEnabled(true);

    return true;
}

void Sidebar::update(float dt) {
    for (unsigned int i = 0; i < 6; i++) {
        // Only update a tab if it's visible (aka active)
        if(m_tabs[i]->isVisible()) m_tabs[i]->update(dt);
    }
}

void Sidebar::onTabPressed(Ref*, ui::Widget::TouchEventType evntType, uint tabNumber) {
    // Only care about release
    if (evntType != ui::Widget::TouchEventType::ENDED) return;

    switchTab(tabNumber);
}

void Sidebar::enableTab(cocos2d::EventCustom* evnt, uint tabNumber) {
    if (tabNumber > 4) return err("Tab number out of bounds");

    m_tabButtons[tabNumber]->setEnabled(true);
    m_tabButtons[tabNumber]->setBright(true);
    switchTab(tabNumber);

    Resources::getInstance()->setTabEnabled(tabNumber, true);

    // Tab doesn't need to listen for the unlock trigger anymore
    _eventDispatcher->removeEventListener(m_unlockListeners[tabNumber]);
}

void Sidebar::switchTab(unsigned short tab) {
    // Some tabs not yet implemented
    if (!m_tabButtons[tab]->isEnabled()) return;

    m_tabs[m_currentTab]->setVisible(false);
    m_tabButtons[m_currentTab]->loadTextureNormal("widgets/tabs/Tab" + std::to_string(m_currentTab + 1) + "Neutral.png");
    m_tabButtons[m_currentTab]->loadTexturePressed("widgets/tabs/Tab" + std::to_string(m_currentTab + 1) + "Selected.png");

    m_currentTab = tab;
    m_tabs[tab]->setVisible(true);
    // There are four textures: tab is locked, tab is unpressed and not active, tab is pressed and not active or tab is active
    m_tabButtons[tab]->loadTextureNormal("widgets/tabs/Tab" + std::to_string(tab + 1) + "Active.png");
    m_tabButtons[tab]->loadTexturePressed("widgets/tabs/Tab" + std::to_string(tab + 1) + "Active.png");
}
