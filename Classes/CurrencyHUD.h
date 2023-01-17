#pragma once

#include <cocos2d.h>

class CurrencyHUD :
	public cocos2d::Node {
public:
	bool init();
	virtual void update(float dt) override;

	CREATE_FUNC(CurrencyHUD);
private:
	cocos2d::Sprite* m_background;
	cocos2d::Sprite* m_greenMatterIcon;

	cocos2d::Label* m_greenMatterAmount;
	// Amount gm is increasing per second
	cocos2d::Label* m_greenMatterIncreaseAmount;
	// Label with the text "/ sec"
	cocos2d::Label* m_perSecLabel;
};