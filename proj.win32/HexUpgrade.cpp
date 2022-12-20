#include "HexUpgrade.h"

HexUpgrade::HexUpgrade(float magnitude) : m_magnitude(magnitude)
{
}

HexUpgrade::HexUpgrade(BigInt magnitude) : m_magnitude(magnitude)
{
}

BigInt& HexUpgrade::actUpon(unsigned int level, BigInt& target) {
	vart a = std::visit([]()->vart{})
}
