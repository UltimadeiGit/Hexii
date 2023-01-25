#include "Upgrades.h"
#include "Console.h"

USING_NS_CC;

Upgrades* Upgrades::m_instance = nullptr;

Upgrade::Upgrade(const std::string_view name, const std::string_view description, BigReal greenMatterCost, BigInt unlockLevel)
	: name(name), description(description), greenMatterCost(greenMatterCost), unlockLevel(unlockLevel),
	icon(Director::getInstance()->getTextureCache()->addImage("icons/upgrades/" + std::string(name) + ".png"))
{}

Upgrades::Upgrades() {
	m_upgrades.push_back(std::make_shared<Upgrade>(std::string_view{ "YieldUp1" }, std::string_view{ "Increases yield by +0.5 per level" }, 30, 1));
	m_upgrades.push_back(std::make_shared<Upgrade>(std::string_view{ "SpeedUp1" }, std::string_view{ "Increases yield speed by +25%" }, 120, 3));
	m_upgrades.push_back(std::make_shared<Upgrade>(std::string_view{ "StrengthToStrength" }, std::string_view{ "Clicking on this hex provides it 1 EXP" }, 600, 6));
	m_upgrades.push_back(std::make_shared<Upgrade>(std::string_view{ "YieldUp2" }, std::string_view{ "Increases yield by +5% per level" }, 900, 12));
	m_upgrades.push_back(std::make_shared<Upgrade>(std::string_view{ "SpeedUp2" }, std::string_view{ "Increases yield speed by a further +25%" }, 3000, 18));

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

const Upgrades::UpgradeList Upgrades::getUpgradesBetweenLevels(uint lower, uint higher) {
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

const Upgrades::UpgradeList Upgrades::getUpgradesFollowing(UpgradePtr pivot, uint count) {
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