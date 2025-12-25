//
// Created by fightinghawks18 on 12/24/2025.
//

#pragma once

#include <vulkan/vulkan.h>

#include "../tools/vma.h"
#include "../instance.h"

struct cco_vulkan_instance_t {
    VkInstance instance;

    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties device_props;
    VkPhysicalDeviceFeatures device_features;

    VkDevice device;

    VmaAllocator allocator;

    VkQueue graphics_queue;
    VkQueue transfer_queue;
    VkQueue compute_queue;
    u32 graphics_family_index;
    u32 transfer_family_index;
    u32 compute_family_index;
};