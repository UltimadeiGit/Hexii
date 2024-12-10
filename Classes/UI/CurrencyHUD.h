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

	// Plays an animation to hide the HUD. If animate is false, the HUD will be shown/hidden instantly instead.
	void toggleHidden(bool hidden, bool animate = true);

	CREATE_FUNC_WITH_CTOR_1(CurrencyHUD, CurrencyType);
private:
	// Animated translation of the HUD.
	void translate(float duration, cocos2d::Vec2 offset);

	void onSacrificeInitiated(cocos2d::EventCustom* evnt);
	void onSacrificeCancelled(cocos2d::EventCustom* evnt);
	void onSacrificeConfirmed(cocos2d::EventCustom* evnt);

	bool m_hidden = false;

	const CurrencyType m_currencyType;

	cocos2d::Sprite* m_background = nullptr;
	cocos2d::Sprite* m_currencyIcon = nullptr;

	cocos2d::Label* m_currencyAmount = nullptr;
};