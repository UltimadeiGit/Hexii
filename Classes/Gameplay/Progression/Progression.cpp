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
	uint achievedCount = 0;

	state = data.contains("purchaseFirstHexiiState") ? data.at("purchaseFirstHexiiState").get<State>() : State::IGNORING;
	m_instance->m_purchaseFirstHexii = new PurchaseFirstHexiiProgressionEvent(state);
	m_instance->m_events.push_back(m_instance->m_purchaseFirstHexii);

	state = data.contains("hexiiUpgradesAvailableState") ? data.at("hexiiUpgradesAvailableState").get<State>() : State::IGNORING;
	m_instance->m_hexiiUpgradesAvailable = new HexiiUpgradesAvailableProgressionEvent(m_instance->m_purchaseFirstHexii, state);
	m_instance->m_events.push_back(m_instance->m_hexiiUpgradesAvailable);

	state = data.contains("hexiiFirstUpgradePurchasedState") ? data.at("hexiiFirstUpgradePurchasedState").get<State>() : State::IGNORING;
	m_instance->m_hexiiFirstUpgradePurchased = new HexiiFirstUpgradePurchasedProgressionEvent(m_instance->m_hexiiUpgradesAvailable, state);
	m_instance->m_events.push_back(m_instance->m_hexiiFirstUpgradePurchased);

	state = data.contains("sacrificeUnlockedState") ? data.at("sacrificeUnlockedState").get<State>() : State::IGNORING;
	achievedCount = data.contains("sacrificeUnlockedAchievedCount") ? data.at("sacrificeUnlockedAchievedCount").get<uint>() : 0;
	m_instance->m_sacrificeUnlocked = new SacrificeUnlockedProgressionEvent(state, achievedCount);
	m_instance->m_events.push_back(m_instance->m_sacrificeUnlocked);

	state = data.contains("firstSacrificeCompletedState") ? data.at("firstSacrificeCompletedState").get<State>() : State::IGNORING;
	m_instance->m_firstSacrificeCompleted = new FirstSacrificeCompletedProgressionEvent(m_instance->m_sacrificeUnlocked, state);
	m_instance->m_events.push_back(m_instance->m_firstSacrificeCompleted);

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

void to_json(nlohmann::json& j, const Progression& progression) {
	j = json{
		{"purchaseFirstHexiiState", progression.purchaseFirstHexii()->getState() },
		{"hexiiUpgradesAvailableState", progression.hexiiUpgradesAvailable()->getState()},
		{"hexiiFirstUpgradePurchasedState", progression.hexiiFirstUpgradePurchased()->getState()},
		{"sacrificeUnlockedState", progression.sacrificeUnlocked()->getState()},
		{"sacrificeUnlockedAchievedCount", progression.sacrificeUnlocked()->getAchievedCount()},
		{"firstSacrificeCompletedState", progression.firstSacrificeCompleted()->getState()}
	};
}

