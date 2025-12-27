//
// Created by fightinghawks18 on 12/26/2025.
//

#pragma once

#ifdef BUILD_SHARED

#ifdef _WIN32

#ifdef BUILD_CORE

#define CCO_CORE_API __declspec(dllexport)

#else // BUILD_CORE

#define CCO_CORE_API __declspec(dllimport)

#endif // BUILD_CORE

#else // _WIN32

#define CCO_CORE_API __attribute__((visibility("default")))

#endif // _WIN32

#else // BUILD_SHARED

#define CCO_CORE_API

#endif // BUILD_SHARED