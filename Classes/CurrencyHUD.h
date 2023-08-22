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
	cocos2d::Sprite* m_nectarIcon;

	cocos2d::Label* m_nectarAmount;
	// Amount gm is increasing per second
	cocos2d::Label* m_nectarIncreaseAmount;
	// Label with the text "/ sec"
	cocos2d::Label* m_perSecLabel;
};