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

        upgrades.push_back(s_allUpgrades->find(0)->second); // GM Yield Up 1
        upgrades.push_back(s_allUpgrades->find(6)->second); // Strong Arm
        upgrades.push_back(s_allUpgrades->find(2)->second); // Speed Up 1
        upgrades.push_back(s_allUpgrades->find(4)->second); // Critical Chance 1
        upgrades.push_back(s_allUpgrades->find(7)->second); // Discount
        upgrades.push_back(s_allUpgrades->find(5)->second); // Critical Bonus 1
        upgrades.push_back(s_allUpgrades->find(3)->second); // Speed Up 2
        upgrades.push_back(s_allUpgrades->find(1)->second); // GM Yield Up 2
        upgrades.push_back(s_allUpgrades->find(8)->second); // Support 1
        upgrades.push_back(s_allUpgrades->find(9)->second); // Global Power

        s_standardL0Path = std::make_shared<UpgradePath>(upgrades, "StandardL0Path", 0);
    }

    return s_standardL0Path;
}

UpgradePathPtr UpgradePath::getStandardPath()
{
    initAllUpgrades();

    if (s_standardPath == nullptr) {
        UpgradeVec upgrades;

        upgrades.push_back(s_allUpgrades->find(10)->second); // EXP Yield Up 1
        upgrades.push_back(s_allUpgrades->find(6)->second); // Strong Arm
        upgrades.push_back(s_allUpgrades->find(2)->second); // Speed Up 1
        upgrades.push_back(s_allUpgrades->find(4)->second); // Critical Chance 1
        upgrades.push_back(s_allUpgrades->find(7)->second); // Discount
        upgrades.push_back(s_allUpgrades->find(5)->second); // Critical Bonus 1
        upgrades.push_back(s_allUpgrades->find(3)->second); // Speed Up 2
        upgrades.push_back(s_allUpgrades->find(11)->second); // EXP Yield Up 2
        upgrades.push_back(s_allUpgrades->find(8)->second); // Support 1
        upgrades.push_back(s_allUpgrades->find(9)->second); // Global Power

        s_standardPath = std::make_shared<UpgradePath>(upgrades, "StandardPath", 1);
    }

    return s_standardPath;
}

void UpgradePath::initAllUpgrades(){
    if (s_allUpgrades != nullptr) return;
    s_allUpgrades = new UpgradeMap;

    // Note: I've switched to an ID system to change how upgrades are loaded.
    // This is because I want to be able to change the names of upgrades without breaking save files.
    // The save file will be storing, not the id, but the index of the upgrade in the upgrade path, per upgrade path
    // e.g if the first upgrade in the path is "Yield Up I", then the save file will store 0 for that upgrade

    // TODO: Later this can be optimized by having all upgrades be adjacent in memory


    // GM yield up and EXP yield up are different only because they have different icons
    s_allUpgrades->emplace(std::make_pair(0, 
        std::make_shared<Upgrade>(fmt::string_view{"GreenMatterYieldUp1"}, fmt::string_view{"Yield Up I"}, fmt::string_view{"Increases yield based on level count"}, fmt::string_view{"+{}"}, 120, 1, Upgrade::ACTS_ON_YIELD)));

    s_allUpgrades->emplace(std::make_pair(1,
        std::make_shared<Upgrade>(fmt::string_view{ "GreenMatterYieldUp2" }, fmt::string_view{ "Yield Up II" }, fmt::string_view{ "Multiplies yield by 1.03 each level" }, fmt::string_view{ "+{}% (+{})" }, 120000, 36, Upgrade::ACTS_ON_YIELD)));

    s_allUpgrades->emplace(std::make_pair(10,
        std::make_shared<Upgrade>(fmt::string_view{ "EXPYieldUp1" }, fmt::string_view{ "Yield Up I" }, fmt::string_view{ "Increases yield based on level count" }, fmt::string_view{ "+{}" }, 120, 1, Upgrade::ACTS_ON_YIELD)));

    s_allUpgrades->emplace(std::make_pair(11,
        std::make_shared<Upgrade>(fmt::string_view{ "EXPYieldUp2" }, fmt::string_view{ "Yield Up II" }, fmt::string_view{ "Multiplies yield by 1.03 each level" }, fmt::string_view{ "+{}% (+{})" }, 120000, 36, Upgrade::ACTS_ON_YIELD)));

    s_allUpgrades->emplace(std::make_pair(2,
        std::make_shared<Upgrade>(fmt::string_view{ "SpeedUp1" }, fmt::string_view{ "Speed Up I" }, fmt::string_view{ "Increases yield speed by +50%" }, fmt::string_view{ "--" }, 450, 3, Upgrade::ACTS_ON_SPEED)));

    s_allUpgrades->emplace(std::make_pair(3,
        std::make_shared<Upgrade>(fmt::string_view{ "SpeedUp2" }, fmt::string_view{ "Speed Up II" }, fmt::string_view{ "Increases yield speed by +2% per level" }, fmt::string_view{ "+{}%" }, 30000, 24, Upgrade::ACTS_ON_SPEED)));

    s_allUpgrades->emplace(std::make_pair(4,
        std::make_shared<Upgrade>(fmt::string_view{ "CriticalChance1" }, fmt::string_view{ "Critical Chance I" }, fmt::string_view{ "Increases critical chance based on level" }, fmt::string_view{ "+{}%" }, 1200, 6, Upgrade::ACTS_ON_CRITS)));

    s_allUpgrades->emplace(std::make_pair(5,
        std::make_shared<Upgrade>(fmt::string_view{ "CriticalBonus1" }, fmt::string_view{ "Critical Bonus I" }, fmt::string_view{ "Critical yields are twice as powerful" }, fmt::string_view{ "--" }, 6000, 12, Upgrade::ACTS_ON_CRITS)));

    s_allUpgrades->emplace(std::make_pair(6,
        std::make_shared<Upgrade>(fmt::string_view{ "StrongArm" }, fmt::string_view{ "Strong Arm" }, fmt::string_view{ "Active bonus is 50% stronger" }, fmt::string_view{ "--" }, 150, 2, Upgrade::ACTS_ON_SPEED)));

    s_allUpgrades->emplace(std::make_pair(7,
        std::make_shared<Upgrade>(fmt::string_view{ "Discount" }, fmt::string_view{ "Discount" }, fmt::string_view{ "Reduces the cost to purchase EXP" }, fmt::string_view{ "--" }, 1500, 9, 0)));

    s_allUpgrades->emplace(std::make_pair(8,
        std::make_shared<Upgrade>(fmt::string_view{ "Support1" }, fmt::string_view{ "Support I" }, fmt::string_view{ "Gives adjacent hexii extra yield per level" }, fmt::string_view{ "+{}%" }, 210000, 48, Upgrade::ACTS_ON_OTHER_HEXII | Upgrade::ACTS_ON_YIELD)));

    s_allUpgrades->emplace(std::make_pair(9,
        std::make_shared<Upgrade>(fmt::string_view{ "GlobalPower" }, fmt::string_view{ "Global Power" }, fmt::string_view{ "Gives +100% yield to all hexii" }, fmt::string_view{ "--" }, 1000000, 60, Upgrade::ACTS_ON_OTHER_HEXII | Upgrade::ACTS_ON_YIELD)));
}
