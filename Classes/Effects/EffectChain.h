#pragma once

#include <functional>
#include <memory>

class EffectChain;
typedef std::shared_ptr<EffectChain> EffectChainPtr;

class EffectChain {

public:
	// Effect function with `bool isOn` parameter
	typedef std::function<void(bool)> EffectFunction;
	const static EffectFunction noEffect;
	
	inline static EffectChainPtr create(EffectChainPtr next, std::function<bool()> carryOnCondition, EffectFunction effect) {
		EffectChainPtr eff = EffectChainPtr(new EffectChain(next, carryOnCondition, effect));
		//EffectChainPtr()
		eff->m_top = eff;
		eff->m_self = eff;
		return eff;
	}

	inline ~EffectChain() { printf("Deleting effect\n"); }

	inline EffectChainPtr getTop() const { return m_top; }
	inline EffectChainPtr getBottom() const { if(m_next) return m_next->getBottom(); return m_self; }

	// Changes the next effect in the chain. 
	// If a current next effect is running, it will be stopped and removed from this chain.
	// If `next` is already part of a chain, that entire chain will be added to this one
	// Returns the bottom of `next` for convenience. (or `self` if `next` is null)
	EffectChainPtr setNext(EffectChainPtr next);
	void start();

	// Permanently terminates the effect chain
	void terminate();

private:
	EffectChain(EffectChainPtr next, std::function<bool()> carryOnCondition, EffectFunction effect);
	EffectChain(const EffectChain& other) = delete;
	EffectChain(EffectChain&& other) = delete;

	// Schedules this effect to be checked again on the next frame
	void cont();
	// Stops the effect
	void stop();

	bool m_active = false;
	EffectChainPtr m_top = nullptr;
	EffectChainPtr m_self = nullptr;
	EffectChainPtr m_prev = nullptr;
	EffectChainPtr m_next = nullptr;
	std::function<bool()> m_condition;
	EffectFunction m_effect;
};