#pragma once

#include "UpgradePath.h"
#include "BoolMap.h"
#include "BeeGroupUpgradeBox.h"
#include <string>
#include "EventUtility.h"

class BeeGroup
{
public:
	enum class BeeType {
		WORKER,
		DRONE
	};

	struct EventUpgradePurchasedData {
		BeeGroup* subject;
		UpgradePtr upgrade;
	};

	struct EventBeeGainedData {
		// The bee group that gained a bee
		BeeGroup* subject;
		BigInt beeCountChange;
		// Upgrades that changed state as a result of the bee gain
		std::vector<UpgradePtr> changedStateUpgrades;
		// The new states of the changed upgrades (in the same order as changedStateUpgrades)
		std::vector<BeeGroupUpgradeBox::State> changedStates;
	};

	constexpr static const char* EVENT_UPGRADE_PURCHASED = "onBeeGroupUpgradePurchased";
	// A nonlocal upgrade is one with the flag ACTS_ON_OTHER_HEXII 
	constexpr static const char* EVENT_NONLOCAL_UPGRADE_PURCHASED = "onBeeGroupNonlocalUpgradePurchased";
	constexpr static const char* EVENT_BEE_GAINED = "onBeeGained";

public:
	BeeGroup(const nlohmann::json& json, BoolMap* upgradesDst, uint district);
	BeeGroup(UpgradePath* upgradePath, BoolMap* upgradesDst, BeeType beeType, uint district);

	const BeeType beeType;
	const UpgradePath* upgradePath;
	const EventUtility::ID eventID = EventUtility::generateID();

	inline BigReal getEXPForNextBee() const { return m_memoizedEXPForNextBee; }
	inline BigInt getBeeCount() const { return m_beeCount; }
	std::string getBeeName() const;
	std::string getBeeNameFriendly();

	// Called whenever exp is gained, to check if the bee count should be increased.
	// returns the change in bee count
	// suppressEvent: if true, no events will be dispatched (used for loading)
	BigInt updateBeeCount(BigReal exp, bool suppressEvent = false);

	void unlockUpgrade(UpgradePtr upgrade);

	BigReal getUpgradePurchaseCostMultiplier() const;
	inline const std::vector<BeeGroupUpgradeBox::State>& getUpgradeStates() const { return m_upgradeStates; }

	/// Upgrade contributions

	BigReal getYieldFromImprovedHandling1Upgrade() const;
	BigReal getYieldFromImprovedHandling2Upgrade() const;
	BigReal getYieldSpeedMultiplierFromImprovedFlight1Upgrade() const;
	BigReal getYieldSpeedMultiplierFromImprovedFlight2Upgrade() const;
	BigReal getCriticalChanceFromBlossomBoost1Upgrade() const;
	BigReal getCriticalYieldMultiplierFromBlossomBoost2Upgrade() const;
	BigReal getActiveYieldSpeedMultiplierFromQueensPresenceUpgrade() const;
	BigReal getAdjacencyYieldMultiplierFromUnity1Upgrade() const;

	// Returns the total yield from constant bonuses (e.g +0.5 per beeCount)
	BigReal getConstantYield() const;
	// Returns the total yield multiplier from additive bonuses
	BigReal getAdditiveYieldMultiplier() const;
	// Returns the total yield multiplier from multiplicative bonuses
	BigReal getMultiplicativeYieldMultiplier() const;

	// Returns the extra yields per second from constant sources
	BigReal getConstantYieldSpeed() const;
	// Returns the multiplier to yields per second from additive sources
	BigReal getAdditiveYieldSpeedMultiplier() const;
	// Returns the multiplier to yields per second from multiplicative sources
	BigReal getMultiplicativeYieldSpeedMultiplier() const;

	// Returns an additive increase to the chance of a crit (represented as a value between 0 and 1)
	BigReal getCriticalChance() const;
	// Returns a multiplicative increase to the yield of a crit
	BigReal getCriticalYieldMultiplier() const;
	// Returns a multiplicative increase to yield from the active bonus
	BigReal getActiveYieldSpeedMultiplier() const;

	// Returns a multiplicative increase this provides to the yields of adjacent hexii
	BigReal getAdjacencyYieldMultiplier() const;

	// Gives a percentage, or if `evaluated` is set, the total value the upgrade evaluates to
	BigReal getContributionFromUpgrade(const std::string& upgradeName, bool evaluated) const;

private:
	void init();

	const uint m_district;

	// Destination for the upgrades
	BoolMap* m_upgradesDst;
	BigInt m_beeCount = 0;
	
	BigReal m_memoizedEXPForNextBee = 0;

	std::vector<BeeGroupUpgradeBox::State> m_upgradeStates;
};

extern void to_json(nlohmann::json& j, const BeeGroup& beeGroup);