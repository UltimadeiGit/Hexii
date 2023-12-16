#include "ProgressionEvent.h"

ProgressionEvent::ProgressionEvent(State initialState)
	: m_state(initialState)
{}

void ProgressionEvent::progressToObserving() {
	State previousState = m_state;
	m_state = State::OBSERVING;

	// Dispatch events for each state change that's occuring
	switch (previousState) {
	case State::IGNORING:
		onProgressToObserving(false);
		EventUtility::dispatchEvent(ProgressionEvent::EVENT_PROGRESSION_TO_OBSERVING, eventID, EventProgressionStateChange{this});
		break;
	default:
		return;
	}
}

void ProgressionEvent::progressToEncouraging() {
	State previousState = m_state;
	m_state = State::ENCOURAGING;

	// Dispatch events for each state change that's occuring
	switch (previousState) {
	case State::IGNORING:
		onProgressToObserving(true);
		EventUtility::dispatchEvent(ProgressionEvent::EVENT_PROGRESSION_TO_OBSERVING, eventID);
	case State::OBSERVING:
		onProgressToEncouraging(false);
		EventUtility::dispatchEvent(ProgressionEvent::EVENT_PROGRESSION_TO_ENCOURAGING, eventID, EventProgressionStateChange{this});
		break;
	default:
		return;
	}
}

void ProgressionEvent::progressToEncouraged() {
	State previousState = m_state;
	m_state = State::ENCOURAGED;

	// Dispatch events for each state change that's occuring
	switch (previousState) {
	case State::IGNORING:
		onProgressToObserving(true);
		EventUtility::dispatchEvent(ProgressionEvent::EVENT_PROGRESSION_TO_OBSERVING, eventID, EventProgressionStateChange{this});
	case State::OBSERVING:
		onProgressToEncouraging(true);
		EventUtility::dispatchEvent(ProgressionEvent::EVENT_PROGRESSION_TO_ENCOURAGING, eventID, EventProgressionStateChange{this});
	case State::ENCOURAGING:
		onProgressToEncouraged(false);
		EventUtility::dispatchEvent(ProgressionEvent::EVENT_PROGRESSION_TO_ENCOURAGED, eventID, EventProgressionStateChange{this});
		break;
	default:
		return;
	}
}

void ProgressionEvent::progressToAchieved() {
	State previousState = m_state;
	m_state = State::ACHIEVED;
	clearConditionListeners();

	// Dispatch events for each state change that's occuring
	switch (previousState) {
	case State::IGNORING:
		onProgressToObserving(true);
		EventUtility::dispatchEvent(ProgressionEvent::EVENT_PROGRESSION_TO_OBSERVING, eventID, EventProgressionStateChange{this});
	case State::OBSERVING:
		onProgressToEncouraging(true);
		EventUtility::dispatchEvent(ProgressionEvent::EVENT_PROGRESSION_TO_ENCOURAGING, eventID, EventProgressionStateChange{this});
	case State::ENCOURAGING:
		onProgressToEncouraged(true);
		EventUtility::dispatchEvent(ProgressionEvent::EVENT_PROGRESSION_TO_ENCOURAGED, eventID, EventProgressionStateChange{this});
	case State::ENCOURAGED:
		onProgressToAchieved();
		EventUtility::dispatchEvent(ProgressionEvent::EVENT_PROGRESSION_TO_ACHIEVED, eventID, EventProgressionStateChange{this});
		break;
	default:
		return;
	}
}

void ProgressionEvent::followInitialState() {
	// Dispatch events for each state change that's occuring

	switch (m_state) {
	case State::OBSERVING:
		onProgressToObserving(false);
		break;
	case State::ENCOURAGING:
		onProgressToObserving(true);
		onProgressToEncouraging(false);
		break;
	case State::ENCOURAGED:
		onProgressToObserving(true);
		onProgressToEncouraging(true);
		onProgressToEncouraged(false);
		break;
	default:
		break;
	}
}

void ProgressionEvent::registerGenericDependency(ProgressionEvent* dependency) {
	if (m_state != State::IGNORING || dependency == nullptr || dependency->getState() == State::ACHIEVED) return;

	EventUtility::addTargetedEventListenerFixedPriority(ProgressionEvent::EVENT_PROGRESSION_TO_ACHIEVED, 1, this, dependency->eventID, &ProgressionEvent::onGenericDependencyAchieved);
	m_unresolvedDependencies++;
}

void ProgressionEvent::registerConditionListener(cocos2d::EventListenerCustom* listener) {
	if (m_state == State::ACHIEVED) return;

	m_conditionListeners.push_back(listener);
}

void ProgressionEvent::onGenericDependencyAchieved(cocos2d::EventCustom*) {
	m_unresolvedDependencies--;
	if (m_unresolvedDependencies == 0) {
		progressToObserving();
	}
}

void ProgressionEvent::clearConditionListeners() {
	for (auto listener : m_conditionListeners) {
		EventUtility::removeEventListener(listener);
	}
	m_conditionListeners.clear();
}
