#pragma once

#include "cocos2d.h"
#include "TabbedWidget.h"

namespace CommonEffects {
	constexpr int TAG_COMMON_EFFECT = 0x123;

	inline cocos2d::RepeatForever* createPulseEffect(float duration, const cocos2d::Color3B& desiredCol = cocos2d::Color3B(255, 255, 0)) {
		return cocos2d::RepeatForever::create(
			cocos2d::Sequence::create(
				cocos2d::TintTo::create(duration, desiredCol),
				cocos2d::TintTo::create(duration, 255, 255, 255),
				nullptr
			)
		);
	}

	inline EffectChainPtr createEncouragementForTarget(cocos2d::Node* target, std::function<bool()> carryOnFunction, const cocos2d::Color3B& desiredCol = cocos2d::Color3B(255, 255, 0)) {
		return EffectChain::create(nullptr, carryOnFunction,
			[target, desiredCol](bool isOn) {
				// Encourage the targets
				if (isOn) {
					target->runAction(CommonEffects::createPulseEffect(0.8f, desiredCol))->setTag(CommonEffects::TAG_COMMON_EFFECT);
				}
				else {
					// Encouragement off				
					target->stopAllActionsByTag(CommonEffects::TAG_COMMON_EFFECT);
					target->setColor(cocos2d::Color3B::WHITE);
				}
			}
		);
	}

	inline EffectChainPtr createEncouragementForTarget(std::function<cocos2d::Node*()> targetGenerator, std::function<bool()> carryOnFunction, const cocos2d::Color3B& desiredCol = cocos2d::Color3B(255, 255, 0)) {
		return EffectChain::create(nullptr, carryOnFunction,
			[targetGenerator, desiredCol](bool isOn) {
				auto target = targetGenerator();
				
				// Encourage the targets
				if (isOn) {
					target->runAction(CommonEffects::createPulseEffect(0.8f, desiredCol))->setTag(CommonEffects::TAG_COMMON_EFFECT);
				}
				else {
					// Encouragement off				
					target->stopAllActionsByTag(CommonEffects::TAG_COMMON_EFFECT);
					target->setColor(cocos2d::Color3B::WHITE);
				}
			}
		);
	}

	template <int TabCount>
	inline EffectChainPtr createEncouragementForTab(TabbedWidget<TabCount>* tabbedWidget, unsigned short tabNumber, const cocos2d::Color3B& desiredCol = cocos2d::Color3B(255, 255, 0)) {
		return CommonEffects::createEncouragementForTarget(
			[tabbedWidget, tabNumber]() { return tabbedWidget->getTabButton(tabNumber); },
			[tabbedWidget, tabNumber]() { 
				return tabbedWidget->getCurrentTabNumber() == tabNumber; 
			},
			desiredCol
		);
	}
}