#pragma once

#include "Maths.h"
#include "JSON_FWD.hpp"
#include "HexPlane.h"
#include <vector>

class SaveData {
public:
	enum LoadFlags {
		NONE = 0,
		RESOURCES = 1,
		SETTINGS = 1 << 1,
		PLANE = 1 << 2
	};
private:
	// Singleton pattern

	SaveData();
	SaveData(const SaveData&) = delete;
	SaveData(SaveData&&) = delete;

public:
	static SaveData* getInstance();

	// Attempts to load the game state from stored data. Returns a flag with the success values. 0 means nothing was loaded 
	// Flags are in SaveData::LoadFlags
	uint tryLoad(uint targets);
	void save();

	float getTimeSinceLastSave() const;

private:
	void onSettingChange(cocos2d::EventCustom* evnt);

	void refreshAutosave(const nlohmann::json& autosaveSetting);

	static SaveData* m_instance;

	time_t m_timeOfLastSave = 0;
};