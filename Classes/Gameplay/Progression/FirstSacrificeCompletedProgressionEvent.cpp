#include "ProgressionEvent.h"
#include "Resources.h"

FirstSacrificeCompletedProgressionEvent::FirstSacrificeCompletedProgressionEvent(SacrificeUnlockedProgressionEvent* dependency, State initialState)
	: ProgressionEvent(initialState)
{
	registerGenericDependency(dependency);
}

void FirstSacrificeCompletedProgressionEvent::onProgressToObserving(bool didSkip) {
	if (didSkip) return;

	// Check if a sacrifice has been performed before
	if(Resources::getInstance()->getSacrificeCount() > 0) {
		progressToAchieved();
	}
	else {
		// Listen for a sacrifice to be performed
		registerConditionListener(EventUtility::addGlobalEventListenerFixedPriority(GameplayCommon::GameEvent::EVENT_SACRIFICE_CONFIRMED, 1, this, &FirstSacrificeCompletedProgressionEvent::onSacrificeCompleted));
	}

	// TODO: Add popup to explain sacrifice
	// No encouragement for this event
	progressToEncouraged();
}

void FirstSacrificeCompletedProgressionEvent::onProgressToEncouraging(bool didSkip)
{}

void FirstSacrificeCompletedProgressionEvent::onProgressToEncouraged(bool didSkip)
{}

void FirstSacrificeCompletedProgressionEvent::onSacrificeCompleted(cocos2d::EventCustom* evnt) {
	progressToAchieved();
}

void FirstSacrificeCompletedProgressionEvent::onProgressToAchieved()
{}
