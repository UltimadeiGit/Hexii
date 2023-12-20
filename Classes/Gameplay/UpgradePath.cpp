#include "UpgradePath.h"

#include "Console.h"

UpgradePath::UpgradeMap* UpgradePath::s_allUpgrades = nullptr;
UpgradePathPtr UpgradePath::s_standardL0Path = nullptr;
UpgradePathPtr UpgradePath::s_standardPath = nullptr;

UpgradePath::UpgradePath(const UpgradeVec upgrades, const std::string& name, PathID id) :
	m_upgrades(upgrades), 
	m_upgradeCount(upgrades.size()), 
	m_name(name),
    m_id(id)
{}

UpgradePathPtr UpgradePath::getPathFromID(PathID id)
{
    switch (id) {
    case 0:
        return getStandardL0Path();
    case 1:
        return getStandardPath();
    default:
        break;
    }

    err("Invalid upgrade path id: " + id);

    return nullptr;
}

UpgradePathPtr UpgradePath::getStandardL0Path()
{
    initAllUpgrades();

    if (s_standardL0Path == nullptr) {
        UpgradeVec upgrades;

        // Green Matter Yield Up 1 ~ 4
        // Speed Up 1 ~ 4
        // Criticals
        // Strong Arm
        // Discount
        // Support 1
        // Global Power
        addUpgradesToVec(upgrades, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 16, 17 });

        s_standardL0Path = std::make_shared<UpgradePath>(upgrades, "StandardL0Path", 0);
    }

    return s_standardL0Path;
}

UpgradePathPtr UpgradePath::getStandardPath()
{
    initAllUpgrades();

    if (s_standardPath == nullptr) {
        UpgradeVec upgrades;

        // EXP Yield Up 1 ~ 4
        // Speed Up 1 ~ 4
        // Criticals
        // Strong Arm
        // Discount
        // Support 1
        // Global Power
        addUpgradesToVec(upgrades, { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 16, 17 });

        s_standardPath = std::make_shared<UpgradePath>(upgrades, "StandardPath", 1);
    }

    return s_standardPath;
}

/*
UpgradePath::UpgradeVec UpgradePath::sortVecByLevelRequirements(const UpgradeVec& upgrades)
{
    // Make a copy of the vector to sort
    UpgradeVec sorted = upgrades;

    // Use std::sort with a custom comparator
    std::sort(sorted.begin(), sorted.end(),
        [](const UpgradePtr& a, const UpgradePtr& b) {
            return a->levelRequirement < b->levelRequirement;
        }
    );

    return sorted;
}
*/

void UpgradePath::initAllUpgrades(){
    if (s_allUpgrades != nullptr) return;
    s_allUpgrades = new UpgradeMap;

    // Note: I've switched to an ID system to change how upgrades are loaded.
    // This is because I want to be able to change the names of upgrades without breaking save files.
    // The save file will be storing, not the id, but the index of the upgrade in the upgrade path, per upgrade path
    // e.g if the first upgrade in the path is "Yield Up I", then the save file will store 0 for that upgrade

    // TODO: Later this can be optimized by having all upgrades be adjacent in memory

    // Yield up
    // GM yield up and EXP yield up are different only because they have different icons
    // Existing initUpgrade calls
    initUpgrade(std::make_shared<Upgrade>("GreenMatterYieldUp1", "Yield Up I", "Increases yield based on level", "+{}", 120, 1, Upgrade::ACTS_ON_YIELD, 0));
    initUpgrade(std::make_shared<Upgrade>("GreenMatterYieldUp2", "Yield Up II", "Increases yield by +100%", "+100%", 9000, 12, Upgrade::ACTS_ON_YIELD, 1));
    initUpgrade(std::make_shared<Upgrade>("GreenMatterYieldUp3", "Yield Up III", "Multiplies yield by 1.03 per level", "+{}%", 60000, 36, Upgrade::ACTS_ON_YIELD, 12));
    initUpgrade(std::make_shared<Upgrade>("GreenMatterYieldUp4", "Yield Up IV", "Multiplies yield by 1.1 per level after 60", "+{}%", 3000000, 62, Upgrade::ACTS_ON_YIELD, 13));
    
    initUpgrade(std::make_shared<Upgrade>("EXPYieldUp1", "Yield Up I", "Increases yield based on level. Stronger in higher layers", "+{}", 120, 1, Upgrade::ACTS_ON_YIELD, 10));
    initUpgrade(std::make_shared<Upgrade>("EXPYieldUp2", "Yield Up II", "Increases yield by +100%", "+100%", 9000, 12, Upgrade::ACTS_ON_YIELD, 11));
    initUpgrade(std::make_shared<Upgrade>("EXPYieldUp3", "Yield Up III", "Multiplies yield by 1.03 per level", "+{}%", 60000, 36, Upgrade::ACTS_ON_YIELD, 14));
    initUpgrade(std::make_shared<Upgrade>("EXPYieldUp4", "Yield Up IV", "Multiplies yield by 1.1 per level after 60", "+{}%", 3000000, 62, Upgrade::ACTS_ON_YIELD, 15));

    // Speed up
    initUpgrade(std::make_shared<Upgrade>("SpeedUp1", "Speed Up I", "Increases yield speed by +50%", "+50%", 450, 3, Upgrade::ACTS_ON_SPEED, 2));
    initUpgrade(std::make_shared<Upgrade>("SpeedUp2", "Speed Up II", "Increases yield speed by +50%", "+50%", 24000, 24, Upgrade::ACTS_ON_SPEED, 3));
    initUpgrade(std::make_shared<Upgrade>("SpeedUp3", "Speed Up III", "Increases yield speed by +1% per level", "+{}%", 36000, 30, Upgrade::ACTS_ON_SPEED, 16));
    initUpgrade(std::make_shared<Upgrade>("SpeedUp4", "Speed Up IV", "Multiplies yield speed by 1.01 per level after 60", "+{}%", 6000000, 64, Upgrade::ACTS_ON_SPEED, 17));

    // Crits
    initUpgrade(std::make_shared<Upgrade>("CriticalChance1", "Critical Chance I", "Adds a 5% chance to produce critical yields (6x regular yield)", "+5%", 1200, 6, Upgrade::ACTS_ON_CRITS, 4));
    initUpgrade(std::make_shared<Upgrade>("CriticalBonus1", "Critical Bonus I", "Critical yields are twice as powerful", "6x -> 12x", 1200, 6, Upgrade::ACTS_ON_CRITS, 5));
    
    // Misc
    initUpgrade(std::make_shared<Upgrade>("Discount", "Discount", "Reduces the cost to purchase EXP", "", 300, 9, 0, 7));
    initUpgrade(std::make_shared<Upgrade>("StrongArm", "Strong Arm", "Active bonus is 50% stronger", "", 150, 2, Upgrade::ACTS_ON_SPEED, 6));
    
    // Support
    initUpgrade(std::make_shared<Upgrade>("Support1", "Support I", "Improves adjacent hexii's yield based on this hexii's level and layer", "+{}%", 210000, 48, Upgrade::ACTS_ON_OTHER_HEXII | Upgrade::ACTS_ON_YIELD, 8));
    initUpgrade(std::make_shared<Upgrade>("GlobalPower", "Global Power", "Gives +100% yield to all hexii", "+100%", 1000000, 60, Upgrade::ACTS_ON_OTHER_HEXII | Upgrade::ACTS_ON_YIELD, 9));
}
