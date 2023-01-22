#pragma once

#include <string>
#include <map>

// Simple wrapper around a map of string : bool pairs
class BoolMap {
public:
	void set(const std::string& name, bool value);
	bool get(const std::string& name) const;

	// Returns the value under `name`. Const safe
	bool operator()(const std::string& name) const;

	// Returns the value under `name`
	bool& operator[](const std::string& name);
private:
	std::map<const std::string, bool> m_values;
};