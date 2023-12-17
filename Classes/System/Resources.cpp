#include "Resources.h"
#include "Console.h"
#include "JSON.hpp"

using namespace nlohmann;

Resources* Resources::m_instance = nullptr;

Resources::Resources() {}

Resources* Resources::getInstance() {
    if (Resources::m_instance) return Resources::m_instance;

    Resources::m_instance = new Resources();
    return Resources::m_instance;
}

bool Resources::load(const nlohmann::json& data) {
    data.at("greenMatter").get_to(m_greenMatter);
    data.at("hexiiCountPerlayer").get_to(m_hexiiCountPerlayer);
    data.at("tabsEnabled").get_to(m_tabsEnabled);
    if (data.contains("globalPowerUpgradeBonus")) data.at("globalPowerUpgradeBonus").get_to(m_globalPowerUpgradeBonus);

    return true;
}

void Resources::addGreenMatter(BigReal op) {
    if (op > 0 || m_greenMatter >= std::abs(op)) m_greenMatter += op;
    else err("greenMatter cost was unaffordable");
}

void Resources::addHexiiInlayer(uint layer) {
    if (layer >= Resources::MAX_layerS) return;

    m_hexiiCountPerlayer[layer]++;
    if (layer > 0 && m_hexiiCountPerlayer[layer] > 6 * layer) err("Hexii count per layer exceeded maximum expected value");
}

void to_json(nlohmann::json& j, const Resources& resources) {
    json hexiiCountPerlayer = json::array();
    for (uint i = 0; i < Resources::MAX_layerS; i++) hexiiCountPerlayer.push_back(resources.getHexiiCountInlayer(i));

    json tabsEnabled = json::array();
    for (uint i = 0; i < 5; i++) tabsEnabled.push_back(resources.getTabEnabled(i));

    j = json{
        {"greenMatter", resources.getGreenMatter()},
        {"globalPowerUpgradeBonus", resources.getGlobalPowerUpgradeBonus()},
        {"hexiiCountPerlayer", hexiiCountPerlayer},
        {"tabsEnabled", tabsEnabled}
    };
}