#include "HexiiScene.h"
#include "Maths.h"
#include "SimpleShader.h"
#include "Resources.h"
#include "JSON.hpp"
#include "SaveData.h"
#include "Progression.h"
#include "GameplayCommon.h"

USING_NS_CC;
using namespace nlohmann;

HexiiScene* HexiiScene::m_instance = nullptr;

cocos2d::Scene* HexiiScene::createScene() {
	return HexiiScene::create();
}

bool HexiiScene::init() {
	// Set the instance pointer
	HexiiScene::m_instance = this;

	useVisibleSizeAndOrigin;

	// Camera
	m_hexiiCamera = Camera::create();
	m_hexiiCamera->setPosition({ 0, 0 });
	//m_hexiiCamera->setScale(0.5f, 0.5f);
	m_hexiiCamera->setDepth(-1);
	m_hexiiCamera->setCameraFlag(CameraFlag::USER1);
	this->addChild(m_hexiiCamera);

	/// Loading

	auto saveData = SaveData::getInstance();

	// Progression
	// dbg
	EventUtility::addGlobalEventListener(ProgressionEvent::EVENT_PROGRESSION_TO_ACHIEVED, this, &HexiiScene::onProgression);
	saveData->tryLoad(SaveData::PROGRESSION);

	// Plane
	bool planeLoaded = saveData->tryLoad(SaveData::PLANE);

	if (planeLoaded) {
		m_plane = HexiiPlane::getInstance();

		// Loaded plane succesfully, so try and load the other systems
		saveData->tryLoad(SaveData::RESOURCES);
	}
	else {
		// Failed to load the plane, so create a new one
		m_plane = HexiiPlane::create(270 + 10);
		// Make the L0 hex
		m_plane->placeHexAtPos(Vec2(0, 0));
	}

	m_plane->setPosition(0, 100);
	this->addChild(m_plane, 0);

	// Settings
	//saveData->tryLoad(SaveData::SETTINGS);

	/// UI & HUD

	// Dock
	m_dock = Dock::create();
	m_dock->setAnchorPoint(Vec2(0.5, 0.0));
	m_dock->setPosition(origin + Vec2(visibleSize.width / 2, 0));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_dock->setScale(1.2f);
#else
	m_dock->setScale(0.9f);
#endif
	this->addChild(m_dock, 1);

	// Sidebar
	m_sidebar = Sidebar::create();
	m_sidebar->setAnchorPoint({ 1, 0 });
	m_sidebar->setPosition({ origin.x, origin.y + visibleSize.height - 30 });
	this->addChild(m_sidebar, 1);

	/// Currency HUD

	constexpr float HUD_SCALE = 1.0f;
	const Vec2 HUD_POS = { origin.x + visibleSize.width / 2, origin.y + visibleSize.height };

	// Green Matter HUD
	m_greenMatterHUD = CurrencyHUD::create(CurrencyHUD::CurrencyType::GREEN_MATTER);
	m_greenMatterHUD->setScale(HUD_SCALE);
	m_greenMatterHUD->setAnchorPoint({ 0.5, 0});
	m_greenMatterHUD->setPosition(HUD_POS + m_greenMatterHUD->getPosition());
	this->addChild(m_greenMatterHUD, 1);

	// Red Matter HUD
	m_redMatterHUD = CurrencyHUD::create(CurrencyHUD::CurrencyType::RED_MATTER);
	m_redMatterHUD->setScale(HUD_SCALE);
	m_redMatterHUD->setAnchorPoint({ 0.5, 0 });
	m_redMatterHUD->setPosition(HUD_POS + m_redMatterHUD->getPosition());
	this->addChild(m_redMatterHUD, 1);

	//// SHADER TESTING
	/*SimpleShaderPtr testShader = SimpleShader::create(SimpleShader::createShaderProgramWithFragmentShader("shaders/dents.frag"));

	// Setup handler for the W key
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this, testShader](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_W) {
			for (auto& p : HexiiPlane::getInstance()->getHexiiInlayer(2)) {
				p.hex->setShaderEffect(SimpleShader::create(SimpleShader::createShaderProgramWithFragmentShader("shaders/dents.frag")));
			}
			HexiiPlane::getInstance()->getHexiiAtPos({ 0, 0 })->setShaderEffect(testShader);
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/

	// Listeners

	EventUtility::addGlobalEventListener(GameplayCommon::GameEvent::EVENT_SACRIFICE_INITIATED, this, &HexiiScene::onSacrificeInitiated);
	EventUtility::addGlobalEventListener(GameplayCommon::GameEvent::EVENT_SACRIFICE_CANCELLED, this, &HexiiScene::onSacrificeCancelled);
	EventUtility::addGlobalEventListener(GameplayCommon::GameEvent::EVENT_SACRIFICE_CONFIRMED, this, &HexiiScene::onSacrificeConfirmed);

	/// Final initialization

	// Finish initializing the progression system (it requires the UI system to be loaded)
	Progression::getInstance()->init();
	this->scheduleUpdate();

	// UI QoL
	m_dock->switchTab(0);
	Hexii* l0Hex = m_plane->getHexiiAtPos({ 0, 0 });
	if (l0Hex->getActive()) l0Hex->focus();
	else m_dock->getHexiiInfoTab()->setFocusHexii(nullptr);

	return true;
}

void HexiiScene::update(float dt) {
	SimpleShaderManager::getInstance()->updateShaderTime();

	//m_debugLabel->setString("(" + std::to_string(a.x) + ", " + std::to_string(a.y) + ")");

	m_plane->update(dt);
	m_dock->update(dt);
	m_greenMatterHUD->update(dt);
	if(m_redMatterHUD->isVisible()) m_redMatterHUD->update(dt);

	// Update the sacrifice HUD if it exists
	if (m_sacrificeHUD) m_sacrificeHUD->update(dt);
}

void HexiiScene::onSacrificeInitiated(cocos2d::EventCustom* evnt) {
	// Possible for the user to spam initiate
	if (m_sacrificeHUD != nullptr) return;

	useVisibleSizeAndOrigin;

	// Create the sacrifice HUD
	m_sacrificeHUD = HexiiSacrificeHUD::create();
	m_sacrificeHUD->setCascadeOpacityEnabled(true);
	m_sacrificeHUD->setOpacity(0);
	m_sacrificeHUD->runAction(EaseQuadraticActionIn::create(FadeTo::create(0.8f, 255)));
	m_sacrificeHUD->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height });
	m_sacrificeHUD->updateValues();
	this->addChild(m_sacrificeHUD, 2);
}

void HexiiScene::onSacrificeCancelled(cocos2d::EventCustom* evnt) {
	closeSacrificeHUD();
}

void HexiiScene::onSacrificeConfirmed(cocos2d::EventCustom* evnt) {
	closeSacrificeHUD();

	m_dock->getHexiiInfoTab()->setFocusHexii(nullptr);
}

void HexiiScene::closeSacrificeHUD() {
	// Possible for the user to spam cancel
	if (m_sacrificeHUD == nullptr) return;

	// Remove the sacrifice HUD
	HexiiSacrificeHUD* sacrificeHUD = m_sacrificeHUD;
	m_sacrificeHUD->runAction(Sequence::createWithTwoActions(
		EaseIn::create(FadeTo::create(0.8f, 0), 0.5f),
		CallFunc::create([this, sacrificeHUD]() {
			this->removeChild(sacrificeHUD);
		}
	)));
	m_sacrificeHUD = nullptr;
}

void HexiiScene::onProgression(cocos2d::EventCustom* evnt) {
	auto dat = EventUtility::getEventData<ProgressionEvent::EventProgressionStateChange>(evnt);
	printf("Progression event achieved!\n");
}
