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
    data.at("nectar").get_to(m_nectar);
    data.at("hexiiCountPerDistrict").get_to(m_hexiiCountPerDistrict);
    data.at("tabsEnabled").get_to(m_tabsEnabled);
    if (data.contains("Unity2UpgradeBonus")) data.at("Unity2UpgradeBonus").get_to(m_Unity2UpgradeBonus);

    return true;
}

void Resources::addNectar(BigReal op) {
    if (op > 0 || m_nectar >= std::abs(op)) m_nectar += op;
    else err("nectar cost was unaffordable");
}

void Resources::addHexiiInDistrict(uint District) {
    if (District >= Resources::MAX_DISTRICTS) return;

    m_hexiiCountPerDistrict[District]++;
    if (District > 0 && m_hexiiCountPerDistrict[District] > 6 * District) err("Hexii count per District exceeded maximum expected value");
}

void to_json(nlohmann::json& j, const Resources& resources) {
    json hexiiCountPerDistrict = json::array();
    for (uint i = 0; i < Resources::MAX_DISTRICTS; i++) hexiiCountPerDistrict.push_back(resources.getHexiiCountInDistrict(i));

    json tabsEnabled = json::array();
    for (uint i = 0; i < 5; i++) tabsEnabled.push_back(resources.getTabEnabled(i));

    j = json{
        {"nectar", resources.getNectar()},
        {"Unity2UpgradeBonus", resources.getUnity2UpgradeBonus()},
        {"hexiiCountPerDistrict", hexiiCountPerDistrict},
        {"tabsEnabled", tabsEnabled}
    };
}