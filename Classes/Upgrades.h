#pragma once

#include <vector>
#include <string_view>
#include <cocos2d.h>
#include <memory>
#include <fmt/format.h>
#include "Maths.h"

struct Upgrade {
public:
	Upgrade(const fmt::string_view name, const fmt::string_view description, const fmt::string_view contributionDescription, BigReal greenMatterCost, BigInt unlockLevel);

	fmt::string_view name;
	fmt::string_view description;
	fmt::string_view contributionDescription;

	BigReal greenMatterCost;
	BigInt unlockLevel;

	cocos2d::Texture2D* icon;
};

typedef std::shared_ptr<Upgrade> UpgradePtr;

class Upgrades {
public:
	typedef std::vector<UpgradePtr> UpgradeList;

	static constexpr uint UPGRADE_COUNT = 5;

private:
	Upgrades();
	Upgrades(const Upgrades&) = delete;
	Upgrades(Upgrades&&) = delete;

public:
	static Upgrades* getInstance();
	static const UpgradeList& getUpgrades();
	// Lists upgrades that are unlocked between the given levels
	static const UpgradeList getUpgradesBetweenLevels(BigInt lower, BigInt higher);
	// Fetches up to the next `count` upgrades after `pivot`. If nullptr, this will just return the first `count` upgrades in order
	static const UpgradeList getUpgradesFollowing(UpgradePtr pivot, BigInt count);
	
	
private:
	static Upgrades* m_instance;

	UpgradeList m_upgrades;
};