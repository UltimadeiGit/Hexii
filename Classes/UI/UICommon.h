#pragma once

#include <cocos2d.h>

namespace UICommon {
	constexpr float PURCHASABLE_BOX_SPACING = 52;
	constexpr float PURCHASABLE_BOX_WIDTH = 223;
	constexpr float PURCHASABLE_BOX_PLATE_HEIGHT = 243;
	constexpr float PURCHASABLE_BOX_NAMEPLATE_HEIGHT = 38;
	constexpr float PURCHASABLE_BOX_UPGRADE_BUTTON_HEIGHT = 70;
	constexpr float PURCHASABLE_BOX_HEIGHT = PURCHASABLE_BOX_PLATE_HEIGHT + PURCHASABLE_BOX_UPGRADE_BUTTON_HEIGHT;
	constexpr cocos2d::Size SCROLLVIEW_CONTENT_SIZE = { 842, PURCHASABLE_BOX_HEIGHT + 10 };

	constexpr cocos2d::Size SIDEBAR_SIZE = { 1816, 455 };
}