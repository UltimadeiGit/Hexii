#pragma once

#include <cocos2d.h>

#include "Hexii.h"

class HexiiTab :
	public cocos2d::Layer {
private:
	HexiiTab(const HexiiTab&) = delete;
	HexiiTab(HexiiTab&&) = delete;

public:

	virtual void update(float dt) override {}
	virtual void setFocus(Hexii* focus) { m_focusHexii = focus; }

protected:
	HexiiTab() {}

	Hexii* m_focusHexii = nullptr;
};