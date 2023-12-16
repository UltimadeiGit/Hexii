#include "UpgradeTracker.h"
#include "JSON.hpp"

using namespace nlohmann;

UpgradeTracker::UpgradeTracker(UpgradePathPtr path)
	: m_path(path), m_states(path->getUpgradeCount())
{
	int i = 0;
	for (auto& upgrade : path->getUpgrades()) {
		m_states[i] = std::make_pair(upgrade, Upgrade::State::LOCKED);
		i++;
	}
}

UpgradeTracker::UpgradeTracker(const nlohmann::json& data)
{
	m_path = UpgradePath::getPathFromID(data["pathID"].get<uint>());
	auto& upgrades = m_path->getUpgrades();

	m_states.resize(m_path->getUpgradeCount());

	auto states = data.at("states");

	if (!states.is_null()) {
		int i = 0;
		for (json::iterator it = states.begin(); it != states.end(); it++) {
			m_states[i].first = upgrades[i];
			m_states[i].second = it->get<Upgrade::State>();

			i++;
		}
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

const std::vector<Upgrade::State> UpgradeTracker::getStates() const
{
	std::vector<Upgrade::State> states; 

	for (auto& pair : m_states) { 
		states.push_back(pair.second); 
	} 
	
	return states;
}

void to_json(nlohmann::json& j, const UpgradeTracker& tracker)
{
	j = nlohmann::json{
		{ "pathID", tracker.getPath()->getID() },
		{ "states", tracker.getStates() }
	};
}
