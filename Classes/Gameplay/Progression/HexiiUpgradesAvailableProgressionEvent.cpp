#include "ProgressionEvent.h"
#include "HexiiPlane.h"

HexiiUpgradesAvailableProgressionEvent::HexiiUpgradesAvailableProgressionEvent(
	PurchaseFirstHexiiProgressionEvent* dependency, State initialState)
	: ProgressionEvent(initialState)
{
	registerGenericDependency(dependency);
}

void HexiiUpgradesAvailableProgressionEvent::onProgressToObserving(bool didSkip) {
	// If we skipped the intermediate state, we're done. No need to set up any listeners
	if (didSkip) return;

	// The only thing this event cares about is getting the hexii to level 1
	// So first check if the L0 hex is level 1 or greater already

	if (HexiiPlane::getInstance()->getHexiiAtPos(cocos2d::Vec2(0, 0))->getTotalLevel() >= 1) progressToAchieved();
	else registerConditionListener(
		EventUtility::addGlobalEventListenerFixedPriority(
			Hexii::EVENT_LEVEL_GAINED, 1, this, &HexiiUpgradesAvailableProgressionEvent::onLevelGained
		)
	);
}

void HexiiUpgradesAvailableProgressionEvent::onLevelGained(cocos2d::EventCustom* evnt) {
	printf("Level gain detected and used to progress upgrade availability.\n");

	// Any level gain is sufficient, so we're done
	progressToAchieved();
}
