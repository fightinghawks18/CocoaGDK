//
// Created by fightinghawks18 on 12/21/25.
//

#pragma once

#ifdef _WIN32

#ifdef CCO_BUILDING_PLATFORM
#define CCO_PLATFORM_API __declspec(dllexport)
#else
#define CCO_PLATFORM_API __declspec(dllimport)
#endif

#else
#define CCO_PLATFORM_API __attribute__((visibility("default")))
#endif