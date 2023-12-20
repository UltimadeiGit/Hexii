#pragma once

#include <vector>
#include <string_view>
#include <cocos2d.h>
#include <memory>
#include <fmt/format.h>
#include "Maths.h"

// Upgrade is an immutable representation of a single upgrade's properties
struct Upgrade {
public:
	// Flags for what this upgrade acts on
	enum ACTS_ON_FLAGS {
		ACTS_ON_YIELD = 1 << 0,
		ACTS_ON_SPEED = 1 << 1,
		ACTS_ON_CRITS = 1 << 2,
		ACTS_ON_OTHER_HEXII = 1 << 3 // Whether or not this upgrade has any effect on other hexii
	};

	// Each hexii should store the State of each upgrade
	enum class State {
		LOCKED,
		AVAILABLE,
		OWNED
	};

	typedef uint UpgradeID;

	Upgrade(
		const char* name,
		const char* friendlyName, 
		const char* description, 
		const char* contributionDescription, 
		BigReal greenMatterCost,
		BigInt levelRequirement,
		long long actsOnFlags,
		UpgradeID id
	);

	fmt::string_view name;
	fmt::string_view friendlyName;
	fmt::string_view description;
	fmt::string_view contributionDescription;

	const BigReal greenMatterCost;
	const BigInt levelRequirement;

	const long long actsOnFlags = 0;
	const UpgradeID id;

	cocos2d::Texture2D* icon;
};

typedef std::shared_ptr<Upgrade> UpgradePtr;

/*
class Upgrades {
public:
	typedef std::vector<UpgradePtr> UpgradeVec;

	static constexpr uint UPGRADE_COUNT = 5;

private:
	Upgrades();
	Upgrades(const Upgrades&) = delete;
	Upgrades(Upgrades&&) = delete;

public:
	static Upgrades* getInstance();
	static const UpgradeVec& getUpgrades();
	// Lists upgrades that are unlocked between the given levels
	static const UpgradeVec getUpgradesBetweenLevels(BigInt lower, BigInt higher);
	// Fetches up to the next `count` upgrades after `pivot`. If nullptr, this will just return the first `count` upgrades in order
	static const UpgradeVec getUpgradesFollowing(UpgradePtr pivot, BigInt count);
	
	
private:
	static Upgrades* m_instance;

	UpgradeVec m_upgrades;
};
*/