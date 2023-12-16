#include "ProgressionEvent.h"
#include "HexiiScene.h"
#include "HexiiInfoTab.h"
#include "EffectChain.h"
#include "CommonEffects.h"
#include "Resources.h"

HexiiFirstUpgradePurchasedProgressionEvent::HexiiFirstUpgradePurchasedProgressionEvent(
	HexiiUpgradesAvailableProgressionEvent* dependency, State initialState
) : ProgressionEvent(initialState)
{
	registerGenericDependency(dependency);
	
}

void HexiiFirstUpgradePurchasedProgressionEvent::init() {
	if (getUnresolvedDependencyCount() == 0) {
		if (getState() == State::IGNORING) progressToObserving();
		else followInitialState();
	}
}

void HexiiFirstUpgradePurchasedProgressionEvent::onProgressToObserving(bool didSkip) {
	if(didSkip) return;

	// Encourage the user to investigate the upgrades tab to see that they need to earn 120 green matter
	createUpgradeTabEncouragement()->getTop()->start();

	// Start listening for when the upgrade becomes affordable
	checkAffordable();
}

void HexiiFirstUpgradePurchasedProgressionEvent::onProgressToEncouraging(bool didSkip) {
	if(didSkip) return;

	// Stages of encouragement when enough green matter is available to purchase an upgrade
	HexiiInfoTab* hexInfoTab = HexiiScene::getDock()->getHexInfoTab();

	UpgradeBox* upgradeBox = dynamic_cast<HexiiUpgradesTab*>(HexiiScene::getDock()->getHexInfoTab()->getTab(1))->m_upgradeBoxes[0];

	//upgradeBox->m_purchaseUpgradeButton->setCascadeColorEnabled(true);
	//upgradeBox->m_purchaseUpgradeButton->setColor(cocos2d::Color3B::YELLOW);

	// Check that L0 hex is focused and the upgrade is affordable (it's possible for it to become unaffordable between the time the encouragement starts and the time it finishes)
	EffectChain::create(nullptr, [hexInfoTab]() {
		return hexInfoTab->m_focusHexii->getLayer() == 0 && Resources::getInstance()->getGreenMatter() >= 120;
		}, EffectChain::noEffect
	)
	->setNext(createUpgradeTabEncouragement())
	->setNext(CommonEffects::createEncouragementForTarget(
		upgradeBox->m_purchaseUpgradeButton,
		[this, upgradeBox]() {
			if (upgradeBox->m_upgradeState == Upgrade::State::OWNED) {
				// The encouragement remains the entire time, until the upgrade is purchased
				progressToEncouraged();
				return true;
			}
			else return false;
		},
		cocos2d::Color3B(200, 128, 255)
	))
	->getTop()
	->start();
}

void HexiiFirstUpgradePurchasedProgressionEvent::onProgressToEncouraged(bool didSkip) {
	if (didSkip) return;

	// As soon as the upgrade is purchased, we're done, and since we only move to Encouraged 
	// when the upgrade is purchased, we are immediately done here
	progressToAchieved();
}

void HexiiFirstUpgradePurchasedProgressionEvent::onProgressToAchieved()
{}

void HexiiFirstUpgradePurchasedProgressionEvent::checkAffordable() {
	if (Resources::getInstance()->getGreenMatter() >= 120) progressToEncouraging();
	else EventUtility::scheduleFunctionOnSelf(this, &HexiiFirstUpgradePurchasedProgressionEvent::checkAffordable);
}

EffectChainPtr HexiiFirstUpgradePurchasedProgressionEvent::createUpgradeTabEncouragement()
{
	Dock* dock = HexiiScene::getDock();
	HexiiInfoTab* hexInfoTab = dock->getHexInfoTab();	
	
	return 
		// Encourage the hexii info tab
		CommonEffects::createEncouragementForTab(dock, 0)
		// Encourage the upgrade tab
		->setNext(CommonEffects::createEncouragementForTab(hexInfoTab, 1));
}
