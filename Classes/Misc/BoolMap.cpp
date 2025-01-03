#include "BoolMap.h"
#include "JSON.hpp"

using namespace nlohmann;

BoolMap::BoolMap(const nlohmann::json& data) {
    data.at("values").get_to(m_values);
}

void BoolMap::set(const std::string& name, bool value) {
    this->operator[](name) = value;
}

bool BoolMap::get(const std::string& name) const {
    return this->operator()(name);
}

bool BoolMap::operator()(const std::string& name) const {
    auto it = m_values.find(name);

    // If `name` was not found in the map then simply return false
    if (it == m_values.end()) return false;

    return it->second;
}

bool& BoolMap::operator[](const std::string& name) {
    auto it = m_values.find(name);

    // If `name` was not found in the map then add it
    if (it == m_values.end()) {
        m_values.emplace(std::make_pair(name, false));
        it = m_values.find(name);
    }

    return it->second;
}

void to_json(json& j, const BoolMap& boolMap) {
    j = json{ { "values", boolMap.getMap() } };
}