#pragma once

#include <cocos2d.h>

class CurrencyHUD :
	public cocos2d::Node {
public:
	enum class CurrencyType {
		GREEN_MATTER,
		RED_MATTER
	};

	bool init();
	virtual void update(float dt) override;

	std::string getCurrencyName() const;

	// Plays an animation to hide the HUD
	void toggleHidden(bool hidden);

	CREATE_FUNC_WITH_CTOR_1(CurrencyHUD, CurrencyType);
private:
	void onSacrificeInitiated(cocos2d::EventCustom* evnt);
	void onSacrificeCancelled(cocos2d::EventCustom* evnt);
	void onSacrificeConfirmed(cocos2d::EventCustom* evnt);

	bool m_hidden = false;

	const CurrencyType m_currencyType;

	cocos2d::Sprite* m_background = nullptr;
	cocos2d::Sprite* m_currencyIcon = nullptr;

	cocos2d::Label* m_currencyAmount = nullptr;
};