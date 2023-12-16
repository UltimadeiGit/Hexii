#pragma once
/*
#include "UpgradePath.h"
#include "BoolMap.h"
#include <string>
#include "EventUtility.h"

// A specialization defines how a hexii's upgrades are organized. 
// Each one subscribes to its own UpgradePath and handles implementing their effects
class Specialization
{
public:
	enum class SpecType {
		STANDARD_L0,
		STANDARD
	};

	struct EventUpgradePurchasedData {
		Specialization* subject;
		UpgradePtr upgrade;
	};

	constexpr static const char* EVENT_UPGRADE_PURCHASED = "onUpgradePurchased";
	// A nonlocal upgrade is one with the flag ACTS_ON_OTHER_HEXII 
	constexpr static const char* EVENT_NONLOCAL_UPGRADE_PURCHASED = "onNonlocalUpgradePurchased";

public:
	Specialization(const nlohmann::json& json, BoolMap* upgradesDst, uint layer);
	Specialization(BoolMap* upgradesDst, SpecType specType, uint layer);

	const SpecType specType;
	const UpgradePath* upgradePath;
	const EventUtility::ID eventID = EventUtility::generateID();

	inline BigReal getEXPForNextLevel() const { return m_memoizedEXPForNextLevel; }
	inline BigInt getLevel() const { return m_level; }
	std::string getSpecName() const;
	std::string getSpecNameFriendly();

	void unlockUpgrade(UpgradePtr upgrade);

	BigReal getUpgradePurchaseCostMultiplier() const;
	inline const std::vector<UpgradeBox::State>& getUpgradeStates() const { return m_upgradeStates; }

private:
	void init();

	const uint m_layer;

	// Destination for the upgrades
	//BoolMap* m_upgradesDst;

	std::vector<Upgrade::State> m_upgradeStates;
};

extern void to_json(nlohmann::json& j, const Specialization& specGroup);
*/