#pragma once

#include <vector>
#include <string_view>
#include <cocos2d.h>
#include <memory>
#include "Maths.h"

struct Upgrade {
public:
	Upgrade(const std::string_view name, const std::string_view description, BigReal greenMatterCost, BigInt unlockLevel);

	std::string_view name;
	std::string_view description;

	BigReal greenMatterCost;
	BigInt unlockLevel;

	cocos2d::Texture2D* icon;
};

typedef std::shared_ptr<Upgrade> UpgradePtr;

class Upgrades {
private:
	Upgrades();
	Upgrades(const Upgrades&) = delete;
	Upgrades(Upgrades&&) = delete;

public:
	typedef std::vector<UpgradePtr> UpgradeList;

	static Upgrades* getInstance();
	static const UpgradeList& getUpgrades();
	// Lists upgrades that are unlocked between the given levels
	static const UpgradeList getUpgradesBetweenLevels(uint lower, uint higher);
	// Fetches up to the next `count` upgrades after `pivot`. If nullptr, this will just return the first `count` upgrades in order
	static const UpgradeList getUpgradesFollowing(UpgradePtr pivot, uint count);
	// Returns the total number of upgrades available
	static const uint getUpgradeCount();
private:
	static Upgrades* m_instance;

	UpgradeList m_upgrades;
};