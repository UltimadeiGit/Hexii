#include "BeeGroup.h"

// +1 per beeCount [Constant]
BigReal BeeGroup::getYieldFromImprovedHandling1Upgrade() const {
	return m_district * (m_district + 1) * (*m_upgradesDst)("ImprovedHandling1");
}

// x1.03 per beeCount [Multiplicative]
BigReal BeeGroup::getYieldFromImprovedHandling2Upgrade() const {
	return (*m_upgradesDst)("ImprovedHandling2") ? std::powl(1.03, m_district) : 1.0;
}

// +50% [Additive]
BigReal BeeGroup::getYieldSpeedMultiplierFromImprovedFlight1Upgrade() const {
	return 0.5 * (*m_upgradesDst)("ImprovedFlight1");
}

// +2% per beeCount [Additive]
BigReal BeeGroup::getYieldSpeedMultiplierFromImprovedFlight2Upgrade() const {
	return (0.02 * m_district * (*m_upgradesDst)("ImprovedFlight2"));
}

// 0.02 plus 0.01*log_1.5(beeCount) [Constant]
BigReal BeeGroup::getCriticalChanceFromBlossomBoost1Upgrade() const {
	return (*m_upgradesDst)("BlossomBoost1") ? 0.02 + 0.01 * (std::logl(m_beeCount) / 0.17609) : 0;
}

// x2 [Multiplicative]
BigReal BeeGroup::getCriticalYieldMultiplierFromBlossomBoost2Upgrade() const {
	return 1 + (*m_upgradesDst)("BlossomBoost2");
}

// x1.5 [Multiplicative]
BigReal BeeGroup::getActiveYieldSpeedMultiplierFromQueensPresenceUpgrade() const {
	return 1 + 0.5 * (*m_upgradesDst)("QueensPresence");
}

BigReal BeeGroup::getAdjacencyYieldMultiplierFromUnity1Upgrade() const {
	return (0.01 * m_district * std::powl(m_district + 1, 2) * (*m_upgradesDst)("Unity1"));
}

BigReal BeeGroup::getContributionFromUpgrade(const std::string& upgradeName, bool evaluated) const {
	// Constant yield upgrades will always return constants (of course)

	BigReal contribution = 0;

	// Constant only here:

	if (upgradeName == "ImprovedHandling1") contribution = getYieldFromImprovedHandling1Upgrade();

	// Multipliers, formatted as percentages
	else if (!evaluated) {
		if (upgradeName == "ImprovedHandling2") contribution = multiplierToPercentage(getYieldFromImprovedHandling2Upgrade());
		else if (upgradeName == "BlossomBoost1") contribution = 100 * getCriticalChanceFromBlossomBoost1Upgrade();
		else if (upgradeName == "ImprovedFlight2") contribution = 100 * getYieldSpeedMultiplierFromImprovedFlight2Upgrade();
		else if (upgradeName == "Unity1") contribution = 100 * getAdjacencyYieldMultiplierFromUnity1Upgrade();
		else return 0;
	}
	// Multipliers, formatted as constants
	else {
		if (upgradeName == "ImprovedHandling2") contribution = (getYieldFromImprovedHandling2Upgrade() - 1) * getConstantYield();
	}

	return contribution;
}

//////////////////////// SUMMATIONS ////////////////////////

/// Yield

BigReal BeeGroup::getConstantYield() const {
	return getYieldFromImprovedHandling1Upgrade();
}

BigReal BeeGroup::getAdditiveYieldMultiplier() const {
	return 1;
}

BigReal BeeGroup::getMultiplicativeYieldMultiplier() const {
	return getYieldFromImprovedHandling2Upgrade();
}

/// Yield speed

BigReal BeeGroup::getConstantYieldSpeed() const {
	return 0;
}

BigReal BeeGroup::getAdditiveYieldSpeedMultiplier() const {
	return 1 +
		getYieldSpeedMultiplierFromImprovedFlight1Upgrade() +
		getYieldSpeedMultiplierFromImprovedFlight2Upgrade()
		;
}

BigReal BeeGroup::getMultiplicativeYieldSpeedMultiplier() const {
	return 1;
}

// [Constant]
BigReal BeeGroup::getCriticalChance() const {
	return getCriticalChanceFromBlossomBoost1Upgrade();
}

// [Multiplicative]
BigReal BeeGroup::getCriticalYieldMultiplier() const {
	return getCriticalYieldMultiplierFromBlossomBoost2Upgrade();
}

// [Multiplicative]
BigReal BeeGroup::getActiveYieldSpeedMultiplier() const {
	return getActiveYieldSpeedMultiplierFromQueensPresenceUpgrade();
}

BigReal BeeGroup::getAdjacencyYieldMultiplier() const {
	return 1 +
		getAdjacencyYieldMultiplierFromUnity1Upgrade();
}
