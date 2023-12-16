#pragma once

#include "ProgressionEvent.h"
#include "JSON_FWD.hpp"

class Progression {
private:
	// Singleton pattern
	Progression() {}
	Progression(const Progression&) = delete;
	Progression(Progression&&) = delete;

public:
	typedef ProgressionEvent::State State;

	static Progression* getInstance();
	static void load(const nlohmann::json& data);

	static bool isAchieved(ProgressionEvent* evnt) { return evnt->getState() == State::ACHIEVED; }
	static State getState(ProgressionEvent* evnt) { return evnt->getState(); }

	// Calls the callback when evnt is achieved. If it's already achieved, the callback will happen immediately
	static void whenAchieved(ProgressionEvent* evnt, std::function<void()> callback);

	static PurchaseFirstHexiiProgressionEvent* purchaseFirstHexii() { return getInstance()->m_purchaseFirstHexii; }
	static HexiiUpgradesAvailableProgressionEvent* hexiiUpgradesAvailable() { return getInstance()->m_hexiiUpgradesAvailable; }
	static HexiiFirstUpgradePurchasedProgressionEvent* hexiiFirstUpgradePurchased() { return getInstance()->m_hexiiFirstUpgradePurchased; }

private:
	friend class HexiiScene;

	// Finishes the initialization by setting up the listeners. This is called after the scene is loaded
	void init();

	static Progression* m_instance;
	std::vector<ProgressionEvent*> m_events;

	PurchaseFirstHexiiProgressionEvent* m_purchaseFirstHexii = nullptr;
	HexiiUpgradesAvailableProgressionEvent* m_hexiiUpgradesAvailable = nullptr;
	HexiiFirstUpgradePurchasedProgressionEvent* m_hexiiFirstUpgradePurchased = nullptr;
};

extern void to_json(nlohmann::json& j, const Progression& progression);