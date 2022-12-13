#include "HexiiScene.h"
#include "Maths.h"
#include "SimpleShader.h"

USING_NS_CC;

cocos2d::Scene* HexiiScene::createScene() {
	return HexiiScene::create();
}

bool HexiiScene::init() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_plane = HexPlane::create();
	m_plane->initWithSize(270 + 10);

	m_plane->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	Hex* l0Hex = m_plane->set(Vec2(0, 0));
	l0Hex->yieldFunction = CC_CALLBACK_1(HexiiScene::onHexYield, this);

	this->addChild(m_plane);

	this->scheduleUpdate();

	m_debugLabel = Label::createWithTTF("TMP", "fonts/arial.ttf", 20.0f);
	m_debugLabel->setTextColor(Color4B(128, 128, 128, 255));
	// m_debugLabel->enableGlow(Color4B(255, 0, 0, 255));
	m_debugLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, 100));
	this->addChild(this->m_debugLabel);

	
	

#ifdef CC_PLATFORM_MOBILE
	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(HexiiScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HexiiScene::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(HexiiScene::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HexiiScene::onTouchMoved, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
#endif

#ifdef CC_PLATFORM_PC
	auto mouseListener = EventListenerMouse::create();

	mouseListener->onMouseDown = CC_CALLBACK_1(HexiiScene::onMousePressed, this);
	mouseListener->onMouseUp = CC_CALLBACK_1(HexiiScene::onMouseUp, this);
	mouseListener->onMouseMove = CC_CALLBACK_1(HexiiScene::onMouseMoved, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
#endif

	for (int i = 0; i < 10; i++) {
		m_debugNodes[i] = DrawNode::create(3.0f);
		this->addChild(m_debugNodes[i]);
	}

	m_infoBox = InfoBox::create();
	m_infoBox->setAnchorPoint(Vec2(0, 0));
	m_infoBox->setPosition(Vec2(0, 35));

	this->addChild(m_infoBox);

	return true;
}

#ifdef CC_PLATFORM_PC

void HexiiScene::onMousePressed(cocos2d::EventMouse* mouse) { 
	EventMouse::MouseButton pressType = mouse->getMouseButton();

	if (pressType == EventMouse::MouseButton::BUTTON_LEFT) {
		if (m_mouseOverHex == nullptr) return;

		if (m_mouseOverHex->getActive()) m_mouseOverHex->onTouchBegan();
		// We clicked on an inactive target, but the hex does exist meaning it may be purchasable
		else tryPurchaseHex(m_mouseOverHex);
	}

	// Right clicks
	if (mouse->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
		if (m_mouseOverHex) m_infoBox->setFocus(m_mouseOverHex);
	}
}

void HexiiScene::onMouseUp(cocos2d::EventMouse* mouse) {
	if (m_mouseOverHex) m_mouseOverHex->onTouchEnded();
}

void HexiiScene::onMouseMoved(cocos2d::EventMouse* mouse) {
	m_debugLabel->setString("(" + std::to_string(mouse->getLocationInView().x) + ", " + std::to_string(mouse->getLocationInView().y) + ")");
	// printf("(%d, %d)\n", (int)mouse->getLocationInView().x, (int)mouse->getLocationInView().y);
	// TODO: This line is a bit long. Some of the functionality belongs in the HexPlane
	m_mouseOverHex = m_plane->get(m_plane->round(m_plane->hexPositionOf(mouse->getLocationInView() - m_plane->getPosition())));
}

#endif

void HexiiScene::update(float dt) {
	SimpleShaderManager::getInstance()->updateShaderTime();

	m_plane->update(dt);
	m_infoBox->update(dt);

	// Used to update certain elements only every few frames
	static unsigned int frameCount = 0;
	frameCount++;

	if (frameCount % 3 == 0) {
		m_infoBox->updateGreenMatterAmount(m_greenMatter);
		m_infoBox->updateGreenMatterIncreaseAmount(m_greenMatterPerSecond);
	}
}

void HexiiScene::onHexYield(Hex* hex) {
	// Layer 0 produces green matter. Outer layers produce EXP for adjacent hexii of lower layers

	int layer = hex->getLayer();
	BigInt yield = hex->getYield();
	if (layer == 0) return updateGreenMatter(yield, 0);
	
	auto neighbors = m_plane->neighborsOf(m_plane->hexPositionOf(hex->getPosition()), true);
	for (auto& neighbor : neighbors) if (neighbor.hex->getLayer() < layer) neighbor.hex->addEXP(yield.to_int());
}

void HexiiScene::onHexStartLevelUp(Hex* hex) {
	// A hex level up may trigger a change in its green matter per second. So, this function will erase its current \
	contributions, then the FinishLevelUp function will add the new value back in
	updateGreenMatter(0, -hex->getYieldPerSecond());
}

void HexiiScene::onHexFinishLevelUp(Hex* hex) {
	updateGreenMatter(0, hex->getYieldPerSecond());
}

void HexiiScene::updateGreenMatter(BigInt extraGreenMatter, BigInt extraGreenMatterPerSecond) {
	m_greenMatter += extraGreenMatter;
	m_greenMatterPerSecond += extraGreenMatterPerSecond;
}

void HexiiScene::tryPurchaseHex(Hex* target) {
	// Check if it's affordable
	unsigned int layer = target->getLayer();

	BigInt cost = 6;
	switch (layer) {
	case 0:
		break;
	case 1:
		cost = 100; //100 * (cost ^ (m_hexiiCountPerLayer[1] + 1));
		break;
	default:
		cost = "100";
		break;
	}
	
	// Not affordable
	if (m_greenMatter < cost) return;

	// Affordable; buy it!

	target->setActive(true);
	m_hexiiCountPerLayer[layer]++;
	// Hacky thing before I change the way the per second display works
	updateGreenMatter(-cost, target->getLayer() == 0 ? target->getYieldPerSecond() : 0);

	// Any neighboring positions that don't yet have an inactive hex need to have one so they can be purchased

	auto neighbors = m_plane->neighborsOf(m_plane->hexPositionOf(target->getPosition()), false);
	for (auto& neighbor : neighbors) {
		// No need to set a hex if one is already in place or if it's beyond the maximum layer
		if (neighbor.hex != nullptr || m_plane->layerOf(neighbor.pos) > 3) continue;
		
		Hex* newHex = m_plane->set(neighbor.pos);
		newHex->yieldFunction = CC_CALLBACK_1(HexiiScene::onHexYield, this);
	}
}

Vec2 HexiiScene::correctInvertedYVec(const Vec2& vec) const {
	return Vec2(vec.x, Director::getInstance()->getVisibleSize().height - vec.y);
}

#ifdef CC_PLATFORM_MOBILE

// TODO: Implement. The functions below are outdated

bool HexiiScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev) {
	m_debugLabel->setString("Touched!!");

	// Touch location in view uses inverted y for some inexplicable reason, so this corrects it
	// Vec2 touchLocation = correctInvertedYVec(touch->getLocationInView());

	return true;
}

void HexiiScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* ev) {
	Vec2 touchLocation = correctInvertedYVec(touch->getLocationInView());

	// The hex the mouse is over
	Hex* target = m_plane->get(m_plane->round(m_plane->hexPositionOf(touchLocation - m_plane->getPosition())));

	if (target) target->onTouchEnded(touch, ev);
}

#endif