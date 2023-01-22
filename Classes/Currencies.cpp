#include "Currencies.h"

Currencies* Currencies::m_instance = nullptr;

Currencies::Currencies() : m_greenMatter(6) {}

Currencies* Currencies::instance() {
    if (Currencies::m_instance) return Currencies::m_instance;

    Currencies::m_instance = new Currencies();
    return Currencies::m_instance;
}

bool Currencies::addGreenMatter(BigReal op) {
    if (op > 0 || m_greenMatter >= std::abs(op)) m_greenMatter += op;
    else return false;
    return true;
}
