#pragma once

#include "Upgrade.h"
#include "cocos2d.h"
#include "Maths.h"

#include <memory>
#include <vector>

class UpgradePath;
typedef std::shared_ptr<UpgradePath> UpgradePathPtr;

// An UpgradePath is an immutable representation of a path (or sequence) of upgrades
class UpgradePath
{
public:
	typedef std::map<uint, UpgradePtr> UpgradeMap;
	typedef std::vector<UpgradePtr> UpgradeVec;
	typedef uint PathID;

private:	
	UpgradePath(const UpgradePath&) = delete;
	UpgradePath(UpgradePath&&) = delete;

public:
	UpgradePath(const UpgradeVec upgrades, const std::string& name, PathID id);

	static UpgradePathPtr getPathFromID(PathID id);
	static UpgradePathPtr getStandardL0Path();
	static UpgradePathPtr getStandardPath();

	inline const UpgradeVec& getUpgrades() const { return m_upgrades; }
	inline int getUpgradeCount() const { return m_upgradeCount; }
	inline std::string getName() const { return m_name; }
	inline PathID getID() const { return m_id; }

	// Returns the level that can be obtained with the given amount of exp along this path in the given layer
	//inline BigInt getLevelFromEXP(BigReal exp, uint layer) const { return m_levelFromEXPFunc(exp, layer); }
	// Returns the amount of exp required to amass the given level along this path in the given layer
	//inline BigReal getEXPRequiredForLevel(BigInt level, uint layer) const { return m_expRequiredForLevelFunc(level, layer); }

private:
	const UpgradeVec m_upgrades;
	const int m_upgradeCount;
	const std::string m_name;
	const PathID m_id;

	/*
	// arg: desired level count, layer
	const std::function<BigReal(BigInt, uint)> m_expRequiredForLevelFunc;
	// arg: exp, layer
	const std::function<BigInt(BigReal, uint)> m_levelFromEXPFunc;	
	*/

private: /// Static block
	// Every upgrade defined in the game
	static UpgradeMap* s_allUpgrades;
	// All upgrades in the standard L0 path
	static UpgradePathPtr s_standardL0Path;
	// All upgrades in the standard path
	static UpgradePathPtr s_standardPath;

	static void initAllUpgrades();

	/// EXP Functions

	//static BigReal expRequiredForLevelStandardPath(BigInt level, uint layer);
	//static BigInt levelFromEXPStandardPath(BigReal exp, uint layer);
};