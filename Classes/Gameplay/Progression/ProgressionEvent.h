#pragma once

#include <cocos2d.h>
#include "EventUtility.h"
#include "EffectChain.h"
#include "Maths.h"
#include "Console.h"

#include <memory>
#include <vector>

class ProgressionEvent {
public:
	enum class State {
		IGNORING,
		OBSERVING,
		ENCOURAGING,
		ENCOURAGED,
		ACHIEVED
	};	

	struct EventProgressionStateChange {
		ProgressionEvent* evnt;
	};

	constexpr static const char* EVENT_PROGRESSION_TO_OBSERVING = "onProgressionToObserving";
	constexpr static const char* EVENT_PROGRESSION_TO_ENCOURAGING = "onProgressionToEncouraging";
	constexpr static const char* EVENT_PROGRESSION_TO_ENCOURAGED = "onProgressionToEncouraged";
	constexpr static const char* EVENT_PROGRESSION_TO_ACHIEVED = "onProgressionToAchieved";

	inline constexpr static const char* EVENT_PROGRESSION_TO(State state);

	ProgressionEvent(State initialState);

	inline State getState() const { return m_state; }
	virtual std::string getName() const = 0;

	// Calls the callback the next time this event progresses to `state`. 
	// If already in state, nothing happens and nullptr will be returned if autoRemoveListener is true
	// Otherwise, returns the listener that is generated.
	virtual cocos2d::EventListenerCustom* whenNext(State state, const std::function<void()>& callback, bool autoRemoveListener = true);
	// Calls the callback when this progression event reaches `state`. If it already has, the callback will happen immediately
	// Returns nullptr if the event is already achieved, otherwise returns the listener that is generated.
	// If exactState is true, the callback will only be called if the event progresses to the exact state specified, not a higher state
	// this means that callback may never be called if the event is already in a higher state
	// If autoRemoveListener is true, the listener will be removed after the callback is called
	virtual cocos2d::EventListenerCustom* when(State state, const std::function<void()>& callback, bool exactState = false, bool autoRemoveListener = true);

	EventUtility::ID eventID = EventUtility::generateID();

protected:
	friend class Progression;

	// Called sometime after construction, but before any other function is called
	// This should direct the event to the appropriate state, and register any listeners
	// Default implementation will follow initial state or progress to observing if there are no unresolved dependencies
	virtual void init();

	void progressToObserving();
	void progressToEncouraging();
	void progressToEncouraged();
	void progressToAchieved();

	// Did skip is true if the intermediate state was skipped (e.g. Ignoring -> Encouraged)
	virtual void onProgressToObserving(bool didSkip = false) = 0;
	virtual void onProgressToEncouraging(bool didSkip = false) = 0;
	virtual void onProgressToEncouraged(bool didSkip = false) = 0;
	virtual void onProgressToAchieved() = 0;


	// This function just calls the appropriate onProgressToX functions
	// If `progressToState`, then also progresses to `state`, sending out the appropriate events
	void followState(State state, bool progressToState);
	// If the initial state isn't already Achieved, listeners probably need to be registered, so the transition
	// calls are necessary. This function just directs to the appropriate progressToX function.
	inline void followInitialState() { if(m_state != State::ACHIEVED) followState(m_state, false); }

	// A generic dependency is a ProgressionEvent that must be achieved before this one can progress,
	// and which does not have any special behavior that must be handled when it becomes achieved.
	// All generic dependencies should be registered upfront, so that when the number of unresolved dependencies
	// reaches 0, the event can progress to observing.
	// Note: Since dependencies only ever matter while in IGNORING state, the dependency will be disregarded in all other states
	// Dependency will also be ignored if it is already ACHIEVED
	void registerGenericDependency(ProgressionEvent* dependency);
	inline virtual unsigned int getUnresolvedDependencyCount() const { return m_unresolvedDependencies.size(); }

	// A condition listener is any listener that is used during progression but which becomes irrelevant
	// once the event is achieved.
	// Such listeners should be registered as they are added, so they can be automatically cleaned up
	// Note: Since listeners are only relevant before the ACHIEVED state, listener will be disregarded if registered in this state
	void registerConditionListener(cocos2d::EventListenerCustom* listener);

private:
	ProgressionEvent(const ProgressionEvent&) = delete;
	ProgressionEvent(ProgressionEvent&&) = delete;

	void onGenericDependencyAchieved(ProgressionEvent* dependency);
	void clearConditionListeners();

	State m_state = State::IGNORING;
	std::vector<ProgressionEvent*> m_unresolvedDependencies;
	std::vector<cocos2d::EventListenerCustom*> m_conditionListeners;

	// RepeatableProgressionEvent needs to be a friend so it can modify the state directly
	// This is justified because state should not be directly modifiable by any non-abstract ProgressionEvents
	friend class RepeatableProgressionEvent;
};

typedef std::shared_ptr<ProgressionEvent> ProgressionEventPtr;

// A progression event that can wrap around from achieved to ignoring, tracking the number of times it has been achieved
class RepeatableProgressionEvent : public ProgressionEvent {
public:
	constexpr static const char* EVENT_PROGRESSION_REVERSED = "onProgressionReversed";

	inline RepeatableProgressionEvent(State initialState, uint achievedCount) : 
		ProgressionEvent(initialState), m_achievedCount(achievedCount) {}

	inline virtual uint getUnresolvedDependencyCount() const override { return ProgressionEvent::getUnresolvedDependencyCount() + m_repeatableDependencies.size(); }
	inline unsigned int getAchievedCount() const { return m_achievedCount; }

