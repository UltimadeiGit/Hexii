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

Vec2 roundVec2(const Vec2& target) {
	return Vec2(roundf(target.x), roundf(target.y));
}

Vec3 roundVec3(const Vec3& target) {
	return Vec3(roundf(target.x), roundf(target.y), roundf(target.z));
}

Vec2 absVec2(const cocos2d::Vec2& target) {
	return Vec2(abs(target.x), abs(target.y));
}

Vec3 absVec3(const Vec3& target) {
	return Vec3(abs(target.x), abs(target.y), abs(target.z));
}