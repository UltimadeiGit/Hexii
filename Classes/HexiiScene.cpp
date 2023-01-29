#include "HexiiScene.h"
#include "Maths.h"
#include "SimpleShader.h"
#include "Resources.h"
#include "JSON.hpp"
#include "SaveData.h"

USING_NS_CC;
using namespace nlohmann;

cocos2d::Scene* HexiiScene::createScene() {
	return HexiiScene::create();
}

bool HexiiScene::init() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/// Loading
	auto saveData = SaveData::getInstance();

	// Try to load the plane and settings system (they are independent)
	bool planeLoaded = saveData->tryLoad(SaveData::PLANE | SaveData::SETTINGS)& SaveData::PLANE;

	if (planeLoaded) {
		m_plane = HexPlane::getInstance();

		// Loaded plane succesfully, so try and load the other systems
		saveData->tryLoad(SaveData::RESOURCES);
	}
	else {
		// Failed to load the plane, so create a new one
		m_plane = HexPlane::create(270 + 10);
		// Make the L0 hex
		m_plane->placeHexAtPos(Vec2(0, 0));
	}

	m_plane->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	//m_debugLabel = Label::createWithTTF("TMP", "fonts/arial.ttf", 20.0f);
	//m_debugLabel->setTextColor(Color4B(128, 128, 128, 255));
	// m_debugLabel->enableGlow(Color4B(255, 0, 0, 255));
	//m_debugLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, 100));

	//for (int i = 0; i < 10; i++) {
	//	m_debugNodes[i] = DrawNode::create(3.0f);
	//	this->addChild(m_debugNodes[i]);
	//}

	m_sidebar = Sidebar::create();
	m_sidebar->setAnchorPoint(Vec2(0.0, 0.5));
	m_sidebar->setPosition(Vec2(0, visibleSize.height / 2 + origin.y));

	m_currencyHUD = CurrencyHUD::create();
	m_currencyHUD->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height + origin.y));

	scheduleUpdate();

	this->addChild(HexPlane::getInstance());
	this->addChild(m_sidebar);
	this->addChild(m_currencyHUD);

	return true;
}

void HexiiScene::update(float dt) {
	SimpleShaderManager::getInstance()->updateShaderTime();

	//m_debugLabel->setString("(" + std::to_string(a.x) + ", " + std::to_string(a.y) + ")");

	m_plane->update(dt);
	m_sidebar->update(dt);
	m_currencyHUD->update(dt);
}