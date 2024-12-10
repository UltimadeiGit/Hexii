#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>

#include "EventUtility.h"
#include "Console.h"

namespace TabbedWidgetCommon {
	constexpr static const char* EVENT_SWITCHED_TAB = "tabbedWidgetSwitchedTab";
}

// Contains various tabs and can switch between them
template <unsigned int TabCount>
class TabbedWidget :
	public cocos2d::ui::Widget {
public:
	struct EventSwitchedTabData {
		TabbedWidget<TabCount>* subject;
		unsigned short tabNumber;
		cocos2d::Node* tab;
		cocos2d::ui::Button* tabButton;
	};

private:
	inline bool init() { return true; }
	inline void onTabPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType, uint tabNumber) {
		// Only care about release
		if (evntType != cocos2d::ui::Widget::TouchEventType::ENDED) return;

		switchTab(tabNumber);
	}

public:
	EventUtility::ID eventID = EventUtility::generateID();

	inline virtual void switchTab(unsigned short tab) {
		// Some tabs may not yet be enabled
		if (!isTabEnabled(tab)) return;

		m_tabs[m_currentTab]->setVisible(false);
		m_tabButtons[m_currentTab]->setHighlighted(false);

		m_currentTab = tab;
		m_tabs[tab]->setVisible(true);
		m_tabButtons[tab]->setHighlighted(true);

		// Dispatch event
		EventUtility::dispatchEvent<EventSwitchedTabData>(TabbedWidgetCommon::EVENT_SWITCHED_TAB, eventID, { this, tab, m_tabs[tab], m_tabButtons[tab] });
	}

	inline unsigned short getCurrentTabNumber() const { return m_currentTab; }

	inline bool isTabEnabled(unsigned short tabNumber) {
		return m_tabButtons[tabNumber] != nullptr && m_tabs[tabNumber] != nullptr && m_tabButtons[tabNumber]->isEnabled();
	}

	inline cocos2d::Node* getTab(unsigned short tabNumber) const { return m_tabs[tabNumber]; }
	inline cocos2d::ui::Button* getTabButton(unsigned short tabNumber) { return m_tabButtons[tabNumber]; }

protected:
	TabbedWidget() {}

	// Sets the positions of the tabs and their buttons.
	// Each button will be offsetted by `buttonStep` from the previous one, beginning at `buttonOrigin`.
	// If set to init enabled, enableTab() will be called. Otherwise, the tab will be disabled.
	inline virtual void initTab(unsigned short tabNumber, cocos2d::Node* tab, cocos2d::Vec2 tabPos, cocos2d::Vec2 buttonAnchor, cocos2d::Vec2 buttonOrigin, cocos2d::Vec2 buttonStep, const std::string& buttonTexturePath, const std::string& disabledTexturePath, bool initEnabled = true) {
		// Set the size and position for all tabs
		tab->setAnchorPoint({ 0.0, 0.0 });
		tab->setPosition(tabPos);
		tab->setVisible(false);

		// Create the button
		cocos2d::ui::Button* button = cocos2d::ui::Button::create(
			buttonTexturePath + ".png",
			buttonTexturePath + "Selected.png",
			disabledTexturePath + ".png"
		);

		// Set the button's position and anchor point
		button->setAnchorPoint(buttonAnchor);
		button->setPosition(buttonOrigin + (buttonStep * tabNumber));

		// Add them as children
		m_tabs[tabNumber] = tab;
		m_tabButtons[tabNumber] = button;

		this->addChild(tab, 1);
		this->addChild(button, -1);

		// Setup listener
		addListenerForTabButton(tabNumber);

		if (!initEnabled) {
			button->setEnabled(false);
			button->setBright(false);
		} else enableTab(tabNumber, false);
	}

	inline void addListenerForTabButton(unsigned short tabNumber) {
		m_tabButtons[tabNumber]->addTouchEventListener(CC_CALLBACK_2(TabbedWidget<TabCount>::onTabPressed, this, tabNumber));
	}

	// Removes a tab and its button. Note, does not switch to another tab.
	inline virtual void removeTab(unsigned short tabNumber) {
		cocos2d::Node* tab = m_tabs[tabNumber];
		if (tab != nullptr) {
			tab->removeFromParentAndCleanup(true);
			m_tabs[tabNumber] = nullptr;
		}

		cocos2d::ui::Button* button = m_tabButtons[tabNumber];
		if (button != nullptr){
			button->removeFromParentAndCleanup(true);
			m_tabButtons[tabNumber] = nullptr; 
		}
	}
	
	// Enable a tab when a certain trigger happens
	inline virtual void enableTab(uint tabNumber, bool autoSwitch = true) {
		if (tabNumber >= TabCount) return err("Tab number out of bounds");

		m_tabButtons[tabNumber]->setEnabled(true);
		m_tabButtons[tabNumber]->setBright(true);
		if(autoSwitch) switchTab(tabNumber);
	}

protected:
	virtual void update(float dt) override {}

	std::string m_tabAssetPath;

	unsigned short m_currentTab = 0;
	cocos2d::Node* m_tabs[TabCount]{};
	cocos2d::ui::Button* m_tabButtons[TabCount]{};
};