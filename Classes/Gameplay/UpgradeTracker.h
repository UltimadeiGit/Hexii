#pragma once

#include <map>

#include "cocos2d.h"
#include "UpgradePath.h"

// Stores the state of upgrades along a given path
class UpgradeTracker
{
public:
	UpgradeTracker(UpgradePathPtr path);
	UpgradeTracker(const nlohmann::json& data);

	const Upgrade::State& getState(uint upgradeIndex) const;
	const Upgrade::State& getState(const std::string& upgradeName) const { return getState(indexOf(upgradeName)); }
	const Upgrade::State& getState(UpgradePtr upgrade) const { return getState(indexOf(upgrade)); }

	inline bool isOwned(uint upgradeIndex) const { return getState(upgradeIndex) == Upgrade::State::OWNED; }
	inline bool isOwned(const std::string& upgradeName) const { return isOwned(indexOf(upgradeName)); }
	inline bool isOwned(UpgradePtr upgrade) const { return isOwned(indexOf(upgrade)); }
	
	uint indexOf(const std::string& upgradeName) const;
	uint indexOf(UpgradePtr upgrade) const { return indexOf(fmt::to_string(upgrade->name)); }

	void purchaseUpgrade(UpgradePtr upgrade);

	// Updates each upgrade's state based on the given level
	// Returns a vector of pairs of affected upgrades and their new states
	std::vector<std::pair<UpgradePtr, Upgrade::State>> updateStatesDueToLevel(BigInt level);


	// Exposed info

	inline UpgradePathPtr getPath() const { return m_path; }
	// Returns a vector of pairs of upgrades and their states
	inline const std::vector<std::pair<UpgradePtr, Upgrade::State>>& getUpgradeStatePairs() const { return m_states; }
	const std::vector<Upgrade::State> getStates() const;
private:
	UpgradePathPtr m_path;

	std::vector<std::pair<UpgradePtr, Upgrade::State>> m_states;
};

typedef std::shared_ptr<UpgradeTracker> UpgradeTrackerPtr;
extern void to_json(nlohmann::json& j, const UpgradeTracker& tracker);