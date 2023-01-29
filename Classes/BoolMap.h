#pragma once

#include <string>
#include <map>
#include "JSON_FWD.hpp"

// Simple wrapper around a map of string : bool pairs
class BoolMap {
public:
	inline BoolMap() {}
	BoolMap(const nlohmann::json& data);

	void set(const std::string& name, bool value);
	bool get(const std::string& name) const;

	inline const std::map<const std::string, bool>& getMap() const { return m_values; }

	// Returns the value under `name`. Const safe
	bool operator()(const std::string& name) const;

	// Returns the value under `name`
	bool& operator[](const std::string& name);
private:
	std::map<const std::string, bool> m_values;
};

extern void to_json(nlohmann::json& j, const BoolMap& boolMap);