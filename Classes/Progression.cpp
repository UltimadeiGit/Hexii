#include "Progression.h"
#include "JSON.hpp"
#include "Console.h"

Progression* Progression::m_instance = nullptr;

using namespace nlohmann;

Progression* Progression::getInstance() {
	if (m_instance == nullptr) {
		warn("Loading default values for progression. Ensure this is intended!");
		load({});  // Load default values
	}

	return m_instance;
}

void Progression::load(const nlohmann::json& data) {
	if(m_instance != nullptr) err("Attempted to load progression data twice!");

	m_instance = new Progression();

	State state;

	state = data.contains("purchaseFirstHexii") ? data.at("purchaseFirstHexii").get<State>() : State::IGNORING;
	m_instance->m_purchaseFirstHexii = new PurchaseFirstHexiiProgressionEvent(state);
	m_instance->m_events.push_back(m_instance->m_purchaseFirstHexii);

	state = data.contains("hexiiUpgradesAvailable") ? data.at("hexiiUpgradesAvailable").get<State>() : State::IGNORING;
	m_instance->m_hexiiUpgradesAvailable = new HexiiUpgradesAvailableProgressionEvent(m_instance->m_purchaseFirstHexii, state);
	m_instance->m_events.push_back(m_instance->m_hexiiUpgradesAvailable);

	state = data.contains("hexiiFirstUpgradePurchased") ? data.at("hexiiFirstUpgradePurchased").get<State>() : State::IGNORING;
	m_instance->m_hexiiFirstUpgradePurchased = new HexiiFirstUpgradePurchasedProgressionEvent(m_instance->m_hexiiUpgradesAvailable, state);
	m_instance->m_events.push_back(m_instance->m_hexiiFirstUpgradePurchased);


	//dbg

	for (auto& evnt : m_instance->m_events) {
		printf("State of event: %d\n", (int)evnt->getState());
	}
}

void Progression::init() {
	for (auto& event : m_events) {
		event->init();
	}
}

void Progression::whenAchieved(ProgressionEvent* evnt, std::function<void()> callback) {
	// If the event is already achieved, call the callback immediately
	if (evnt->getState() == State::ACHIEVED) callback();
	// Otherwise, add a listener to call the callback when the event is achieved
	else cocos2d::EventListenerCustom* listener = EventUtility::addTargetedEventListenerFixedPriority(ProgressionEvent::EVENT_PROGRESSION_TO_ACHIEVED, 1, evnt->eventID, 
		[listener, callback](cocos2d::EventCustom*) {
			// Call the callback and remove the listener

			callback(); 
			EventUtility::removeEventListener(listener);
		}
	);
}

void to_json(nlohmann::json& j, const Progression& progression) {
	j = json{
		{"purchaseFirstHexii", progression.purchaseFirstHexii()->getState() },
		{"hexiiUpgradesAvailable", progression.hexiiUpgradesAvailable()->getState()},
		{"hexiiFirstUpgradePurchased", progression.hexiiFirstUpgradePurchased()->getState()}
	};
}

