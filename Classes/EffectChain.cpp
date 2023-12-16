#include "EffectChain.h"
#include <cocos2d.h>

const EffectChain::EffectFunction EffectChain::noEffect = [](bool) {};

EffectChain::EffectChain(EffectChainPtr next, std::function<bool()> carryOnCondition, EffectFunction effect)
	: m_next(next), m_condition(carryOnCondition), m_effect(effect)
{}

EffectChainPtr EffectChain::setNext(EffectChainPtr next) {
	// Terminate whatever is currently next
	if (m_next) {
		m_next->terminate();
	}

	m_next = next ? next->getTop() : nullptr;
	if (m_next) {
		m_next->m_prev = m_self;
		// Whenever a chain is extended, the reference to the top effect remains the same
		m_next->m_top = m_top;
		return m_next->getBottom();
	}

	return m_self;
}

void EffectChain::start() {
	if (m_condition()) {
		// Stop our own effect
		stop();
		// Start the next effect
		if (m_next) m_next->start();
		else m_top->terminate();
	}
	else {
		// Start our own effect
		if (!m_active) {
			m_effect(true);
			m_active = true;
		}

		// Stop the next effect
		if (m_next) m_next->stop();	

		// Schedule to check again on the next frame
		cont();
	}
}

void EffectChain::cont() {
	cocos2d::Director::getInstance()->getScheduler()->schedule(
		[this](float dt) {
			m_top->start();
		}, 
		this, 0, 0, 0, false, "continueEffect"
	);		
}

void EffectChain::stop() {
	if (!m_active) return;

	// Stop our own effect
	m_effect(false);
	m_active = false;
}

void EffectChain::terminate() {
	// Terminates recursively from the top
	printf("Beginning termination of effect\n");
	stop();

	// Clean up all of the shared ptrs

	if (m_next) {
		// Detach from next
		m_next->m_prev.reset();
		// Continue terminating forward
		m_next->terminate();
		// Remove the ptr to next
		m_next.reset();
	}
	/*
	if (m_prev) {
		// Detach from prev
		m_prev->m_next.reset();
		// Continue terminating backward
		m_prev->terminate();
		// Remove the ptr to prev
		m_prev.reset();
	}
	*/
	// Remove the ptr to top
	m_top.reset();
	// Remove the ptr to self
	m_self.reset();
}
