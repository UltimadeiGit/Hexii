#include "PurchasableBox.h"
#include "PurchasableBox.h"
#include "ColourSchemes.h"

USING_NS_CC;

PurchasableBox::PurchasableBox(bool hidePurchaseButtonWhenUnaffordable, const std::string& footerConstPartStr)
    : m_hidePurchaseButtonWhenUnaffordable(hidePurchaseButtonWhenUnaffordable),
    m_footerConstPartStr(footerConstPartStr)
{}

bool PurchasableBox::init() {
    m_backgroundPlate = Sprite::create("UI/parts/ImagePlate.png");
    m_backgroundPlate->setAnchorPoint({ 0.0f, 0.0f });
    m_backgroundPlate->setPosition({ 0, 69 });
    m_backgroundPlate->setCascadeColorEnabled(true);
    this->addChild(m_backgroundPlate, 1);

    constexpr float NAMEPLATE_HEIGHT = 38.0f;
    constexpr float FOOTER_HEIGHT = 44.0f;
    const Size backgroundPlateSize = m_backgroundPlate->getContentSize();
    const float TOP_Y = backgroundPlateSize.height - (NAMEPLATE_HEIGHT / 2);

    m_icon = Sprite::create("icons/Missing.png");
    m_icon->setScale(0.32);
    m_icon->setAnchorPoint({ 0.5f, 0.5f });
    m_icon->setPosition(backgroundPlateSize.width / 2, backgroundPlateSize.height / 2);
    m_backgroundPlate->addChild(m_icon);

    //Widget::setFlippedY(true);

    m_nameLabel = Label::createWithTTF("???", "fonts/BreeSerif.ttf", 28, Size::ZERO, TextHAlignment::CENTER);
    m_nameLabel->setTextColor(Color4B::WHITE);
    m_nameLabel->enableOutline(Color4B::BLACK, 2);
    m_nameLabel->enableShadow(Color4B::BLACK, Size(1, 1), 2);
    m_nameLabel->setAnchorPoint({ 0.5f, 0.5f });
    m_nameLabel->setPosition({ backgroundPlateSize.width / 2, TOP_Y });
    m_backgroundPlate->addChild(m_nameLabel);

    m_footerLabel = CompoundLabel::create(m_footerConstPartStr, "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
    m_footerLabel->setStyle(true, true, 38, Color4B(182, 255, 0, 255), Color4B::BLACK, 2, Color4B::BLACK, Size(2, -2), 2);
    m_footerLabel->setSpacingConstraint(5.0f);
    m_footerLabel->setAnchorPoint({ 0.5, 0.5 });
    m_footerLabel->setPosition({ backgroundPlateSize.width / 2, FOOTER_HEIGHT / 2 });
    m_backgroundPlate->addChild(m_footerLabel);

    // Purchase button

    constexpr Size PURCHASE_BUTTON_SIZE = { 186, 70 };

    m_purchaseButton = ui::Button::create("UI/buttons/PurchaseButton.png", "UI/buttons/PurchaseButtonSelected.png", "UI/buttons/PurchaseButtonDisabled.png");
    m_purchaseButton->ignoreContentAdaptWithSize(false);
    m_purchaseButton->setContentSize(PURCHASE_BUTTON_SIZE);
    m_purchaseButton->setAnchorPoint({ 0.5f, 1.0f });
    m_purchaseButton->setPosition({ backgroundPlateSize.width / 2, 0 });
    m_purchaseButton->addTouchEventListener(CC_CALLBACK_2(PurchasableBox::onPurchaseButtonPressed, this));
    m_purchaseButton->setCascadeColorEnabled(true);
    m_backgroundPlate->addChild(m_purchaseButton);

    m_purchaseCostLabel = CompoundLabel::create("", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
    m_purchaseCostLabel->setStyle(true, true, 37, UNAFFORDABLE_RED, Color4B::BLACK, 2, Color4B::BLACK, Size(2, -2), 2);
    m_purchaseCostLabel->setSpacingConstraint(0.0f, 10.0f);
    m_purchaseCostLabel->setIconTexture("icons/GreenMatterSmall.png");
    m_purchaseCostLabel->setAnchorPoint({ 0.5, 0.5 });
    m_purchaseCostLabel->setPosition({ (PURCHASE_BUTTON_SIZE.width / 2) + 3, PURCHASE_BUTTON_SIZE.height / 2 });
    m_purchaseButton->addChild(m_purchaseCostLabel);

    m_lockedLabel = Label::createWithTTF("", "fonts/BreeSerif.ttf", 28, Size(140, 140), TextHAlignment::CENTER);
    m_lockedLabel->setContentSize(Size(140, 140));
    m_lockedLabel->setAnchorPoint({ 0.5, 0.5 });
    m_lockedLabel->setPosition({ backgroundPlateSize.width / 2, (backgroundPlateSize.height / 2) - 9 });
    CompoundLabel::setSingleStyle(m_lockedLabel, 28, Color4B::WHITE, Color4B::BLACK, 4);
    m_backgroundPlate->addChild(m_lockedLabel);

    setContentSize(m_backgroundPlate->getContentSize() + Size(0, m_purchaseButton->getContentSize().height));
    setCascadeColorEnabled(true);

    return true;
}

void PurchasableBox::update(float dt) {
    if (m_purchaseButtonHeld && m_purchaseButtonHoldable) {
        m_secondsSincePurchaseButtonTouched += dt;
        onPurchaseButtonHeld(m_secondsSincePurchaseButtonTouched, dt);
	}
}

void PurchasableBox::togglePurchaseable(bool purchasable) {
    if(m_isPurchasable == purchasable) return;

    m_isPurchasable = purchasable;

    if (m_hidePurchaseButtonWhenUnaffordable) m_purchaseButton->setVisible(m_isPurchasable);
    m_purchaseButton->setEnabled(m_isPurchasable);
    m_purchaseButton->setBright(m_isPurchasable);
}

void PurchasableBox::onPurchaseButtonPressed(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType evntType) {
    switch (evntType) {
    case ui::Widget::TouchEventType::BEGAN:
        m_secondsSincePurchaseButtonTouched = 0;
        m_purchaseButtonHeldProgress = 0.0f;
        m_purchaseButtonHeld = true;
        break;
    case ui::Widget::TouchEventType::ENDED:
        m_purchaseButtonHeld = false;
        tryPurchase();
        break;
    }
}

void PurchasableBox::onPurchaseButtonHeld(float heldTime, float dt) {
    if (!m_purchaseButton->isEnabled()) return;

    /// Purchase EXP every 5 times per second for the first 2 seconds,
    // then 20 times per second for the next 8 seconds
    // then 100 times per second

    // How much of the time was spent in the first purchase interval
    float firstIntervalTime = MIN(dt, MAX(0.0f, 2.0f - (heldTime - dt)));
    // How much of the time was spent in the second purchase interval
    float secondIntervalTime = MIN(dt, MAX(0.0f, MIN(heldTime - 2.0f, 10.0f - (heldTime - dt))));
    // How much of the time was spent in the third purchase interval
    float thirdIntervalTime = MIN(dt, MAX(0.0f, heldTime - 10.0f));

    // The number of times to purchase from the first interval
    m_purchaseButtonHeldProgress += firstIntervalTime / 0.2f;
    // The number of times to purchase from the second interval
    m_purchaseButtonHeldProgress += secondIntervalTime / 0.05f;
    // The number of times to purchase from the third interval
    m_purchaseButtonHeldProgress += thirdIntervalTime / 0.01f;

    while (m_purchaseButtonHeldProgress >= 1.0f) {
		m_purchaseButtonHeldProgress -= 1.0f;
		tryPurchase();
	}
}
