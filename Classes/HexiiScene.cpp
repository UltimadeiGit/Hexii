#include "HexiiScene.h"
#include "Maths.h"

USING_NS_CC;

cocos2d::Scene* HexiiScene::createScene() {
	return HexiiScene::create();
}

bool HexiiScene::init() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_plane = HexPlane::create();
	m_plane->initWithSize(Hex::getTextureBaseRect().size.height);

	m_plane->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	m_plane->set(Vec2(0, 0), Hex::create());
	m_plane->set(Vec2(1, 0), Hex::create());
	m_plane->set(Vec2(-1, 0), Hex::create());
	m_plane->set(Vec2(1, -1), Hex::create());
	m_plane->set(Vec2(-1, 1), Hex::create());
	m_plane->set(Vec2(0, 1), Hex::create());
	m_plane->set(Vec2(0, -1), Hex::create());
	m_plane->set(Vec2(0, 2), Hex::create());
	m_plane->set(Vec2(1, 1), Hex::create());
	m_plane->set(Vec2(2, 0), Hex::create());
	m_plane->set(Vec2(2, -1), Hex::create());

	this->addChild(m_plane);

	this->scheduleUpdate();

	m_debugLabel = Label::createWithTTF("TMP", "fonts/arial.ttf", 20.0f);
	m_debugLabel->setTextColor(Color4B(128, 128, 128, 255));
	// m_debugLabel->enableGlow(Color4B(255, 0, 0, 255));
	m_debugLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, 100));
	this->addChild(this->m_debugLabel);

	auto touchListener = EventListenerTouchOneByOne::create();
	auto mouseListener = EventListenerMouse::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(HexiiScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HexiiScene::onTouchMoved, this);

	mouseListener->onMouseMove = CC_CALLBACK_1(HexiiScene::onMouseMoved, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	for (int i = 0; i < 10; i++) {
		m_debugNodes[i] = DrawNode::create(3.0f);
		this->addChild(m_debugNodes[i]);
	}

	return true;
}

bool HexiiScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev) {
	m_debugLabel->setString("Touched!!");

	Hex* t = m_plane->get(m_plane->round(m_plane->hexPositionOf(m_mousePos - m_plane->getPosition())));

	return false;
}

void HexiiScene::onMouseMoved(cocos2d::EventMouse* mouse) {
	m_mousePos = mouse->getLocationInView();
	const Vec2 planePos = m_plane->getPosition();
	
	Vec2 a = m_mousePos - planePos;
	Vec2 tmp = m_plane->hexPositionOf(a);
	Vec2 tmp2 = m_plane->localPositionOf(tmp);
	if (tmp2 != a) {
		bool brk = true;
	}

	static Hex* mouseover = nullptr;
	if (mouseover) mouseover->setActive(false);
	mouseover = m_plane->get(m_plane->round(m_plane->hexPositionOf(m_mousePos - planePos)));
	if(mouseover) mouseover->setActive(true);
}

void HexiiScene::update(float dt) {
	static float time = 0;
	time += dt;// *0.5f;

	

	/*
	

	HexPtr l0Hex = m_plane->get(Vec2(0, 0));
	l0Hex->setColor(Color3B(abs(sin(time)) * 255, abs(cos(time)) * 255, abs(cos(time * 1.1f) * 255)));
	*/
}
