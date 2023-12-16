#include "Hexii.h"

// +1 per level [Constant]
BigReal Hexii::getYieldFromYieldUp1Upgrade() const {
	return m_level * (m_layer + 1) * m_standardPathTracker->isOwned(m_type == HexiiType::HOME_L0 ? "GreenMatterYieldUp1" : "EXPYieldUp1");
}

// x1.03 per level [Multiplicative]
BigReal Hexii::getYieldFromYieldUp2Upgrade() const {
	return m_standardPathTracker->isOwned(m_type == HexiiType::HOME_L0 ? "GreenMatterYieldUp2" : "EXPYieldUp2") ? std::powl(1.03, m_level) : 1.0;
}

// +50% [Additive]
BigReal Hexii::getYieldSpeedMultiplierFromSpeedUp1Upgrade() const {
	return 0.5 * m_standardPathTracker->isOwned("SpeedUp1");
}

// +2% per level [Additive]
BigReal Hexii::getYieldSpeedMultiplierFromSpeedUp2Upgrade() const {
	return (0.02 * m_level * m_standardPathTracker->isOwned("SpeedUp2"));
}

// 0.02 plus 0.01*log_1.5(level) [Constant]
BigReal Hexii::getCriticalChanceFromCriticalChance1Upgrade() const {
	return m_standardPathTracker->isOwned("CriticalChance1") ? 0.02 + 0.01 * (std::logl(m_level) / 0.17609) : 0;
}

// x2 [Multiplicative]
BigReal Hexii::getCriticalYieldMultiplierFromCriticalBonus1Upgrade() const {
	return 1 + m_standardPathTracker->isOwned("CriticalBonus1");
}

// x1.5 [Multiplicative]
BigReal Hexii::getActiveYieldSpeedMultiplierFromStrongArmUpgrade() const {
	return 1 + 0.5 * m_standardPathTracker->isOwned("StrongArm");
}

BigReal Hexii::getAdjacencyYieldMultiplierFromSupport1Upgrade() const {
	return (0.01 * m_level * std::powl(m_layer + 1, 2) * m_standardPathTracker->isOwned("Support1"));
}

BigReal Hexii::getContributionFromUpgrade(const std::string& upgradeName, bool evaluated) const {
	// Constant yield upgrades will always return constants (of course)

	BigReal contribution = 0;

	// Constant only here:

	if (upgradeName == "GreenMatterYieldUp1" || upgradeName == "EXPYieldUp1") contribution = getYieldFromYieldUp1Upgrade();

	// Multipliers, formatted as percentages
	else if (!evaluated) {
		if (upgradeName == "GreenMatterYieldUp2" || upgradeName == "EXPYieldUp2") contribution = multiplierToPercentage(getYieldFromYieldUp2Upgrade());
		else if (upgradeName == "CriticalChance1") contribution = 100 * getCriticalChanceFromCriticalChance1Upgrade();
		else if (upgradeName == "SpeedUp2") contribution = 100 * getYieldSpeedMultiplierFromSpeedUp2Upgrade();
		else if (upgradeName == "Support1") contribution = 100 * getAdjacencyYieldMultiplierFromSupport1Upgrade();
		else return 0;
	}
	// Multipliers, formatted as constants
	else {
		if (upgradeName == "GreenMatterYieldUp2" || upgradeName == "EXPYieldUp2") contribution = (getYieldFromYieldUp2Upgrade() - 1) * getConstantYield();
	}

	return contribution;
}

//////////////////////// SUMMATIONS ////////////////////////

/// Yield

BigReal Hexii::getConstantYield() const {
	return getYieldFromYieldUp1Upgrade();
}

BigReal Hexii::getAdditiveYieldMultiplier() const {
	return 1;
}

BigReal Hexii::getMultiplicativeYieldMultiplier() const {
	return getYieldFromYieldUp2Upgrade();
}

/// Yield speed

BigReal Hexii::getConstantYieldSpeed() const {
	return 0;
}

BigReal Hexii::getAdditiveYieldSpeedMultiplier() const {
	return 1 +
		getYieldSpeedMultiplierFromSpeedUp1Upgrade() +
		getYieldSpeedMultiplierFromSpeedUp2Upgrade()
		;
}

BigReal Hexii::getMultiplicativeYieldSpeedMultiplier() const {
	return 1;
}

// [Constant]
BigReal Hexii::getCriticalChance() const {
	return getCriticalChanceFromCriticalChance1Upgrade();
}

// [Multiplicative]
BigReal Hexii::getCriticalYieldMultiplier() const {
	return getCriticalYieldMultiplierFromCriticalBonus1Upgrade();
}

// [Multiplicative]
BigReal Hexii::getActiveYieldSpeedMultiplier() const {
	return getActiveYieldSpeedMultiplierFromStrongArmUpgrade();
}
