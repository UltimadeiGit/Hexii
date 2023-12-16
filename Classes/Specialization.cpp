/*
#include "Specialization.h"
#include "JSON.hpp"
#include "Resources.h"
#include "cocos2d.h"
#include "Console.h"

USING_NS_CC;
using namespace nlohmann;

Specialization::Specialization(BoolMap* upgradesDst, SpecType specType, uint layer) :
	m_upgradesDst(upgradesDst),
	specType(specType),
	m_layer(layer)
{
	switch (specType) {
	case SpecType::STANDARD_L0:
		upgradePath = UpgradePath::getStandardL0Path();
	case SpecType::STANDARD:
		upgradePath = UpgradePath::getStandardPath();
		break;
	}

	init();
}

Specialization::Specialization(const nlohmann::json& json, BoolMap* upgradesDst, uint layer) :
	upgradePath(UpgradePath::getPathFromID(json.at("upgradePathID").get<int>())),
	m_upgradesDst(upgradesDst),
	specType(json.at("specType").get<SpecType>()),
	m_layer(layer)
{
	init();

	m_upgradeStates = json.at("upgradeStates").get<std::vector<UpgradeBox::State>>();
}

void Specialization::init() {
	m_upgradeStates.resize(upgradePath->getUpgradeCount());
	m_memoizedEXPForNextLevel = upgradePath->getEXPRequiredForLevel(m_level + 1, m_layer);
}

std::string Specialization::getSpecName() const {
	switch (specType) {
		case SpecType::STANDARD_L0:
			return "StandardL0";
		case SpecType::STANDARD:
			return "Standard";
	}
}

std::string Specialization::getSpecNameFriendly() {
	switch (specType) {
		case SpecType::STANDARD_L0:
		case SpecType::STANDARD:
			return "Standard";
	}
}

void Specialization::unlockUpgrade(UpgradePtr upgrade) {
	auto& validUpgrades = upgradePath->getUpgrades();
	int index = -1;
	std::string upgradeName = fmt::to_string(upgrade->name);

	for (uint i = 0; i < validUpgrades.size(); i++) {
		if (validUpgrades[i] == upgrade) {
			index = i;
			break;
		}
	}

	if (index == -1) err("Upgrade " + upgradeName + " is not a valid upgrade for " + getSpecNameFriendly() + "s.");

	(*m_upgradesDst)[upgradeName] = true;
	m_upgradeStates[index] = UpgradeBox::State::PURCHASED;

	if (upgrade->actsOnFlags & Upgrade::ACTS_ON_OTHER_HEXII) {
		if(upgrade->name == "GlobalPower") Resources::getInstance()->addglobalPowerUpgradeBonus();
		EventUtility::dispatchEvent(EVENT_NONLOCAL_UPGRADE_PURCHASED, eventID, EventUpgradePurchasedData{ this, upgrade });
	}

	EventUtility::dispatchEvent(EVENT_UPGRADE_PURCHASED, eventID, EventUpgradePurchasedData{ this, upgrade });
}



void to_json(json& j, const Specialization& specGroup) {
	// TODO: Check if this works
	j = json{
		{"specType", specGroup.specType},
		{"upgradePathID", specGroup.upgradePath->getID()},
		{"upgradeStates", specGroup.getUpgradeStates()}
	};
}
*/