	// Calls the callback when this progression event reaches `state`. If it already has, the callback will happen immediately
	// If exactState is false, the callback will also happen immediately if this event has ever been achieved before.
	virtual cocos2d::EventListenerCustom* when(State state, const std::function<void()>& callback, bool exactState = false, bool autoRemoveListener = true) override;
	
protected:
	// Registers a dependency that must be achieved before this one can progress but which can reverse its own progression
	// and so must be handled differently
	void registerRepeatableDependency(RepeatableProgressionEvent* dependency);

	// Queues a state to be changed to pending all dependencies.
	void setQueuedState(State state);
	inline State getQueuedState() const { return m_queuedState; }

	// Reverses the progression state of this event to IGNORING. This should only be called when the event is ACHIEVED
	void reverseProgression();

	inline virtual void onProgressToAchieved() override { m_achievedCount++; }
	virtual void onProgressionReversed() = 0;

private:
	void onRepeatableDependencyAchieved(RepeatableProgressionEvent* dependency);

	State m_queuedState = State::OBSERVING;
	// The repeatable dependencies that this event is waiting on
	std::vector<RepeatableProgressionEvent*> m_repeatableDependencies;
	uint m_achievedCount = 0;
};

// Placed down here after EVENT_PROGRESSION_REVERSED is defined
inline constexpr const char* ProgressionEvent::EVENT_PROGRESSION_TO(ProgressionEvent::State state) {
	switch (state) {
	case State::IGNORING:
		return RepeatableProgressionEvent::EVENT_PROGRESSION_REVERSED;
	case State::OBSERVING:
		return EVENT_PROGRESSION_TO_OBSERVING;
	case State::ENCOURAGING:
		return EVENT_PROGRESSION_TO_ENCOURAGING;
	case State::ENCOURAGED:
		return EVENT_PROGRESSION_TO_ENCOURAGED;
	case State::ACHIEVED:
		return EVENT_PROGRESSION_TO_ACHIEVED;
	default:
		err("Unhandled state");
		return "";
	}
}

class PurchaseFirstHexiiProgressionEvent : public ProgressionEvent {
public:
	PurchaseFirstHexiiProgressionEvent(State initialState);

	std::string getName() const override { return "purchaseFirstHexii"; }

private:
	void onProgressToObserving(bool didSkip = false) override;
	void onProgressToEncouraging(bool didSkip = false) override;
	void onProgressToEncouraged(bool didSkip = false) override {}
	void onProgressToAchieved() override {}

	void onHexiiPurchase(cocos2d::EventCustom* evnt);
};

// Progression from before the upgrade menu is available to after the first upgrade is purchased
class HexiiUpgradesAvailableProgressionEvent : public ProgressionEvent {
public:
	HexiiUpgradesAvailableProgressionEvent(PurchaseFirstHexiiProgressionEvent* dependency, State initialState);

	std::string getName() const override { return "hexiiUpgradesAvailalble"; }

private:
	void onProgressToObserving(bool didSkip = false) override;
	void onProgressToEncouraging(bool didSkip = false) override {}
	void onProgressToEncouraged(bool didSkip = false) override {}
	void onProgressToAchieved() override {}

	void onLevelGained(cocos2d::EventCustom* evnt);
};

class HexiiFirstUpgradePurchasedProgressionEvent : public ProgressionEvent {
public:
	// TODO: * Temporary encouragement: Highlight upgrade tab once it becomes available until it is clicked
	// * Stages of encouragement when enough green matter is available to purchase an upgrade

	HexiiFirstUpgradePurchasedProgressionEvent(HexiiUpgradesAvailableProgressionEvent* dependency, State initialState);
	
	std::string getName() const override { return "hexiiFirstUpgradePurchased"; }
private:
	void onProgressToObserving(bool didSkip = false) override;
	void onProgressToEncouraging(bool didSkip = false) override;
	void onProgressToEncouraged(bool didSkip = false) override;
	void onProgressToAchieved() override;

	void checkAffordable();

	EffectChainPtr createUpgradeTabEncouragement();
};


// Encouragement starts when the L0 hex reaches level 60
// Achieved when the L0 hex reaches level 66
class SacrificeUnlockedProgressionEvent : public RepeatableProgressionEvent {

public:
	SacrificeUnlockedProgressionEvent(State initialState, uint achievedCount);

	inline std::string getName() const override { return "sacrificeUnlocked"; }

private:

	void onProgressToObserving(bool didSkip) override;
	void onProgressToEncouraging(bool didSkip) override;
	void onProgressToEncouraged(bool didSkip) override;
	void onProgressToAchieved() override;
	void onProgressionReversed() override;

	void onHexiiLevelGained(cocos2d::EventCustom* evnt);

	// Directs to the appropriate state based on hexii level. Returns true if the event progressed to a new state
	bool progressFromLevel(BigInt hexiiLevel);
};

class FirstSacrificeCompletedProgressionEvent : public ProgressionEvent {
public:
	FirstSacrificeCompletedProgressionEvent(SacrificeUnlockedProgressionEvent* dependency, State initialState);

	inline std::string getName() const override { return "firstSacrificeCompleted"; }

private:

	void onProgressToObserving(bool didSkip) override;
	void onProgressToEncouraging(bool didSkip) override;
	void onProgressToEncouraged(bool didSkip) override;
	void onProgressToAchieved() override;

	void onSacrificeCompleted(cocos2d::EventCustom* evnt);
};