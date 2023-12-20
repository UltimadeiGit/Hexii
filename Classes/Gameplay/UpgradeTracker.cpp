#include "UpgradeTracker.h"
#include "JSON.hpp"
#include "Console.h"

using namespace nlohmann;

UpgradeTracker::UpgradeTracker(UpgradePathPtr path)
	: m_path(path), m_states(path->getUpgradeCount())
{
	initStates();
}

UpgradeTracker::UpgradeTracker(const nlohmann::json& data)
{
	m_path = UpgradePath::getPathFromID(data["pathID"].get<uint>());

	initStates();

	// Attempt to load states from JSON by looking up upgrade IDs
	try {
		auto& states = data.at("states");
		if (!states.is_null()) {
			// Iterate over the upgrades we've already initialised and set their states
			// to the ones found in the JSON (if possible)

			auto stateMap = states.get<std::map< Upgrade::UpgradeID, Upgrade::State>>();

			for (int i = 0; i < m_states.size(); i++) {
				// Look up the upgrade ID in the state map
				auto it = stateMap.find(m_states[i].first->id);
				if (it == stateMap.end()) continue;

				m_states[i].second = it->second;
			}
		}
	}
	catch (json::out_of_range& e) {
		// No states found
	}
	catch (json::type_error& e) {
		// States found, but not in the correct format
		err("Upgrade states are not in the correct format");
	}
	
}

const Upgrade::State& UpgradeTracker::getState(uint upgradeIndex) const {
	return m_states[upgradeIndex].second;
}

uint UpgradeTracker::indexOf(const std::string& upgradeName) const {
	for (uint i = 0; i < m_states.size(); i++) {
		if (m_states[i].first->name == upgradeName) return i;
	}
	throw std::out_of_range("Upgrade not found");
}

void UpgradeTracker::purchaseUpgrade(UpgradePtr upgrade) {
	uint index = indexOf(upgrade);

	if (m_states[index].second != Upgrade::State::AVAILABLE) {
		throw std::runtime_error("Upgrade is not available");
	}

	m_states[index].second = Upgrade::State::OWNED;
}

std::vector<std::pair<UpgradePtr, Upgrade::State>> UpgradeTracker::updateStatesDueToLevel(BigInt level) {
	std::vector<std::pair<UpgradePtr, Upgrade::State>> changedStates;

	for (auto& pair : m_states) {
		auto& upgrade = pair.first;
		auto& state = pair.second;

		if (state == Upgrade::State::OWNED) continue;

		if (upgrade->levelRequirement <= level) {
			if (state != Upgrade::State::AVAILABLE) {
				changedStates.push_back(std::make_pair(upgrade, state));
				state = Upgrade::State::AVAILABLE;
			}
		}
		else {
			// TODO: In future, it may not be valid just to set the state to LOCKED. Just bear this in mind
			if (state != Upgrade::State::LOCKED) {
				changedStates.push_back(std::make_pair(upgrade, state));
				state = Upgrade::State::LOCKED;
			}
		}
	}

	return changedStates;
}

std::vector<Upgrade::State> UpgradeTracker::getStates() const {
	std::vector<Upgrade::State> states;

	for (int i = 0; i < m_states.size(); i++) {
		states.push_back(m_states[i].second);
	}	

	return states;
}

std::map<Upgrade::UpgradeID, Upgrade::State> UpgradeTracker::getStateMap() const
{
	std::map<Upgrade::UpgradeID, Upgrade::State> stateMap;

	for (int i = 0; i < m_states.size(); i++) {
		stateMap.emplace(std::make_pair(m_states[i].first->id, m_states[i].second));
	} 
	
	return stateMap;
}

void UpgradeTracker::initStates() {
	auto& upgrades = m_path->getUpgrades();

	// Initialise states to LOCKED by default
	m_states.resize(upgrades.size());
	for (int i = 0; i < upgrades.size(); i++) {
		m_states[i] = std::make_pair(upgrades[i], Upgrade::State::LOCKED);
	}

	// Sort the vector by level requirement
	std::sort(m_states.begin(), m_states.end(),
		[](const std::pair<UpgradePtr, Upgrade::State>& a, const std::pair<UpgradePtr, Upgrade::State>& b) {
			return a.first->levelRequirement < b.first->levelRequirement;
		}
	);
}

void to_json(nlohmann::json& j, const UpgradeTracker& tracker)
{
	j = nlohmann::json{
		{ "pathID", tracker.getPath()->getID() },
		{ "states", tracker.getStateMap() }
	};
}
