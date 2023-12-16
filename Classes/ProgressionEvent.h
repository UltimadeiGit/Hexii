#pragma once

#include "cocos2d.h"
#include "EventUtility.h"
#include "EffectChain.h"

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

	ProgressionEvent(State initialState);

	inline State getState() const { return m_state; }
	virtual std::string getName() const = 0;

	EventUtility::ID eventID = EventUtility::generateID();

protected:
	ProgressionEvent(const ProgressionEvent&) = delete;
	ProgressionEvent(ProgressionEvent&&) = delete;

	friend class Progression;

	// Called sometime after construction, but before any other function is called
	// This should direct the event to the appropriate state, and register any listeners
	virtual void init() = 0;

	void progressToObserving();
	void progressToEncouraging();
	void progressToEncouraged();
	void progressToAchieved();

	// Did skip is true if the intermediate state was skipped (e.g. Ignoring -> Encouraged)
	virtual void onProgressToObserving(bool didSkip = false) = 0;
	virtual void onProgressToEncouraging(bool didSkip = false) = 0;
	virtual void onProgressToEncouraged(bool didSkip = false) = 0;
	virtual void onProgressToAchieved() = 0;


	// If the initial state isn't already Achieved, listeners probably need to be registered, so the transition
	// calls are necessary. This function just directs to the appropriate progressToX function.
	void followInitialState();

	// A generic dependency is a ProgressionEvent that must be achieved before this one can progress,
	// and which does not have any special behavior that must be handled when it becomes achieved.
	// All generic dependencies should be registered upfront, so that when the number of unresolved dependencies
	// reaches 0, the event can progress to observing.
	// Note: Since dependencies only ever matter while in IGNORING state, the dependency will be disregarded in all other states
	// Dependency will also be ignored if it is already ACHIEVED
	void registerGenericDependency(ProgressionEvent* dependency);
	inline unsigned int getUnresolvedDependencyCount() const { return m_unresolvedDependencies; }

	// A condition listener is any listener that is used during progression but which becomes irrelevant
	// once the event is achieved.
	// Such listeners should be registered as they are added, so they can be automatically cleaned up
	// Note: Since listeners are only relevant before the ACHIEVED state, listener will be disregarded if registered in this state
	void registerConditionListener(cocos2d::EventListenerCustom* listener);

private:
	void onGenericDependencyAchieved(cocos2d::EventCustom*);
	void clearConditionListeners();

	State m_state = State::IGNORING;
	unsigned int m_unresolvedDependencies = 0;
	std::vector<cocos2d::EventListenerCustom*> m_conditionListeners;
};

typedef std::shared_ptr<ProgressionEvent> ProgressionEventPtr;

class PurchaseFirstHexiiProgressionEvent : public ProgressionEvent {
public:
	PurchaseFirstHexiiProgressionEvent(State initialState = State::IGNORING);

	std::string getName() const override { return "purchaseFirstHexii"; }

	void init() override;

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
	HexiiUpgradesAvailableProgressionEvent(PurchaseFirstHexiiProgressionEvent* dependency, State initialState = State::IGNORING);

	std::string getName() const override { return "hexiiUpgradesAvailalble"; }

	void init() override;

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

	HexiiFirstUpgradePurchasedProgressionEvent(HexiiUpgradesAvailableProgressionEvent* dependency, State initialState = State::IGNORING);
	
	std::string getName() const override { return "hexiiFirstUpgradePurchased"; }

	void init() override;

private:
	void onProgressToObserving(bool didSkip = false) override;
	void onProgressToEncouraging(bool didSkip = false) override;
	void onProgressToEncouraged(bool didSkip = false) override;
	void onProgressToAchieved() override;

	void checkAffordable();

	EffectChainPtr createUpgradeTabEncouragement();
};