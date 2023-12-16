#include "ProgressionEvent.h"
#include "HexiiPlane.h"

HexiiUpgradesAvailableProgressionEvent::HexiiUpgradesAvailableProgressionEvent(
	PurchaseFirstHexiiProgressionEvent* dependency, State initialState)
	: ProgressionEvent(initialState)
{
	registerGenericDependency(dependency);
}

void HexiiUpgradesAvailableProgressionEvent::init() {
	// Register the dependency. If the dependency is not needed / already fulfilled, 
	// then this will return false and we can progress to the appropriate state
	if (getUnresolvedDependencyCount() == 0) {
		if (getState() == State::IGNORING) progressToObserving();
		else followInitialState();
	}
}

void HexiiUpgradesAvailableProgressionEvent::onProgressToObserving(bool didSkip) {
	// If we skipped the intermediate state, we're done. No need to set up any listeners
	if (didSkip) return;

	// The only thing this event cares about is getting the hexii to level 1
	registerConditionListener(
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
