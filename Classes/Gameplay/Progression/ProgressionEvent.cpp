#include "ProgressionEvent.h"
#include "Console.h"


/*
constexpr const char* ProgressionEvent::EVENT_PROGRESSION_TO(State state)
{	
	switch (state) {
	case State::OBSERVING:
		return EVENT_PROGRESSION_TO_OBSERVING;
	case State::ENCOURAGING:
		return EVENT_PROGRESSION_TO_ENCOURAGING;
	case State::ENCOURAGED:
		return EVENT_PROGRESSION_TO_ENCOURAGED;
	case State::ACHIEVED:
		return EVENT_PROGRESSION_TO_ACHIEVED;
	default:
		return "";
	}
}
*/

ProgressionEvent::ProgressionEvent(State initialState)
	: m_state(initialState)
{}

cocos2d::EventListenerCustom* ProgressionEvent::whenNext(State state, const std::function<void()>&callback, bool autoRemoveListener) {
	if (m_state >= state && autoRemoveListener) return nullptr;

	cocos2d::EventListenerCustom* listener = EventUtility::addTargetedEventListenerFixedPriority(
		ProgressionEvent::EVENT_PROGRESSION_TO(state), 1, eventID,
		[listener, callback, autoRemoveListener](cocos2d::EventCustom*) {
			// Call the callback and remove the listener if required to

			callback();
			if (autoRemoveListener) EventUtility::removeEventListener(listener);
		}
	);

	return listener;
}

cocos2d::EventListenerCustom* ProgressionEvent::when(State state, const std::function<void()>& callback, bool exactState, bool autoRemoveListener) {
	cocos2d::EventListenerCustom* listener = nullptr;

	// If the event is already in the required state (or greater), call the callback immediately
	if (m_state == state || (m_state > state && !exactState)) callback();
	// Otherwise, add a listener to call the callback when the event is achieved
	else listener = whenNext(state, callback, autoRemoveListener);

	return listener;
}

void ProgressionEvent::init() {
	if (getUnresolvedDependencyCount() == 0) {
		if (getState() == State::IGNORING) progressToObserving();
		else followInitialState();
	}
}

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


void ProgressionEvent::followState(State state, bool progressToState) {
	if(progressToState) m_state = state;

	// Dispatch events for each state change that's occuring
	switch (state) {
	case State::OBSERVING:
		if (progressToState) progressToObserving();
		else onProgressToObserving(false);
		break;
	case State::ENCOURAGING:
		onProgressToObserving(true);
		if (progressToState) progressToEncouraging();
		else onProgressToEncouraging(false);
		break;
	case State::ENCOURAGED:
		onProgressToObserving(true);
		onProgressToEncouraging(true);
		if (progressToState) progressToEncouraged();
		else onProgressToEncouraged(false);
		break;
	case State::ACHIEVED:
		onProgressToObserving(true);
		onProgressToEncouraging(true);
		onProgressToEncouraged(true);
		if (progressToState) progressToAchieved();
		else onProgressToAchieved();
		break;
	default:
		break;
	}
}

void ProgressionEvent::registerGenericDependency(ProgressionEvent* dependency) {
	m_unresolvedDependencies.push_back(dependency);
	dependency->when(State::ACHIEVED, [this, dependency]() {
		onGenericDependencyAchieved(dependency);
	});
}

void ProgressionEvent::registerConditionListener(cocos2d::EventListenerCustom* listener) {
	m_conditionListeners.push_back(listener);
}

void ProgressionEvent::onGenericDependencyAchieved(ProgressionEvent* dependency) {
	// Remove the dependency from the vector
	m_unresolvedDependencies.erase(std::remove(m_unresolvedDependencies.begin(), m_unresolvedDependencies.end(), dependency));
	
	// If there are no more unresolved dependencies, progress to observing
	if (getUnresolvedDependencyCount() == 0 && m_state == State::IGNORING) progressToObserving();
}

void ProgressionEvent::clearConditionListeners() {
	for (auto listener : m_conditionListeners) {
		EventUtility::removeEventListener(listener);
	}
	m_conditionListeners.clear();
}

cocos2d::EventListenerCustom* RepeatableProgressionEvent::when(State state, const std::function<void()>& callback, bool exactState, bool autoRemoveListener) {
	if (!exactState && m_achievedCount > 0) {
		callback();
		return nullptr;
	} else return ProgressionEvent::when(state, callback, exactState, autoRemoveListener);
}

void RepeatableProgressionEvent::registerRepeatableDependency(RepeatableProgressionEvent* dependency) {
	// If this progression event is in a state other than IGNORING and then one of its dependencies is reversed, it needs to reverse its own progression as well
	if (getState() != State::IGNORING) reverseProgression();
	
	// If the dependency is already achieved, it will try and progress to the queued state, so we need to make sure that is IGNORING
	m_queuedState = State::IGNORING;

	if(dependency->getState() != State::ACHIEVED) m_repeatableDependencies.push_back(dependency);

	dependency->when(State::ACHIEVED, [this, dependency]() {
		onRepeatableDependencyAchieved(dependency);
	}, true); // The dependency is achieved only when its state is EXACTLY Achieved, not if it's been achieved before

	// Handle the dependency reversing by re-registering it
	dependency->whenNext(State::IGNORING, [this, dependency]() {
		// Re-register the dependency
		registerRepeatableDependency(dependency);
	});

	// Re-set the queued state to observing
	m_queuedState = State::OBSERVING;
}

void RepeatableProgressionEvent::setQueuedState(State state) {
	m_queuedState = state;

	// If there are no unresolved dependencies, immediately progress to the queued state
	if (getUnresolvedDependencyCount() == 0) followState(m_queuedState, true);
}

void RepeatableProgressionEvent::onRepeatableDependencyAchieved(RepeatableProgressionEvent* dependency) {
	// Remove the dependency from the vector
	m_repeatableDependencies.erase(std::remove(m_repeatableDependencies.begin(), m_repeatableDependencies.end(), dependency));

	// If there are no more unresolved dependencies, progress to the current queued state
	if (getUnresolvedDependencyCount() == 0 && m_state == State::IGNORING) followState(m_queuedState, true);
}

void RepeatableProgressionEvent::reverseProgression() {
	if (getState() != State::ACHIEVED) warn("Attempting to reverse progression when not in achieved state");

	m_state = State::IGNORING;
	m_queuedState = State::OBSERVING;
	// Condition listeners will be reregistered at the appropriate time so they need to be cleared now
	clearConditionListeners();
	onProgressionReversed();
	EventUtility::dispatchEvent(RepeatableProgressionEvent::EVENT_PROGRESSION_REVERSED, eventID, EventProgressionStateChange{ this });

	// If there are already no unresolved dependencies, immediately progress to observing
	if(getUnresolvedDependencyCount() == 0) progressToObserving();
}
