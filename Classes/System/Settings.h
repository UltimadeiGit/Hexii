#pragma once

#include "Maths.h"
#include "JSON.hpp"
#include "HexiiPlane.h"
#include <vector>

class Settings {
public:
	struct EventSettingChange {
		const std::string settingName;
		nlohmann::json settingBefore;
		nlohmann::json settingAfter;
	};

private:
	// Singleton pattern

	Settings();
	Settings(const Settings&) = delete;
	Settings(Settings&&) = delete;

public:
	static Settings* getInstance();

	bool load(const nlohmann::json& data);

	inline const nlohmann::json getAllSettings() const { return m_settings; }

	// Returns the requested setting, or an empty json object if it doesn't exist
	nlohmann::json getSetting(const std::string& settingName) const;
	// Changes the requested setting and dispatches an event for the change
	void changeSetting(const std::string& settingName, nlohmann::json newSetting, bool suppressEvent = false);

private:
	static Settings* m_instance;

	nlohmann::json m_settings;
};

extern void to_json(nlohmann::json& j, const Settings& settings);