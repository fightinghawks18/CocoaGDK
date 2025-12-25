//
// Created by fightinghawks18 on 12/24/2025.
//

#pragma once

#include <vulkan/vulkan.h>

#include "../buffer.h"
#include "instance_vulkan.h"

struct cco_vulkan_buffer_t {
    cco_vulkan_instance instance;

    VkBuffer buffer;
    VmaAllocation allocation;
    VmaAllocationInfo allocationInfo;
};