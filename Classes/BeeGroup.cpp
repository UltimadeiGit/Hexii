#include "BeeGroup.h"
#include "JSON.hpp"
#include "Resources.h"
#include "cocos2d.h"
#include "Console.h"

USING_NS_CC;
using namespace nlohmann;

BeeGroup::BeeGroup(UpgradePath* upgradePath, BoolMap* upgradesDst, BeeType beeType, uint district) :
	upgradePath(upgradePath), 
	m_upgradesDst(upgradesDst),
	beeType(beeType),
	m_district(district)
{
	init();
}

BeeGroup::BeeGroup(const nlohmann::json& json, BoolMap* upgradesDst, uint district) :
	upgradePath(UpgradePath::getPathFromName(json.at("upgradePathName").get<std::string>())),
	m_upgradesDst(upgradesDst),
	beeType(json.at("beeType").get<BeeType>()),
	m_district(district)
{
	init();

	m_upgradeStates = json.at("upgradeStates").get<std::vector<BeeGroupUpgradeBox::State>>();
}

void BeeGroup::init() {
	m_upgradeStates.resize(upgradePath->getUpgradeCount());
	m_memoizedEXPForNextBee = upgradePath->getEXPRequiredForBees(m_beeCount + 1, m_district);
}

std::string BeeGroup::getBeeName() const {
	switch (beeType) {
		case BeeType::WORKER:
			return "WorkerBee";
		case BeeType::DRONE:
			return "Drone";
	}
}

std::string BeeGroup::getBeeNameFriendly() {
	switch (beeType) {
		case BeeType::WORKER:
			return "Worker Bee";
		case BeeType::DRONE:
			return "Drone";
	}
}

BigInt BeeGroup::updateBeeCount(BigReal exp, bool suppressEvent) {
	if (exp < m_memoizedEXPForNextBee) return 0;

	BigInt beeCount = upgradePath->getBeeCountFromEXP(exp, m_district);
	BigInt change = beeCount - m_beeCount;

	if (change == 0) return change;
	
	/// Update state

	m_beeCount = beeCount;
	m_memoizedEXPForNextBee = upgradePath->getEXPRequiredForBees(m_beeCount + 1, m_district);

	/// Check upgrade states

	// Upgrades that changed state as a result of the bee gain
	std::vector<UpgradePtr> changedStateUpgrades;
	// The new states of the changed upgrades (in the same order as changedStateUpgrades)
	std::vector<BeeGroupUpgradeBox::State> changedStates;

	auto& validUpgrades = upgradePath->getUpgrades();
	for (uint i = 0; i < validUpgrades.size(); i++) {
		auto& upgrade = validUpgrades[i];

		if (upgrade->beeRequirement <= m_beeCount && m_upgradeStates[i] == BeeGroupUpgradeBox::State::LOCKED) {
			changedStateUpgrades.push_back(upgrade);
			changedStates.push_back(BeeGroupUpgradeBox::State::REVEALED);
			m_upgradeStates[i] = BeeGroupUpgradeBox::State::REVEALED;
		}
	}

	/// Dispatch events

	if(!suppressEvent)
		EventUtility::dispatchEvent(EVENT_BEE_GAINED, eventID, EventBeeGainedData{ this, change, changedStateUpgrades, changedStates });

	return change;
}

void BeeGroup::unlockUpgrade(UpgradePtr upgrade) {
	auto& validUpgrades = upgradePath->getUpgrades();
	int index = -1;
	std::string upgradeName = fmt::to_string(upgrade->name);

	for (uint i = 0; i < validUpgrades.size(); i++) {
		if (validUpgrades[i] == upgrade) {
			index = i;
			break;
		}
	}

	if (index == -1) err("Upgrade " + upgradeName + " is not a valid upgrade for " + getBeeNameFriendly() + "s.");

	(*m_upgradesDst)[upgradeName] = true;
	m_upgradeStates[index] = BeeGroupUpgradeBox::State::PURCHASED;

	if (upgrade->actsOnFlags & Upgrade::ACTS_ON_OTHER_HEXII) {
		if(upgrade->name == "Unity2") Resources::getInstance()->addUnity2UpgradeBonus();
		EventUtility::dispatchEvent(EVENT_NONLOCAL_UPGRADE_PURCHASED, eventID, EventUpgradePurchasedData{ this, upgrade });
	}

	EventUtility::dispatchEvent(EVENT_UPGRADE_PURCHASED, eventID, EventUpgradePurchasedData{ this, upgrade });
}

BigReal BeeGroup::getUpgradePurchaseCostMultiplier() const {
	// Multiplier = 30 ^ district
	if (m_district == 0) return 1;
	return std::powl(30, m_district);
}

void to_json(json& j, const BeeGroup& beeGroup) {
	// TODO: Check if this works
	j = json{
		{"beeType", beeGroup.beeType},
		{"upgradePathName", beeGroup.upgradePath->getName()},
		{"upgradeStates", beeGroup.getUpgradeStates()}
	};
}
