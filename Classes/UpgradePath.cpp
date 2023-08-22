#include "UpgradePath.h"

#include "Console.h"

UpgradePath* UpgradePath::m_workerPath = nullptr;

UpgradePath::UpgradePath(const UpgradeVec upgrades, const std::string& name) : 
	m_upgrades(upgrades), 
	m_upgradeCount(upgrades.size()), 
	m_name(name)
{}

UpgradePath* UpgradePath::getPathFromName(const std::string& name)
{
    if(name == "WorkerPath") return getWorkerPath();

    err("Invalid upgrade path name: " + name);

    return nullptr;
}

UpgradePath* UpgradePath::getWorkerPath()
{
    if (m_workerPath == nullptr) {
        UpgradeVec upgrades;

        upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "ImprovedHandlingI" }, fmt::string_view{ "Increases yield based on bee count" }, fmt::string_view{ "+{}" }, 120, 1, Upgrade::ACTS_ON_YIELD));
        upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "QueensPresence" }, fmt::string_view{ "Active bonus is 50% stronger" }, fmt::string_view{ "--" }, 150, 2, 0));
        upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "ImprovedFlightI" }, fmt::string_view{ "Increases yield speed by +50%" }, fmt::string_view{ "--" }, 450, 3, Upgrade::ACTS_ON_SPEED));
        upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "BlossomBoostI" }, fmt::string_view{ "Increases critical chance based on beeCount" }, fmt::string_view{ "+{}%" }, 1200, 6, Upgrade::ACTS_ON_CRITS));
        upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "RoyalAffluence" }, fmt::string_view{ "Reduces the cost to purchase XB" }, fmt::string_view{ "--" }, 1500, 9, 0));
        upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "BlossomBoostII" }, fmt::string_view{ "Critical yields are twice as powerful" }, fmt::string_view{ "--" }, 6000, 12, Upgrade::ACTS_ON_CRITS));
        upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "ImprovedFlightII" }, fmt::string_view{ "Increases yield speed by +2% per beeCount" }, fmt::string_view{ "+{}%" }, 30000, 24, Upgrade::ACTS_ON_SPEED));
        upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "ImprovedHandlingII" }, fmt::string_view{ "Multiplies yield by 1.03 each beeCount" }, fmt::string_view{ "+{}% (+{})" }, 120000, 36, Upgrade::ACTS_ON_YIELD));
        upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "UnityI" }, fmt::string_view{ "Gives adjacent hexii extra yield per beeCount" }, fmt::string_view{ "+{}%" }, 210000, 48, Upgrade::ACTS_ON_OTHER_HEXII | Upgrade::ACTS_ON_YIELD));
        upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "UnityII" }, fmt::string_view{ "Gives +100% yield to all hexii" }, fmt::string_view{ "--" }, 1000000, 60, Upgrade::ACTS_ON_OTHER_HEXII | Upgrade::ACTS_ON_YIELD));

        m_workerPath = new UpgradePath(upgrades, "WorkerPath");

        m_workerPath->m_expRequiredForBeesFunc = UpgradePath::expRequiredForBeesWorker;
        m_workerPath->m_beeCountFromEXPFunc = UpgradePath::beeCountFromEXPWorker;
    }

    return m_workerPath;
}

BigReal UpgradePath::expRequiredForBeesWorker(BigInt beeCount, uint district) {
    // = A(b^beeCount - 1) where \
	    b = (1.2 + district/50) \
	    A = 6b / (b - 1)

        // First is hardcoded
    if (beeCount == 0) return 0;
    beeCount -= 1;

    // Derived from EXP required at `beeCount` to gain a bee, which is simply 6 * b^beeCount

    const BigReal base = 1.2 + (district * 0.02);
    return std::ceill(6 + ((6 * base) / (base - 1)) * (std::powl(base, beeCount) - 1));
}

BigInt UpgradePath::beeCountFromEXPWorker(BigReal exp, uint district) {
    // = log_b(exp/A + 1) 
    // Inverse function of expRequiredForBeesFunc

    // First is hardcoded
    if (exp < 6) return 0;
    exp -= 6;

    const BigReal base = 1.2 + (district * 0.02);
    const BigReal operand = 1 + exp / ((6 * base) / (base - 1));
    return std::floorl(1 + std::logl(operand) / std::logl(base));
}
