#include "Dock.h"
#include "HexiiInfoTab.h"
#include "SettingsTab.h"
#include "Console.h"
#include "Resources.h"

USING_NS_CC;

bool Dock::init() {
    m_background = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("UI/Dock/DockBackground.png"));
    m_background->setAnchorPoint(Vec2(0.0, 0.0));
    this->addChild(m_background, 0);

    setContentSize(m_background->getContentSize());
    printf("Size of frame: %.3f %.3f\n", Director::getInstance()->getOpenGLView()->getFrameSize().width, Director::getInstance()->getOpenGLView()->getFrameSize().height);

    std::string paths[3]{ "HexiiInfoTab", "FractureTab", "SettingsTab" };

    // Hexii info tab
    m_tabs[0] = HexiiInfoTab::create();
    // Fracture tab
    m_tabs[1] = Layer::create();
    // Settings tab
    m_tabs[2] = SettingsTab::create();

    for (unsigned int i = 0; i < 3; i++) {
        initTab(i, m_tabs[i], { 0, 0 }, { 1.0f, 0.5f }, { 115, 335 }, { 0, -120 }, "UI/Dock/" + paths[i], "UI/Dock/DisabledTab",
            i == 2 || Resources::getInstance()->getTabEnabled(i) // Enable the tab if it's the settings tab or if it's enabled in the save file
        );
    }

    /// Listeners

    m_unlockListeners[0] = EventUtility::addGlobalEventListener(Hexii::EVENT_PURCHASE, this, &Dock::onProgressionHexiiPurchased); // EventListenerCustom::create("onHexiiPurchase", [this](cocos2d::EventCustom*) {enableTab(0);});    

    //for (uint i = 0; i < 2; i++) if (m_unlockListeners[i] != nullptr) _eventDispatcher->addEventListenerWithSceneGraphPriority(m_unlockListeners[i], this);

   
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

void Dock::onProgressionHexiiPurchased(cocos2d::EventCustom*) {
    enableTab(0);
}

void Dock::enableTab(uint tabNumber, bool autoSwitch) {
    TabbedWidget::enableTab(tabNumber, autoSwitch);

    Resources::getInstance()->setTabEnabled(tabNumber, true);
    // Tab doesn't need to listen for the unlock trigger anymore
    if(tabNumber < 2) _eventDispatcher->removeEventListener(m_unlockListeners[tabNumber]);
}
