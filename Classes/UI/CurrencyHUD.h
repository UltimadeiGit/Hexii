#pragma once

#include <cocos2d.h>

class CurrencyHUD :
	public cocos2d::Node {
public:
	enum class CurrencyType {
		GREEN_MATTER
	};

	bool init();
	virtual void update(float dt) override;

	std::string getCurrencyName() const;

	CREATE_FUNC_WITH_CTOR_1(CurrencyHUD, CurrencyType);
private:
	const CurrencyType m_currencyType;

	cocos2d::Sprite* m_background;
	cocos2d::Sprite* m_currencyIcon;

	cocos2d::Label* m_currencyAmount;
};