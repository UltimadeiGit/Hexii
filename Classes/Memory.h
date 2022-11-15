#pragma once

#include <memory>

#include "cocos2d.h"

#define CREATE_FUNC_SHARED(__TYPE__) \
CREATE_FUNC(__TYPE__) \
static std::shared_ptr<__TYPE__> createShared() \
{ \
	__TYPE__* pRet = __TYPE__::create(); \
	if (pRet != nullptr) return std::shared_ptr<__TYPE__>(pRet); \
	else return nullptr; \
}