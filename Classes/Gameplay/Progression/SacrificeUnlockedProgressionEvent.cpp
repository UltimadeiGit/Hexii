#include "ProgressionEvent.h"
#include "HexiiPlane.h"

SacrificeUnlockedProgressionEvent::SacrificeUnlockedProgressionEvent(State initialState, uint achievedCount)
	: RepeatableProgressionEvent(initialState, achievedCount) {}

void SacrificeUnlockedProgressionEvent::onProgressToObserving(bool didSkip) {
	// Check hex level 66 has been reached
	
	Hexii* l0Hexii = HexiiPlane::getInstance()->getHexiiAtPos({ 0, 0 });
	
	if(!progressFromLevel(l0Hexii->getTotalLevel())) {
		// Add listeners for l0 hex levelling

		registerConditionListener(EventUtility::addTargetedEventListenerFixedPriority(Hexii::EVENT_LEVEL_GAINED, 1, this, l0Hexii->eventID, &SacrificeUnlockedProgressionEvent::onHexiiLevelGained));
	}

}

void SacrificeUnlockedProgressionEvent::onHexiiLevelGained(cocos2d::EventCustom* evnt) {
	auto data = EventUtility::getEventData<Hexii::EventHexiiLevelGainedData>(evnt);
	progressFromLevel(data->subject->getTotalLevel());	
}

bool SacrificeUnlockedProgressionEvent::progressFromLevel(BigInt hexiiLevel) {
	if (hexiiLevel >= 66) {
		progressToAchieved();
	}
	else if (hexiiLevel >= 60) {
		progressToEncouraging();
	}
	else return false;

	return true;
}

void SacrificeUnlockedProgressionEvent::onProgressionReversed() {
}

void SacrificeUnlockedProgressionEvent::onProgressToEncouraging(bool didSkip) {
	if (didSkip) return;

	progressToEncouraged();
}

void SacrificeUnlockedProgressionEvent::onProgressToEncouraged(bool didSkip) {}

void SacrificeUnlockedProgressionEvent::onProgressToAchieved()
{}

