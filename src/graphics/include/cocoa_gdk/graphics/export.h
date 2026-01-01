//
// Created by fightinghawks18 on 12/29/2025.
//

#pragma once



#ifdef BUILD_SHARED

#ifdef _WIN32

#ifdef BUILD_GRAPHICS

#define CCO_GRAPHICS_API __declspec(dllexport)

#else // BUILD_GRAPHICS

#define CCO_GRAPHICS_API __declspec(dllimport)

#endif // BUILD_GRAPHICS

#else // _WIN32

#define CCO_GRAPHICS_API __attribute__((visibility("default")))

#endif // _WIN32

#else // BUILD_SHARED

#define CCO_GRAPHICS_API

#endif // BUILD_SHARED
