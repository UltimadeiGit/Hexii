#pragma once

#include <cocos2d.h>

namespace EventUtility {
	typedef long long ID;

	// Generates a unique ID used for dispatching events.
	inline ID generateID() {
		static ID id = 0;

		return id++;
	}

	inline std::string IDStr(ID id) { return std::to_string(id); }
	inline cocos2d::EventDispatcher* getEventDispatcher() { return cocos2d::Director::getInstance()->getEventDispatcher(); }
	
	// Dispatches the event with the given name both globally and for the given ID.
	template <typename _EventDataT>
	void dispatchEvent(const std::string& eventName, const ID id, _EventDataT data) {
		// TODO: At some point, check that the custom data is in fact deleted.
		getEventDispatcher()->dispatchCustomEvent(eventName, new _EventDataT(data));
		getEventDispatcher()->dispatchCustomEvent(eventName + "_" + std::to_string(id), new _EventDataT(data));
	}

	// A targeted event listener only listens for events dispatched to the given ID.
	template <typename _Ty, typename _FnSel>
	cocos2d::EventListenerCustom* addTargetedEventListener(const std::string& eventName, _Ty* instance, ID targetID, _FnSel function) {
		auto listener = EventListenerCustom::create(eventName + "_" + IDStr(targetID), std::bind(function, instance, std::placeholders::_1));
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, instance);
		return listener;
	}

	// A global event listener listens for events dispatched globally.
	template <typename _Ty, typename _FnSel>
	cocos2d::EventListenerCustom* addGlobalEventListener(const std::string& eventName, _Ty* instance, _FnSel function) {
		auto listener = EventListenerCustom::create(eventName, std::bind(function, instance, std::placeholders::_1));
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, instance);
		return listener;
	}

	template <typename _EventDataT>
	_EventDataT* getEventData(cocos2d::EventCustom* evnt) {
		return static_cast<_EventDataT*>(evnt->getUserData());
	}
}
