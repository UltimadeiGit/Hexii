#include "Upgrade.h"
#include "Console.h"

USING_NS_CC;

Upgrade::Upgrade(
	const char* name, 
	const char* friendlyName,
	const char* description, 
	const char* contributionDescription,
	BigReal greenMatterCost, 
	BigInt levelRequirement, 
	long long actsOnFlags,
	UpgradeID id
)
	: name(name), 
	friendlyName(friendlyName),
	description(description), 
	contributionDescription(contributionDescription), 
	greenMatterCost(greenMatterCost), 
	levelRequirement(levelRequirement),
	actsOnFlags(actsOnFlags),
	id(id),
	icon(Director::getInstance()->getTextureCache()->addImage("icons/upgrades/" + fmt::to_string(name) + ".png"))
{}