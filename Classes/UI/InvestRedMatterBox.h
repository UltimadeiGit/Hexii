#pragma once

#include "PurchasableBox.h"
#include "Hexii.h"

class InvestRedMatterBox :
	public PurchasableBox
{
private:
	InvestRedMatterBox() : PurchasableBox(false) {}

	bool init() override;

public:
	CREATE_FUNC(InvestRedMatterBox);

	virtual BigReal getCost() override { return m_desiredInvestment; }

	void setFocusHexii(Hexii* focus);

	void update(float dt) override;
	void updateValues(BigReal redMatterForNext);
	void updateLabels();

private:
	void updateIsAffordable() override;
	// Invests red matter
	void tryPurchase() override;

	Hexii* m_focusHexii = nullptr;

	// The amount of red matter the purchase button will invest
	BigReal m_desiredInvestment = 0;
};