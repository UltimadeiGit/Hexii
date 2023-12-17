#include "HexiiScene.h"
#include "Maths.h"
#include "SimpleShader.h"
#include "Resources.h"
#include "JSON.hpp"
#include "SaveData.h"
#include "Progression.h"

USING_NS_CC;
using namespace nlohmann;

HexiiScene* HexiiScene::m_instance = nullptr;

cocos2d::Scene* HexiiScene::createScene() {
	return HexiiScene::create();
}

bool HexiiScene::init() {
	// Set the instance pointer
	HexiiScene::m_instance = this;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/// Init the camera

	m_hexiiCamera = Camera::create();
	m_hexiiCamera->setPosition({ 0, 0 });
	//m_hexiiCamera->setScale(0.5f, 0.5f);
	m_hexiiCamera->setDepth(-1);
	m_hexiiCamera->setCameraFlag(CameraFlag::USER1);
	this->addChild(m_hexiiCamera);

	/// Loading
	auto saveData = SaveData::getInstance();

	// Load progression

	// dbg
	EventUtility::addGlobalEventListener(ProgressionEvent::EVENT_PROGRESSION_TO_ACHIEVED, this, &HexiiScene::onProgression);
	saveData->tryLoad(SaveData::PROGRESSION);

	// Try to load the plane and settings system (they are independent)
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
	//m_plane->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	//m_debugLabel = Label::createWithTTF("TMP", "fonts/arial.ttf", 20.0f);
	//m_debugLabel->setTextColor(Color4B(128, 128, 128, 255));
	// m_debugLabel->enableGlow(Color4B(255, 0, 0, 255));
	//m_debugLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, 100));

	//for (int i = 0; i < 10; i++) {
	//	m_debugNodes[i] = DrawNode::create(3.0f);
	//	this->addChild(m_debugNodes[i]);
	//}

	/// UI & HUD

	// Camera

	m_backgroundCamera = Camera::create();
	m_backgroundCamera->setCameraFlag(CameraFlag::USER2);
	m_backgroundCamera->setPosition({0, 0 });
	m_backgroundCamera->setDepth(-2);

	//this->addChild(m_backgroundCamera);

	// Background 
	/*
	Size bgDesiredSize = Director::getInstance()->getTextureCache()->addImage("icons/TileableBackground.png")->getContentSize();

	auto bgTex = Director::getInstance()->getTextureCache()->addImage("icons/TileableBackgroundPOT.png");
	// Size of the power of two texture (its the same as the original texture but squashed to a power of two width and height)
	const Size bgPOTTexSize = bgTex->getContentSize();

	// Set the texture wrap mode to repeat
	bgTex->setTexParameters(Texture2D::TexParams(cocos2d::backend::SamplerFilter::LINEAR,
		cocos2d::backend::SamplerFilter::LINEAR,
		cocos2d::backend::SamplerAddressMode::REPEAT,
		cocos2d::backend::SamplerAddressMode::REPEAT));

	float scaleX = bgDesiredSize.width / bgPOTTexSize.width;
	float scaleY = bgDesiredSize.height / bgPOTTexSize.height;

	auto background = Sprite::createWithTexture(bgTex);
	

	background->setAnchorPoint(Vec2(0, 0));
	background->setStretchEnabled(false);
	background->setTextureRect(Rect(Vec2::ZERO, Size(visibleSize.width, visibleSize.height)));
	background->setScale(scaleX, scaleY);

	this->addChild(background, -10);
	background->setCameraMask((unsigned short)m_backgroundCamera->getCameraFlag());
	*/

	// Dock

	m_dock = Dock::create();
	m_dock->setAnchorPoint(Vec2(0.5, 0.0));
	m_dock->setPosition(origin + Vec2(visibleSize.width / 2, 0));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_dock->setScale(1.2f);
#endif

	// Currency HUD

	m_currencyHUD = CurrencyHUD::create(CurrencyHUD::CurrencyType::GREEN_MATTER);
	//m_currencyHUD->setPosition(Vec2(origin.x, (visibleSize.height * 0.85) + origin.y));
	m_currencyHUD->setAnchorPoint({ 0.5, 0});
	m_currencyHUD->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height + origin.y));

	scheduleUpdate();

	this->addChild(HexiiPlane::getInstance(), 0);
	this->addChild(m_dock, 1);
	this->addChild(m_currencyHUD, 1);

	m_dock->switchTab(0);
	m_plane->getHexAtPos({ 0, 0 })->focus();

	//// SHADER TESTING
	SimpleShaderPtr testShader = SimpleShader::create(SimpleShader::createShaderProgramWithFragmentShader("shaders/dents.frag"));

	// Setup handler for the W key
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this, testShader](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_W) {
			for (auto& p : HexiiPlane::getInstance()->getHexiiInlayer(2)) {
				p.hex->setShaderEffect(SimpleShader::create(SimpleShader::createShaderProgramWithFragmentShader("shaders/dents.frag")));
			}
			HexiiPlane::getInstance()->getHexAtPos({ 0, 0 })->setShaderEffect(testShader);
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	Progression::getInstance()->init();

	return true;
}

void HexiiScene::update(float dt) {
	SimpleShaderManager::getInstance()->updateShaderTime();

	//m_debugLabel->setString("(" + std::to_string(a.x) + ", " + std::to_string(a.y) + ")");

	m_plane->update(dt);
	m_dock->update(dt);
	m_currencyHUD->update(dt);
}

void HexiiScene::onProgression(cocos2d::EventCustom* evnt) {
	auto dat = EventUtility::getEventData<ProgressionEvent::EventProgressionStateChange>(evnt);
	printf("Progression event achieved!\n");
}
