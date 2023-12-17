#include "Settings.h"

USING_NS_CC;
using namespace nlohmann;

Settings* Settings::m_instance = nullptr;

Settings::Settings() {
    // Default settings

    changeSetting("autosaveFrequencySeconds", (float)30, true);
}

Settings* Settings::getInstance() {
    if (Settings::m_instance) return Settings::m_instance;

    Settings::m_instance = new Settings();
    return Settings::m_instance;
}

bool Settings::load(const nlohmann::json& data) {
    m_settings.merge_patch(data.at("settings"));

    return true;
}

nlohmann::json Settings::getSetting(const std::string& settingName) const {
    if (!m_settings.contains(settingName)) return json::object();

    return m_settings.at(settingName);
}

void Settings::changeSetting(const std::string& settingName, json newSetting, bool suppressEvent) {
    json settingBefore = getSetting(settingName);

    m_settings[settingName] = newSetting;
    
    if(!suppressEvent)
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("onSettingChange", new EventSettingChange{ settingName, settingBefore, newSetting });
}

void to_json(nlohmann::json& j, const Settings& resources) {
    j = json{
        {"settings", resources.getAllSettings()}
    };
}