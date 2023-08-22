#pragma once

#include "Upgrade.h"
#include "cocos2d.h"
#include "Maths.h"
#include "BeeGroupUpgradeBox.h"
#include <vector>

// Defines a path of upgrades
class UpgradePath
{
public:
	typedef std::vector<UpgradePtr> UpgradeVec;

private:
	UpgradePath(const UpgradeVec upgrades, const std::string& name);
	UpgradePath(const UpgradePath&) = delete;
	UpgradePath(UpgradePath&&) = delete;

public:
	static UpgradePath* getPathFromName(const std::string& name);
	static UpgradePath* getWorkerPath();

	inline const UpgradeVec& getUpgrades() const { return m_upgrades; }
	inline int getUpgradeCount() const { return m_upgradeCount; }
	inline std::string getName() const { return m_name; }

	// Returns the number of bees that can be obtained with the given amount of exp along this path in the given district
	inline BigInt getBeeCountFromEXP(BigReal exp, uint district) const { return m_beeCountFromEXPFunc(exp, district); }
	// Returns the amount of exp required to amass the given number of bees along this path in the given district
	inline BigReal getEXPRequiredForBees(BigInt beeCount, uint district) const { return m_expRequiredForBeesFunc(beeCount, district); }

private:
	static UpgradePath* m_workerPath;

	const UpgradeVec m_upgrades;
	const int m_upgradeCount;
	const std::string m_name;

	// arg: desired bee count, district
	std::function<BigReal(BigInt, uint)> m_expRequiredForBeesFunc;
	// arg: exp, district
	std::function<BigInt(BigReal, uint)> m_beeCountFromEXPFunc;

	/// EXP Functions

	static BigReal expRequiredForBeesWorker(BigInt beeCount, uint district);
	static BigInt beeCountFromEXPWorker(BigReal exp, uint district);
};