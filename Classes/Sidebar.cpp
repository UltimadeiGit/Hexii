#include "Sidebar.h"
#include "HexiiInfoTab.h"
#include "SettingsTab.h"
#include "Console.h"
#include "Resources.h"

USING_NS_CC;

bool Dock::init() {
    m_background = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("components/DockBackground.png"));
    m_background->setAnchorPoint(Vec2(0.0, 0.0));
    this->addChild(m_background, 0);


    setContentSize(m_background->getContentSize());
    printf("Size of frame: %.3f %.3f\n", Director::getInstance()->getOpenGLView()->getFrameSize().width, Director::getInstance()->getOpenGLView()->getFrameSize().height);

    Sprite* icons[3];

    // Hexii info tab
    m_tabs[0] = HexiiInfoTab::create();
    icons[0] = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("icons/tabs/HexiiInfoTabIcon.png"));
    
    // District tab
    m_tabs[1] = Layer::create();
    icons[1] = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("icons/tabs/DistrictTabIcon.png"));

    // Settings tab
    m_tabs[2] = SettingsTab::create();
    icons[2] = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("icons/tabs/SettingsTabIcon.png"));


    for (unsigned int i = 0; i < 3; i++) {
        // Set the size and position for all tabs
        m_tabs[i]->setAnchorPoint(Vec2(0.0, 0.0));
        m_tabs[i]->setPosition(Vec2(0, 0));
        m_tabs[i]->setVisible(false);

        const std::string istr = std::to_string(i + 1);

        m_tabButtons[i] = ui::Button::create(
            "widgets/tabs/TabHorizontalInactive.png",
            "widgets/tabs/TabHorizontalSelected.png",
            "widgets/tabs/TabHorizontalDisabled.png"
        );

        m_tabButtons[i]->setPosition(Vec2(115, 335 - (120 * i)));
        m_tabButtons[i]->setAnchorPoint(Vec2(1.0, 0.5));

        icons[i]->setContentSize(Size(90, 90));
        icons[i]->setPosition(Vec2(62, 66));
        icons[i]->setAnchorPoint(Vec2(0.5, 0.5));
        icons[i]->setTag(1);
        m_tabButtons[i]->addChild(icons[i]);

        if (i != 2) {
            m_tabButtons[i]->setEnabled(false);
            m_tabButtons[i]->setBright(false);
            icons[i]->setVisible(false);

            if (Resources::getInstance()->getTabEnabled(i)) enableTab(nullptr, i);
        }
        
        m_tabButtons[i]->addTouchEventListener(CC_CALLBACK_2(Dock::onTabPressed, this, i));

        this->addChild(m_tabs[i], 1);
        this->addChild(m_tabButtons[i], -1);
    }

    //m_tabs[0]->addChild()

    /// Listeners

    m_unlockListeners[0] = EventListenerCustom::create("onHexPurchase", CC_CALLBACK_1(Dock::enableTab, this, 0));

    for (uint i = 0; i < 2; i++) if (m_unlockListeners[i] != nullptr) _eventDispatcher->addEventListenerWithSceneGraphPriority(m_unlockListeners[i], this);

   
    /*
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    // Dock wants to fit in 95% of the height
    setScale((Director::getInstance()->getOpenGLView()->getFrameSize().height * 0.95) / _contentSize.height);
#else
    setScale((Director::getInstance()->getOpenGLView()->getFrameSize().height * 0.8) / _contentSize.height);
#endif
*/

    setFocusEnabled(false);
    setSwallowTouches(true);

    ui::Widget::setTouchEnabled(true);

    return true;
}

void Dock::update(float dt) {
    for (unsigned int i = 0; i < 3; i++) {
        // Only update a tab if it's visible (aka active)
        if(m_tabs[i]->isVisible()) m_tabs[i]->update(dt);
    }
}

void Dock::onTabPressed(Ref*, ui::Widget::TouchEventType evntType, uint tabNumber) {
    // Only care about release
    if (evntType != ui::Widget::TouchEventType::ENDED) return;

    switchTab(tabNumber);
}

void Dock::enableTab(cocos2d::EventCustom* evnt, uint tabNumber) {
    if (tabNumber > 1) return err("Tab number out of bounds");

    m_tabButtons[tabNumber]->setEnabled(true);
    m_tabButtons[tabNumber]->setBright(true);
    m_tabButtons[tabNumber]->getChildByTag(1)->setVisible(true);
    switchTab(tabNumber);

    Resources::getInstance()->setTabEnabled(tabNumber, true);

    // Tab doesn't need to listen for the unlock trigger anymore
    _eventDispatcher->removeEventListener(m_unlockListeners[tabNumber]);
}

void Dock::switchTab(unsigned short tab) {
    // Some tabs not yet implemented
    if (!m_tabButtons[tab]->isEnabled()) return;

    m_tabs[m_currentTab]->setVisible(false);
    m_tabButtons[m_currentTab]->loadTextureNormal("widgets/tabs/TabHorizontalInactive.png");
    m_tabButtons[m_currentTab]->loadTexturePressed("widgets/tabs/TabHorizontalSelected.png");

    m_currentTab = tab;
    m_tabs[tab]->setVisible(true);
    // The texture should remain the active texture when the tab is selected
    m_tabButtons[tab]->loadTextureNormal("widgets/tabs/TabHorizontalActive.png");
    m_tabButtons[tab]->loadTexturePressed("widgets/tabs/TabHorizontalActive.png");
}
