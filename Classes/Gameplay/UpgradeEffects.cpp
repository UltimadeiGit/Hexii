#include "Hexii.h"

// Note: Constant should be 0 when not owned.
// Additive should be 0 when not owned.
// Multiplicative should be 1 when not owned.

// +1 per level per layer [Constant]
BigReal Hexii::getYieldFromYieldUp1Upgrade() const {
	return m_totalLevel * (m_layer + 1) * m_standardPathTracker->isOwned(m_type == HexiiType::HOME_L0 ? "GreenMatterYieldUp1" : "EXPYieldUp1");
}

// +100% [Additive]
BigReal Hexii::getYieldFromYieldUp2Upgrade() const {
	return 1.0 * m_standardPathTracker->isOwned(m_type == HexiiType::HOME_L0 ? "GreenMatterYieldUp2" : "EXPYieldUp2");
}

// x1.03 per level [Multiplicative]
BigReal Hexii::getYieldFromYieldUp3Upgrade() const {
	return m_standardPathTracker->isOwned(m_type == HexiiType::HOME_L0 ? "GreenMatterYieldUp3" : "EXPYieldUp3") ? std::powl(1.03, m_totalLevel) : 1.0;
}

// x1.1 per level after 60 [Multiplicative]
BigReal Hexii::getYieldFromYieldUp4Upgrade() const {
	return m_standardPathTracker->isOwned(m_type == HexiiType::HOME_L0 ? "GreenMatterYieldUp4" : "EXPYieldUp4") ? std::powl(1.1, m_totalLevel > 60 ? m_totalLevel - 60 : 0) : 1.0;
}

// +50% [Additive]
BigReal Hexii::getYieldSpeedMultiplierFromSpeedUp1Upgrade() const {
	return 0.5 * m_standardPathTracker->isOwned("SpeedUp1");
}

// +50% [Additive]
BigReal Hexii::getYieldSpeedMultiplierFromSpeedUp2Upgrade() const {
	return 0.5 * m_standardPathTracker->isOwned("SpeedUp2");
}

// +1% per level [Additive]
BigReal Hexii::getYieldSpeedMultiplierFromSpeedUp3Upgrade() const {
	return (0.01 * m_totalLevel * m_standardPathTracker->isOwned("SpeedUp3"));
}

// x1.01 per level after 60 [Multiplicative]
BigReal Hexii::getYieldSpeedMultiplierFromSpeedUp4Upgrade() const {
	return m_standardPathTracker->isOwned("SpeedUp4") ? std::powl(1.01, m_totalLevel > 60 ? m_totalLevel - 60 : 0) : 1.0;
}

// +5% [Additive]
BigReal Hexii::getCriticalChanceFromCriticalChance1Upgrade() const {
	return 0.05 * m_standardPathTracker->isOwned("CriticalChance1");
}

/*
// 0.02 plus 0.01*log_1.5(level) [Constant]
BigReal Hexii::getCriticalChanceFromCriticalChance1Upgrade() const {
	return m_standardPathTracker->isOwned("CriticalChance1") ? 0.02 + 0.01 * (std::logl(m_totalLevel) / 0.17609) : 0;
}
*/

// x2 [Multiplicative]
BigReal Hexii::getCriticalYieldMultiplierFromCriticalBonus1Upgrade() const {
	return m_standardPathTracker->isOwned("CriticalBonus1") ? 2.0 : 1.0;
}

// x1.5 [Multiplicative]
BigReal Hexii::getActiveYieldSpeedMultiplierFromStrongArmUpgrade() const {
	return m_standardPathTracker->isOwned("StrongArm") ? 1.5 : 1.0;
}

// +1% per level * (layer+1)^2 [Additive]
BigReal Hexii::getAdjacencyYieldMultiplierFromSupport1Upgrade() const {
	return (0.01 * m_totalLevel * std::powl(m_layer + 1, 2) * m_standardPathTracker->isOwned("Support1"));
}

BigReal Hexii::getContributionFromUpgrade(const std::string& upgradeName/*, bool evaluated*/) const {
	// Constant yield upgrades will always return constants (of course)

	BigReal contribution = 0;

	// Constant only here:

	if (upgradeName == "GreenMatterYieldUp1" || upgradeName == "EXPYieldUp1") contribution = getYieldFromYieldUp1Upgrade();

	// Multipliers
	else /*if (!evaluated)*/ {
		if (upgradeName == "GreenMatterYieldUp2" || upgradeName == "EXPYieldUp2") contribution = 100 * getYieldFromYieldUp2Upgrade();
		else if (upgradeName == "GreenMatterYieldUp3" || upgradeName == "EXPYieldUp3") contribution = multiplierToPercentage(getYieldFromYieldUp3Upgrade());
		else if (upgradeName == "GreenMatterYieldUp4" || upgradeName == "EXPYieldUp4") contribution = multiplierToPercentage(getYieldFromYieldUp4Upgrade());
		//else if (upgradeName == "CriticalChance1") contribution = 100 * getCriticalChanceFromCriticalChance1Upgrade();
		else if (upgradeName == "SpeedUp3") contribution = 100 * getYieldSpeedMultiplierFromSpeedUp3Upgrade();
		else if (upgradeName == "SpeedUp4") contribution = multiplierToPercentage(getYieldSpeedMultiplierFromSpeedUp4Upgrade());
		else if (upgradeName == "Support1") contribution = 100 * getAdjacencyYieldMultiplierFromSupport1Upgrade();
		else return 0;
	}
	/*
	// Multipliers, formatted as constants
	else {
		if (upgradeName == "GreenMatterYieldUp2" || upgradeName == "EXPYieldUp2") contribution = (getYieldFromYieldUp2Upgrade() - 1) * (1 + getConstantYield());
		else if (upgradeName == "GreenMatterYieldUp3" || upgradeName == "EXPYieldUp3") contribution = (getYieldFromYieldUp3Upgrade() - 1) * (1 + getConstantYield());
		else if (upgradeName == "GreenMatterYieldUp4" || upgradeName == "EXPYieldUp4") contribution = (getYieldFromYieldUp4Upgrade() - 1) * (1 + getConstantYield());
	}
	*/

	return contribution;
}

//////////////////////// SUMMATIONS ////////////////////////

/// Yield

BigReal Hexii::getConstantYield() const {
	return getYieldFromYieldUp1Upgrade();
}

BigReal Hexii::getAdditiveYieldMultiplier() const {
	return 1 + getYieldFromYieldUp2Upgrade();
}

BigReal Hexii::getMultiplicativeYieldMultiplier() const {
	return getYieldFromYieldUp3Upgrade() * getYieldFromYieldUp4Upgrade();
}

/// Yield speed

BigReal Hexii::getConstantYieldSpeed() const {
	return 0;
}

BigReal Hexii::getAdditiveYieldSpeedMultiplier() const {
	return 1 +
		getYieldSpeedMultiplierFromSpeedUp1Upgrade() +
		getYieldSpeedMultiplierFromSpeedUp2Upgrade() +
		getYieldSpeedMultiplierFromSpeedUp3Upgrade()
		;
}

BigReal Hexii::getMultiplicativeYieldSpeedMultiplier() const {
	return getYieldSpeedMultiplierFromSpeedUp4Upgrade();
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
