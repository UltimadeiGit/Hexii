#pragma once

#include <cocos2d.h>

#define useVisibleSize cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize()
#define useVisibleOrigin cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin()
#define useVisibleSizeAndOrigin useVisibleSize; useVisibleOrigin

namespace CocosUtility {
	extern cocos2d::Vec2 getCameraMousePos(cocos2d::Camera* const& camera, const cocos2d::Vec2& mousePos);
}
