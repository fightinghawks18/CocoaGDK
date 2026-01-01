//
// Created by fightinghawks18 on 12/25/2025.
//

#pragma once



#ifdef BUILD_SHARED

#ifdef _WIN32

#ifdef BUILD_PLATFORM

#define CCO_PLATFORM_API __declspec(dllexport)

#else // BUILD_PLATFORM

#define CCO_PLATFORM_API __declspec(dllimport)

#endif // BUILD_PLATFORM

#else // _WIN32

#define CCO_PLATFORM_API __attribute__((visibility("default")))

#endif // _WIN32

#else // BUILD_SHARED

#define CCO_PLATFORM_API

#endif // BUILD_SHARED
