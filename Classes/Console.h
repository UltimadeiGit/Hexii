#pragma once

#include <string>

// This file is hacky but it's debug only so it gets a pass :)

static bool ErrFlag = 0;

extern void warn(const std::string& msg);
extern void err(const std::string& msg);