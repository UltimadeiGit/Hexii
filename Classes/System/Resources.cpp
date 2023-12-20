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
    try { data.at("greenMatter").get_to(m_greenMatter); } catch (json::out_of_range) { m_greenMatter = 6; }
    try { data.at("redMatter").get_to(m_redMatter); } catch (json::out_of_range) { m_redMatter = 0; }
    data.at("tabsEnabled").get_to(m_tabsEnabled);
   
    return true;
}

void Resources::addHexiiInlayer(uint layer, int num) {
    if (layer >= GameplayCommon::MAX_LAYER) return;

    m_hexiiCountPerlayer[layer] += num;
    if (layer > 0 && m_hexiiCountPerlayer[layer] > 6 * layer) err("Hexii count per layer exceeded maximum expected value");
}

void to_json(nlohmann::json& j, const Resources& resources) {
    json hexiiCountPerlayer = json::array();
    for (uint i = 0; i < GameplayCommon::MAX_LAYER; i++) hexiiCountPerlayer.push_back(resources.getHexiiCountInlayer(i));

    json tabsEnabled = json::array();
    for (uint i = 0; i < 5; i++) tabsEnabled.push_back(resources.getTabEnabled(i));

    j = json{
        {"greenMatter", resources.getGreenMatter()},
		{"redMatter", resources.getRedMatter()},
        {"hexiiCountPerlayer", hexiiCountPerlayer},
        {"tabsEnabled", tabsEnabled}
    };
}