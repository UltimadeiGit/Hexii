#include "ProgressionEvent.h"
#include "HexiiPlane.h"

PurchaseFirstHexiiProgressionEvent::PurchaseFirstHexiiProgressionEvent(State initialState) : ProgressionEvent(initialState) 
{}

void PurchaseFirstHexiiProgressionEvent::onProgressToObserving(bool didSkip) {
	// Immediately go to encouraging
	progressToEncouraging();
}

void PurchaseFirstHexiiProgressionEvent::onProgressToEncouraging(bool didSkip) {
	if (didSkip) return;

	Hexii* centreHex = HexiiPlane::getInstance()->getHexiiAtPos({ 0, 0 });

	// Check if the player has already purchased a hexii
	if (centreHex->getActive()) {
		// Immediately go to achieved
		progressToAchieved();
		return;
	}

	// Set up the listener for the first hexii purchase
	registerConditionListener(
		EventUtility::addGlobalEventListenerFixedPriority(
			Hexii::EVENT_PURCHASE, 1, this, &PurchaseFirstHexiiProgressionEvent::onHexiiPurchase
		)
	);

	// Encouragement
	centreHex->setShaderEffect(SimpleShader::create(SimpleShaderManager::getEncouragementGlowProgram()));
}

void PurchaseFirstHexiiProgressionEvent::onHexiiPurchase(cocos2d::EventCustom* evnt) {
	printf("Hexii purchase detected and used to progress first hexii purchase.\n");

	auto data = EventUtility::getEventData<Hexii::EventHexiiPurchaseData>(evnt);

	// Remove the encouragement effect
	data->subject->clearShaderEffect();

	// Immediately go to achieved (skipping encouraged)
	progressToAchieved();
}
