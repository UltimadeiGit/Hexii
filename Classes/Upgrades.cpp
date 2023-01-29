#include "Upgrades.h"
#include "Console.h"

USING_NS_CC;

Upgrades* Upgrades::m_instance = nullptr;

Upgrade::Upgrade(const fmt::string_view name, const fmt::string_view description, const fmt::string_view contributionDescription, BigReal greenMatterCost, BigInt unlockLevel)
	: name(name), description(description), contributionDescription(contributionDescription), greenMatterCost(greenMatterCost), unlockLevel(unlockLevel),
	icon(Director::getInstance()->getTextureCache()->addImage("icons/upgrades/" + fmt::to_string(name) + ".png"))
{}

Upgrades::Upgrades() {
	m_upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "YieldUp1" }, fmt::string_view{ "Increases yield based on level" }, fmt::string_view{"+{}"}, 120, 1));
	m_upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "StrongArm" }, fmt::string_view{ "Active bonus is 50% stronger" }, fmt::string_view{ "--" }, 150, 2));
	m_upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "SpeedUp1" }, fmt::string_view{ "Increases yield speed by +50%" }, fmt::string_view{ "--" }, 450, 3));
	m_upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "CriticalChance1" }, fmt::string_view{ "Increases critical chance based on level" }, fmt::string_view{ "+{}%" }, 1200, 6));
	m_upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "Discount" }, fmt::string_view{ "Reduces the cost to purchase exp" }, fmt::string_view{ "--" }, 1500, 9));
	m_upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "CriticalBonus1" }, fmt::string_view{ "Critical yields are twice as powerful" }, fmt::string_view{ "--" }, 6000, 12));
	m_upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "SpeedUp2" }, fmt::string_view{ "Increases yield speed by +2% per level" }, fmt::string_view{ "+{}%" }, 30000, 24));
	m_upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "YieldUp2" }, fmt::string_view{ "Multiplies yield by 1.03 each level" }, fmt::string_view{ "+{}% (+{})" }, 120000, 36));
	m_upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "Support" }, fmt::string_view{ "Gives adjacent hexii extra yield per level" }, fmt::string_view{ "+{}%" }, 210000, 48));
	m_upgrades.push_back(std::make_shared<Upgrade>(fmt::string_view{ "GlobalPower" }, fmt::string_view{ "Gives +100% yield to all hexii (additive)" }, fmt::string_view{ "--%" }, 1000000, 60));

	//if (m_upgrades.size() != Upgrades::UPGRADE_COUNT) err(std::format("Upgrades - Expected {} upgrades but got {} instead", UPGRADE_COUNT, m_upgrades.size()));
}

Upgrades* Upgrades::getInstance() {
	if (Upgrades::m_instance == nullptr) {
		Upgrades::m_instance = new Upgrades();
	}

	return Upgrades::m_instance;
}

const Upgrades::UpgradeList& Upgrades::getUpgrades() {
	return Upgrades::getInstance()->m_upgrades;
}

const Upgrades::UpgradeList Upgrades::getUpgradesBetweenLevels(BigInt lower, BigInt higher) {
	UpgradeList& fullList = Upgrades::getInstance()->m_upgrades;
	UpgradeList subList;

	if (higher < lower) return subList;

	for (unsigned int i = 0; i < fullList.size(); i++) {
		unsigned int unlockLevel = fullList[i]->unlockLevel;
		if (unlockLevel >= lower && unlockLevel <= higher) subList.push_back(fullList[i]);

		// No more upgrades will fall in the given range
		if (unlockLevel > higher) break;
	}

	return subList;
}

const Upgrades::UpgradeList Upgrades::getUpgradesFollowing(UpgradePtr pivot, BigInt count) {
	UpgradeList& fullList = Upgrades::getInstance()->m_upgrades;
	UpgradeList subList;
	
	if (count == 0) return subList;

	// Begun once pivot has been reached (if nullptr then begin immediately)
	bool begun = pivot == nullptr;
	
	for (unsigned int i = 0; i < fullList.size(); i++) {
		if (begun) {
			subList.push_back(fullList[i]);
			// Requested count has been reached
			if (subList.size() == count) break;
		}
		else if (pivot == fullList[i]) begun = true;
	}

	return subList;
}