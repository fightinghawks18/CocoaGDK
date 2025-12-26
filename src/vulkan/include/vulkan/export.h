//
// Created by fightinghawks18 on 12/25/2025.
//

#pragma once

#ifdef BUILD_SHARED

#ifdef _WIN32

#ifdef BUILD_VULKAN

#define CCO_VULKAN_API __declspec(dllexport)

#else // BUILD_VULKAN

#define CCO_VULKAN_API __declspec(dllimport)

#endif // BUILD_VULKAN

#else // _WIN32

#define CCO_VULKAN_API __attribute__((visibility("default")))

#endif // _WIN32

#else // BUILD_SHARED

#define CCO_VULKAN_API

#endif // BUILD_SHARED