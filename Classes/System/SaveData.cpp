#include "SaveData.h"
#include "JSON.hpp"
#include "Resources.h"
#include "Settings.h"
#include "Console.h"
#include "Progression.h"

USING_NS_CC;
using namespace nlohmann;

SaveData* SaveData::m_instance = nullptr;

SaveData::SaveData() {
    time(&m_timeOfLastSave);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(
        EventListenerCustom::create("onSettingChange", CC_CALLBACK_1(SaveData::onSettingChange, this)),
        1
    );

    refreshAutosave(Settings::getInstance()->getSetting("autosaveFrequencySeconds"));
}

SaveData* SaveData::getInstance() {
    if (SaveData::m_instance) return SaveData::m_instance;

    SaveData::m_instance = new SaveData();
    return SaveData::m_instance;
}

uint SaveData::tryLoad(uint targets) {
    auto userDefault = UserDefault::getInstance();
    uint successFlag = LoadFlags::NONE;

    if (targets & LoadFlags::SETTINGS) {
        std::string unparsedSettingsData = userDefault->getStringForKey("saveData.settings", "--");
        if (unparsedSettingsData != "--") {
            json settingsData = json::parse(unparsedSettingsData);

            if (Settings::getInstance()->load(settingsData)) successFlag |= LoadFlags::SETTINGS;
        }        
    }

    if (targets & LoadFlags::RESOURCES) {
        std::string unparsedResourcesData = userDefault->getStringForKey("saveData.resources", "--");
        if (unparsedResourcesData != "--") {
            json resourcesData = json::parse(unparsedResourcesData);

            if (Resources::getInstance()->load(resourcesData)) successFlag |= LoadFlags::RESOURCES;
        }
    }

    if (targets & LoadFlags::PLANE) {
        std::string unparsedPlaneData = userDefault->getStringForKey("saveData.HexiiPlane", "--");
        if (unparsedPlaneData != "--") {
            json planeData = json::parse(unparsedPlaneData);

            if (HexiiPlane::create(planeData)) successFlag |= LoadFlags::PLANE;
        }
    }

    if (targets & LoadFlags::PROGRESSION) {
        std::string unparsedProgressionData = userDefault->getStringForKey("saveData.Progression", "--");
        if (unparsedProgressionData != "--") {
            json progressionData = json::parse(unparsedProgressionData);

            Progression::load(progressionData);

            successFlag |= LoadFlags::PROGRESSION;
        }
        else {
            // Load progression with the default values
            Progression::getInstance();
        }
    }

    return successFlag;
}

void SaveData::save() {
    auto userDefault = UserDefault::getInstance();

    userDefault->setStringForKey("saveData.resources", json(*Resources::getInstance()).dump());
    userDefault->setStringForKey("saveData.settings", json(*Settings::getInstance()).dump());
    userDefault->setStringForKey("saveData.HexiiPlane", json(*HexiiPlane::getInstance()).dump());
    userDefault->setStringForKey("saveData.Progression", json(*Progression::getInstance()).dump());

    time(&m_timeOfLastSave);
}

float SaveData::getTimeSinceLastSave() const {
    time_t currentTime;
    time(&currentTime);

    return currentTime - m_timeOfLastSave;
}

void SaveData::onSettingChange(cocos2d::EventCustom* evnt) {
    Settings::EventSettingChange* data = static_cast<Settings::EventSettingChange*>(evnt->getUserData());

    if (data->settingName == "autosaveFrequencySeconds") refreshAutosave(data->settingAfter);
}

void SaveData::refreshAutosave(const json& autosaveSetting) {
    auto scheduler = Director::getInstance()->getScheduler();

    scheduler->unschedule("autosave", this);
    scheduler->schedule([this](float) {save(); }, this, autosaveSetting, false, "autosave");

    // TODO: Should trigger an autosave whenever the user refocuses the app
}
