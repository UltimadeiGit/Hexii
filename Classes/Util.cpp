#include <cstdio>

#include "Console.h"
#include "Maths.h"

USING_NS_CC;

// cpp file for all util headers

void warn(const std::string& msg) {
	printf("Warning: %s\n", msg.c_str());
}

void err(const std::string& msg) {
	printf("Error: %s\n", msg.c_str());
	ErrFlag = 1;
}

Vec2 roundVec2(Vec2 target) {
	target.x = std::roundf(target.x);
	target.y = std::roundf(target.y);

	return target;
}