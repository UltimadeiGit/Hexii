#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIRichText.h>

class Tooltip : public cocos2d::Node 
{
private:
	bool init() override;

public:
	CREATE_FUNC(Tooltip);

	inline void setContentAnchor(const cocos2d::Vec2& anchor ) { m_content->setAnchorPoint(anchor); }

	void setContent(cocos2d::ui::RichText* content);

	inline cocos2d::ui::RichText* getContent() { return m_content; }

	void setVisible(bool visible) override;

	// Override draw
	void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

private:
	static Tooltip* s_currentTooltip;

	cocos2d::Sprite* m_background = nullptr;
	cocos2d::ui::RichText* m_content = nullptr;
};

// Wraps around a node and shows a tooltip when hovered over
class TooltipWidget : public cocos2d::ui::Widget
{
private:
	bool init() override;

	void setInner(cocos2d::Node* inner);
public:
	~TooltipWidget();

	// arg1: The node to wrap around, arg2: The tooltip to show
	CREATE_FUNC_WITH_CTOR_2(TooltipWidget, cocos2d::Node*, Tooltip*);

	void update(float dt) override;

	// The stickiness is how long the tooltip will stay visible after a touch. 
	// Any value less than 0 will make the tooltip stay visible until the next touch.
	void setStickiness(float time) { m_stickiness = time; m_shownTime = 0.0f; }
	
	inline bool isShown() { return m_shown; }
	void toggleTooltip(bool shown);

	inline cocos2d::Node* getInner() { return m_inner; }
	inline Tooltip* getTooltip() { return m_tooltip; }

private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	void onMouseMove(cocos2d::EventMouse* mouse);
#endif
	bool onTouch(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType);

	// Whether the tooltip is currently shown
	bool m_shown = false;
	// Whether the mouse is currently hovering over the widget (note: always false on mobile)
	bool m_hovering = false;
	// Sticky after touch, until the next touch or until the stickiness time is up
	bool m_sticky = false;
	float m_stickiness = -1.0f;
	float m_shownTime = 0.0f;

	cocos2d::Node* m_inner = nullptr;
	Tooltip* m_tooltip = nullptr;
};