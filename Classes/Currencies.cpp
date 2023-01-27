#include "Currencies.h"
#include "Console.h"

Currencies* Currencies::m_instance = nullptr;

Currencies::Currencies() : m_greenMatter(6), m_hexiiCountPerLayer{ 0 } {}

Currencies* Currencies::instance() {
    if (Currencies::m_instance) return Currencies::m_instance;

    Currencies::m_instance = new Currencies();
    return Currencies::m_instance;
}

void Currencies::addGreenMatter(BigReal op) {
    if (op > 0 || m_greenMatter >= std::abs(op)) m_greenMatter += op;
    else err("Green matter cost was unaffordable");
}

void Currencies::addHexInLayer(uint layer) {
    if (layer >= Currencies::MAX_LAYERS) return;

    m_hexiiCountPerLayer[layer]++;
    if (layer > 0 && m_hexiiCountPerLayer[layer] > 6 * layer) err("Hex count per layer exceeded maximum expected value");
}
