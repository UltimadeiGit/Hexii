#include "HexiiScene.h"
#include "Maths.h"
#include "SimpleShader.h"`
#include "Currencies.h"

USING_NS_CC;

cocos2d::Scene* HexiiScene::createScene() {
	return HexiiScene::create();
}

bool HexiiScene::init() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_plane = HexPlane::create(270 + 10);
	m_plane->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	m_debugLabel = Label::createWithTTF("TMP", "fonts/arial.ttf", 20.0f);
	m_debugLabel->setTextColor(Color4B(128, 128, 128, 255));
	// m_debugLabel->enableGlow(Color4B(255, 0, 0, 255));
	m_debugLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, 100));

	for (int i = 0; i < 10; i++) {
		m_debugNodes[i] = DrawNode::create(3.0f);
		this->addChild(m_debugNodes[i]);
	}

	m_sidebar = Sidebar::create();
	m_sidebar->setPosition(Vec2(0, 35));

	m_currencyHUD = CurrencyHUD::create();
	m_currencyHUD->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height + origin.y));

	scheduleUpdate();

	this->addChild(m_plane);
	this->addChild(m_sidebar);
	this->addChild(m_currencyHUD);
	this->addChild(m_debugLabel);

	return true;
}

void HexiiScene::update(float dt) {
	SimpleShaderManager::getInstance()->updateShaderTime();

	//m_debugLabel->setString("(" + std::to_string(a.x) + ", " + std::to_string(a.y) + ")");

	m_plane->update(dt);
	m_sidebar->update(dt);
	m_currencyHUD->update(dt);

	getDefaultCamera();

	getDefaultCamera()->setPosition(getDefaultCamera()->getPosition() + dt * Vec2(10.0, 10.0));
}