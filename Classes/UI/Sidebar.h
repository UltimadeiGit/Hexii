#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>

#include "EventUtility.h"

class Sidebar : public cocos2d::Node 
{
public:
	bool init() override;

	CREATE_FUNC(Sidebar);

	EventUtility::ID eventID = EventUtility::generateID();
private:

	void onSacrificeButtonPressed(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType);

	cocos2d::Sprite* m_background;
	cocos2d::ui::Button* m_sacrificeButton;
	cocos2d::ui::Button* m_miscButtons[3];

};