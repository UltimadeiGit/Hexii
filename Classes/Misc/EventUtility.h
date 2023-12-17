#pragma once

#ifndef EVENT_UTILITY_H
#define EVENT_UTILITY_H
#include <cocos2d.h>

namespace EventUtility {
	typedef long long ID;
	typedef std::function<void(cocos2d::EventCustom*)> EventCallback;

	// Generates a unique ID used for dispatching events.
	inline ID generateID() {
		static ID id = 0;

		return id++;
	}

	inline std::string IDStr(ID id) { return std::to_string(id); }
	inline cocos2d::EventDispatcher* getEventDispatcher() { return cocos2d::Director::getInstance()->getEventDispatcher(); }
	
	// Dispatches the event with the given name both globally and for the given ID.
	template <typename _EventDataT>
	inline void dispatchEvent(const std::string& eventName, const ID id, _EventDataT data) {
		// TODO: At some point, check that the custom data is in fact deleted.
		getEventDispatcher()->dispatchCustomEvent(eventName, new _EventDataT(data));
		getEventDispatcher()->dispatchCustomEvent(eventName + "_" + std::to_string(id), new _EventDataT(data));
	}

	// Dispatches the event with the given name both globally and for the given ID. 
	inline void dispatchEvent(const std::string& eventName, const ID id) {
		// TODO: At some point, check that the custom data is in fact deleted.
		getEventDispatcher()->dispatchCustomEvent(eventName);
		getEventDispatcher()->dispatchCustomEvent(eventName + "_" + std::to_string(id));
	}

	// A targeted event listener only listens for events dispatched to the given ID.
	template <typename _Ty, typename _FnSel>
	inline cocos2d::EventListenerCustom* addTargetedEventListener(const std::string& eventName, _Ty* instance, ID targetID, _FnSel function) {
		auto listener = cocos2d::EventListenerCustom::create(eventName + "_" + IDStr(targetID), std::bind(function, instance, std::placeholders::_1));
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, instance);
		getEventDispatcher()->resumeEventListenersForTarget(instance);
		return listener;
	}

	// A targeted event listener only listens for events dispatched to the given ID.
	inline cocos2d::EventListenerCustom* addTargetedEventListener(const std::string& eventName, cocos2d::Node* instance, ID targetID, EventCallback function) {
		auto listener = cocos2d::EventListenerCustom::create(eventName + "_" + IDStr(targetID), function);
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, instance);
		getEventDispatcher()->resumeEventListenersForTarget(instance);
		return listener;
	}

	// A targeted event listener only listens for events dispatched to the given ID and acts with fixed priority
	template <typename _Ty, typename _FnSel>
	inline cocos2d::EventListenerCustom* addTargetedEventListenerFixedPriority(const std::string& eventName, int priority, _Ty* instance, ID targetID, _FnSel function) {
		auto listener = cocos2d::EventListenerCustom::create(eventName + "_" + IDStr(targetID), std::bind(function, instance, std::placeholders::_1));
		getEventDispatcher()->addEventListenerWithFixedPriority(listener, priority);
		return listener;
	}

	// A targeted event listener only listens for events dispatched to the given ID and acts with fixed priority
	inline cocos2d::EventListenerCustom* addTargetedEventListenerFixedPriority(const std::string& eventName, int priority, ID targetID, EventCallback function) {
		auto listener = cocos2d::EventListenerCustom::create(eventName + "_" + IDStr(targetID), function);
		getEventDispatcher()->addEventListenerWithFixedPriority(listener, priority);
		return listener;
	}

	// A global event listener listens for events dispatched globally.
	template <typename _Ty, typename _FnSel>
	inline cocos2d::EventListenerCustom* addGlobalEventListener(const std::string& eventName, _Ty* instance, _FnSel function) {
		auto listener = cocos2d::EventListenerCustom::create(eventName, std::bind(function, instance, std::placeholders::_1));
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, instance);
		getEventDispatcher()->resumeEventListenersForTarget(instance);
		return listener;
	}

	// A global event listener listens for events dispatched globally.
	inline cocos2d::EventListenerCustom* addGlobalEventListener(const std::string& eventName, cocos2d::Node* instance, EventCallback function) {
		auto listener = cocos2d::EventListenerCustom::create(eventName, function);
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, instance);
		getEventDispatcher()->resumeEventListenersForTarget(instance);
		return listener;
	}

	// A global event listener listens for events dispatched globally.
	template <typename _Ty, typename _FnSel>
	inline cocos2d::EventListenerCustom* addGlobalEventListenerFixedPriority(const std::string& eventName, int priority, _Ty* instance, _FnSel function) {
		auto listener = cocos2d::EventListenerCustom::create(eventName, std::bind(function, instance, std::placeholders::_1));
		getEventDispatcher()->addEventListenerWithFixedPriority(listener, priority);
		return listener;
	}

	// A global event listener listens for events dispatched globally.
	inline cocos2d::EventListenerCustom* addGlobalEventListenerFixedPriority(const std::string& eventName, int priority, EventCallback function) {
		auto listener = cocos2d::EventListenerCustom::create(eventName, function);
		getEventDispatcher()->addEventListenerWithFixedPriority(listener, priority);
		return listener;
	}

	// Schedules the given function to be called on the given instance.
	template <typename _Ty, typename _FnSel>
	inline void scheduleFunctionOnSelf(_Ty* instance, _FnSel function, float interval = 0.0f, unsigned int repeat = 0, const std::string& key = "scheduledFunction") {
		cocos2d::Director::getInstance()->getScheduler()->schedule(
			std::bind(function, instance), instance, interval, repeat, 0.0f, false, key
		);
	}

	template <typename _EventDataT>
	inline _EventDataT* getEventData(cocos2d::EventCustom* evnt) {
		return static_cast<_EventDataT*>(evnt->getUserData());
	}

	// Removes the given listener.
	inline void removeEventListener(cocos2d::EventListenerCustom* listener) {
		if(listener) getEventDispatcher()->removeEventListener(listener);
	}
}

#endif // !EVENT_UTILITY_H