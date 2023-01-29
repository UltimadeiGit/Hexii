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
    data.at("hexiiCountPerLayer").get_to(m_hexiiCountPerLayer);
    data.at("tabsEnabled").get_to(m_tabsEnabled);

    return true;
}

void Resources::addGreenMatter(BigReal op) {
    if (op > 0 || m_greenMatter >= std::abs(op)) m_greenMatter += op;
    else err("Green matter cost was unaffordable");
}

void Resources::addHexInLayer(uint layer) {
    if (layer >= Resources::MAX_LAYERS) return;

    m_hexiiCountPerLayer[layer]++;
    if (layer > 0 && m_hexiiCountPerLayer[layer] > 6 * layer) err("Hex count per layer exceeded maximum expected value");
}

void to_json(nlohmann::json& j, const Resources& resources) {
    json hexiiCountPerLayer = json::array();
    for (uint i = 0; i < Resources::MAX_LAYERS; i++) hexiiCountPerLayer.push_back(resources.getHexiiCountInLayer(i));

    json tabsEnabled = json::array();
    for (uint i = 0; i < 5; i++) tabsEnabled.push_back(resources.getTabEnabled(i));

    j = json{
        {"greenMatter", resources.getGreenMatter()},
        {"hexiiCountPerLayer", hexiiCountPerLayer},
        {"tabsEnabled", tabsEnabled}
    };
}