#include "Tooltip.h"
#include "Console.h"

Tooltip* Tooltip::s_currentTooltip = nullptr;

constexpr cocos2d::Vec2 TOOLTIP_PADDING { 20, 10 };

bool Tooltip::init() {
    // Background
    m_background = cocos2d::Sprite::create("UI/Tooltip/Background.png");
    m_background->setAnchorPoint({ 0, 0 });
    this->addChild(m_background, 0);

    // Update content size
    Node::setContentSize(m_background->getContentSize());

    return true;
}

void Tooltip::setContent(cocos2d::ui::RichText* content) {
    if (m_content != nullptr) this->removeChild(m_content);

    m_content = content;
    m_content->ignoreContentAdaptWithSize(false);
    m_content->setPosition({ TOOLTIP_PADDING.x, getContentSize().height - TOOLTIP_PADDING.y });
    m_content->setContentSize(m_background->getContentSize() - cocos2d::Size(TOOLTIP_PADDING * 2));
    m_content->setAnchorPoint({ 0, 1 });
    m_content->setFontFace("fonts/BreeSerif.ttf");
    this->addChild(m_content, 1);
}

TooltipWidget::~TooltipWidget() {
    if (m_tooltip) m_tooltip->release();
}

bool TooltipWidget::init() {
    setInner(m_inner);

    m_inner->setAnchorPoint({0, 0 });
    m_inner->setPosition({ 0, 0 });
    this->addChild(m_inner, 0);

    m_tooltip->setVisible(false);
    this->addChild(m_tooltip, 1);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    // Listener to handle hover
    auto mouseListener = cocos2d::EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(TooltipWidget::onMouseMove, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
#endif

    /*

    // Listener to handle touch
    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(TooltipWidget::onTouchBegan, this);

    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    */

    setTouchEnabled(true);
    setFocusEnabled(false);

    Widget::addTouchEventListener(CC_CALLBACK_2(TooltipWidget::onTouch, this));

    return true;
}

void TooltipWidget::setInner(cocos2d::Node* inner) {
    if (m_inner->getParent() == this) return;

    m_inner = inner;

    if (m_inner == nullptr) return;

    // Match this widget to inner as closely as possible

    // Size 
    setContentSize(m_inner->getContentSize());    
}

void TooltipWidget::update(float dt) {
    if(m_shown) m_shownTime += dt;

    // The tooltip is visible if the widget is hovered over or if it is sticky
    toggleTooltip(
        m_hovering ||
        (m_stickiness < 0 && m_sticky) ||
        (m_stickiness > 0 && m_shownTime < m_stickiness && m_sticky)
    );
}

void TooltipWidget::toggleTooltip(bool shown) {
    if (m_shown == shown) return;

    m_shown = shown;

    if (m_shown) {
        // Show tooltip by adding it to the scene
        m_tooltip->setPosition((cocos2d::Vec2)getContentSize() + TOOLTIP_PADDING);
        m_tooltip->setVisible(true);

        // Reset stickiness timer
        m_shownTime = 0.0f;        
    }
    else {
        m_tooltip->setVisible(false);
    }

}

void Tooltip::setVisible(bool visible) {
    Node::setVisible(visible);
    if (visible) {
        if (s_currentTooltip && s_currentTooltip != this) {
            s_currentTooltip->setVisible(false);
            s_currentTooltip = this;
        }
        _director->setNotificationNode(this);
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

void TooltipWidget::onMouseMove(cocos2d::EventMouse* mouse) {
    // If the mouse is over the widget, show the tooltip
    // Check if the mouse is hovering over the widget
    m_hovering = this->getBoundingBox().containsPoint(getParent()->convertToNodeSpace({mouse->getCursorX(), mouse->getCursorY()}));

    if (m_hovering) {
        bool dbg = true;
	}
}

#endif

bool TooltipWidget::onTouch(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
    if(evntType != cocos2d::ui::Widget::TouchEventType::BEGAN) return false;

    // If the widget is not currently sticky, sticky it. Otherwise, unsticky it (will take effect next frame)
    if (!m_sticky) m_shownTime = 0.0f;
   
    m_sticky = !m_sticky;

    return true;
}


TooltipWidget::TooltipWidget(cocos2d::Node* inner, Tooltip* tooltip)
    : m_inner(inner), m_tooltip(tooltip)
{}