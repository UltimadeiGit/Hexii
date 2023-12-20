#include "HexiiSacrificeHUD.h"
#include "GameplayCommon.h"
#include "SimpleShader.h"
#include "HexiiPlane.h"
#include "GameplayCommon.h"
#include "Resources.h"
#include "ColourSchemes.h"

bool HexiiSacrificeHUD::init()
{
    m_header = cocos2d::Sprite::create("HUD/HexiiSacrifice/Header.png");
    m_header->setAnchorPoint({ 0.5, 1.0 });
    //static auto shader = SimpleShader::create(SimpleShaderManager::getDentsProgram());
    //m_header->setProgramState(shader->getProgramState());
    this->addChild(m_header, 1);

    m_backgroundPlate = cocos2d::Sprite::create("HUD/HexiiSacrifice/BackgroundPlate.png");
    m_backgroundPlate->setAnchorPoint({ 0.5, 1.0 });
    this->addChild(m_backgroundPlate, 0);

    // Buttons

    // Distance from the center
    constexpr float BUTTON_DIST_X = 50;
    // Distance from the top
    constexpr float BUTTON_DIST_Y = 890;

    m_cancelButton = cocos2d::ui::Button::create("UI/buttons/CancelButton.png", "UI/buttons/CancelButtonSelected.png");
    m_cancelButton->setAnchorPoint({ 1.0, 0.0 });
    m_cancelButton->setPosition({ -BUTTON_DIST_X, -BUTTON_DIST_Y });
    m_cancelButton->addTouchEventListener(CC_CALLBACK_2(HexiiSacrificeHUD::onCancelButtonPressed, this));
    this->addChild(m_cancelButton, 1);

    m_confirmButton = cocos2d::ui::Button::create("UI/buttons/ConfirmButton.png", "UI/buttons/ConfirmButtonSelected.png");
    m_confirmButton->setAnchorPoint({ 0.0, 0.0 });
    m_confirmButton->setPosition({ BUTTON_DIST_X, -BUTTON_DIST_Y });
    m_confirmButton->addTouchEventListener(CC_CALLBACK_2(HexiiSacrificeHUD::onConfirmButtonPressed, this));
    m_confirmButton->setCascadeOpacityEnabled(true);
    this->addChild(m_confirmButton, 1);

    // Labels

    constexpr float RED_MATTER_GAIN_DIST_X = 50;
    constexpr float LABEL_TOP_Y = 200;
    constexpr float LABEL_SPACING_Y = 65;
    constexpr unsigned int FONT_SIZE = 50.0f;

    m_sacrifices[0] = CompoundLabel::create("Green Matter:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf", cocos2d::TextHAlignment::RIGHT);
    m_sacrifices[0]->setIconTexture("icons/GreenMatterSmall.png");

    for (int i = 1; i < STAT_LABEL_COUNT - 1; i++) {
        m_sacrifices[i] = CompoundLabel::create("Layer " + std::to_string(i - 1) + ":", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf", cocos2d::TextHAlignment::RIGHT);
        m_sacrifices[i]->setIconTexture("icons/EXPSmall.png");
    }

    m_sacrifices[STAT_LABEL_COUNT - 1] = CompoundLabel::create("Respec:", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf", cocos2d::TextHAlignment::RIGHT);
    m_sacrifices[STAT_LABEL_COUNT - 1]->setIconTexture("icons/RedMatterSmall.png");

    // Set properties common to all of the labels
    for (int i = 0; i < STAT_LABEL_COUNT; i++) {
        m_sacrifices[i]->setAnchorPoint({ 1.0, 1.0 });
        m_sacrifices[i]->setPosition({ -RED_MATTER_GAIN_DIST_X, -LABEL_TOP_Y - (LABEL_SPACING_Y * i) });
        m_sacrifices[i]->setStyle(true, true, FONT_SIZE, cocos2d::Color4B::WHITE, cocos2d::Color4B::BLACK, 3, cocos2d::Color4B::BLACK, { 2, -2 }, 2);
        m_sacrifices[i]->setSpacingConstraint(50.0f, 20.0f);
        m_sacrifices[i]->getVariablePart()->setDimensions(200.0f, m_sacrifices[i]->getContentSize().height );
        //m_sacrifices[i]->setWidthConstrant(400.0f, 5.0f, CompoundLabel::VariablePartFloat::RIGHT);
        //m_sacrifices[i]->setContentSize({ 600.0f, m_sacrifices[i]->getContentSize().height});
        m_sacrifices[i]->setCascadeOpacityEnabled(true);
        this->addChild(m_sacrifices[i], 1);

        m_redMatterGains[i] = CompoundLabel::create("->", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf", cocos2d::TextHAlignment::LEFT);
        m_redMatterGains[i]->setAnchorPoint({ 0.0, 1.0 });
        m_redMatterGains[i]->setPosition({ -10, -LABEL_TOP_Y - (LABEL_SPACING_Y * i) });
        m_redMatterGains[i]->setStyle(true, true, FONT_SIZE, cocos2d::Color4B::WHITE, cocos2d::Color4B::BLACK, 3, cocos2d::Color4B::BLACK, { 2, -2 }, 2);
        //m_redMatterGains[i]->setSpacingConstraint(RED_MATTER_GAIN_DIST_X, 5.0f);
        m_redMatterGains[i]->setWidthConstrant(325, 5.0f, CompoundLabel::VariablePartFloat::RIGHT);
        m_redMatterGains[i]->setIconTexture("icons/RedMatterSmall.png");
        m_redMatterGains[i]->setCascadeOpacityEnabled(true);
        this->addChild(m_redMatterGains[i], 1);
    }

    m_redMatterGainLabel = CompoundLabel::create("+ ", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
    m_redMatterGainLabel->setAnchorPoint({ 0.5, 0.0 });
    m_redMatterGainLabel->setPosition({ m_confirmButton->getContentSize().width / 2, 25});
    m_redMatterGainLabel->setStyle(true, true, 60, COMMON_RED, cocos2d::Color4B::BLACK, 4, cocos2d::Color4B::BLACK, { 0, 0 }, 6);
    m_redMatterGainLabel->setIconTexture("icons/RedMatterSmall.png");
    m_redMatterGainLabel->setSpacingConstraint(5.0f);
    m_redMatterGainLabel->setCascadeOpacityEnabled(true);
    m_confirmButton->addChild(m_redMatterGainLabel, 1);

    // Set the content size (width of the header, height of the background plate)
    setContentSize({ m_header->getContentSize().width, m_backgroundPlate->getContentSize().height });

    return true;
}

void HexiiSacrificeHUD::update(float dt) {
    // Update the labels every `UPDATE_INTERVAL` frames
    constexpr uint UPDATE_INTERVAL = 10;

    if (cocos2d::Director::getInstance()->getTotalFrames() % UPDATE_INTERVAL == 0) updateValues();    
}

void HexiiSacrificeHUD::updateValues() {
    // Set the green matter sacrifice & gain labels
    BigReal greenMatter = Resources::getInstance()->getGreenMatter();
    BigReal redMatterGainFromGreenMatter = GameplayCommon::Sacrifice::greenMatterToRedMatter(greenMatter);

    getGreenMatterSacrificeLabel()->setVariablePartString(formatBigReal(greenMatter));
    getGreenMatterRedMatterGainLabel()->setVariablePartString(
        formatBigReal(redMatterGainFromGreenMatter)
    );

    // Sum all of the EXP from the hexii in each layer to set the layer sacrifice and gain labels
    // also track red matter investment while at it (for respec)
    HexiiPlane* plane = HexiiPlane::getInstance();
    BigReal redMatterGainFromEXP = 0;
    BigReal redMatterGainFromRespec = 0;
    for (int i = 0; i < GameplayCommon::MAX_LAYER; i++) {
        auto hexii = plane->getHexiiInlayer(i);

        // Loop over all the hexii in this layer
        BigReal totalEXP = 0;
        for (auto& hexiiPosPair : hexii) {
            totalEXP += hexiiPosPair.hex->getEXP();
            redMatterGainFromRespec += hexiiPosPair.hex->getRedMatterInvested();
        }

        BigInt redMatterGainFromLayerEXP = GameplayCommon::Sacrifice::expToRedMatter(totalEXP, i);

        // Set the layer sacrifice and gain labels
        getLayerSacrificeLabel(i)->setVariablePartString(formatBigReal(totalEXP));
        getLayerRedMatterGainLabel(i)->setVariablePartString(formatBigReal(redMatterGainFromLayerEXP));
    
        redMatterGainFromEXP += redMatterGainFromLayerEXP;
    }

    // Set the respec labels
    if (redMatterGainFromRespec == 0) { // Hide respec if it's not applicable
        getRespecSacrificeLabel()->setVisible(false);
        getRespecRedMatterGainLabel()->setVisible(false);
    }
    else {
        std::string respecRedMatterStr = formatBigReal(redMatterGainFromRespec);

        getRespecSacrificeLabel()->setVisible(true);
        getRespecSacrificeLabel()->setVariablePartString(respecRedMatterStr);

        // Respec sacrifice and gain are, by definition, equal
        getRespecRedMatterGainLabel()->setVisible(true);
        getRespecRedMatterGainLabel()->setVariablePartString(respecRedMatterStr);
    }

    m_redMatterGain = redMatterGainFromGreenMatter + redMatterGainFromEXP + redMatterGainFromRespec;

    m_redMatterGainLabel->setVariablePartString(formatBigReal(m_redMatterGain));
}

void HexiiSacrificeHUD::onCancelButtonPressed(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
    // We only care about the button being released
    if (evntType != cocos2d::ui::Widget::TouchEventType::ENDED) return;

    // Dispatch the event
    EventUtility::dispatchEvent(GameplayCommon::GameEvent::EVENT_SACRIFICE_CANCELLED, eventID);
}

void HexiiSacrificeHUD::onConfirmButtonPressed(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
    // We only care about the button being released
	if (evntType != cocos2d::ui::Widget::TouchEventType::ENDED) return;
	
    // Dispatch the event
	EventUtility::dispatchEvent<GameplayCommon::GameEvent::EventSacrificeConfirmedData>
        (GameplayCommon::GameEvent::EVENT_SACRIFICE_CONFIRMED, eventID, {m_redMatterGain});

    // Add the red matter
    Resources::getInstance()->addRedMatter(m_redMatterGain);
}
