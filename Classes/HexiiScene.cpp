#include "HexiiScene.h"

USING_NS_CC;

cocos2d::Scene* HexiiScene::createScene() {
	return HexiiScene::create();
}

bool HexiiScene::init() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_plane = HexPlane::createShared();
	m_plane->initWithSize(Hex::getTextureBaseRect().size.width / 2);

	m_plane->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	m_plane->set(Vec2(0, 0), Hex::createShared());
	m_plane->set(Vec2(1, 0), Hex::createShared());

	this->addChild(m_plane.get());

	return true;
}
