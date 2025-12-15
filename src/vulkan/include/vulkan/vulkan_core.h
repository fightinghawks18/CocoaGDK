//
// Created by fightinghawks18 on 12/12/25.
//

#pragma once

#include "core/core_types.h"
#include "vulkan_types.h"
#include <vulkan/vulkan.h>

typedef struct CcoVulkanCore_T CcoVulkanCore_T;
typedef CcoVulkanCore_T *CcoVulkanCore;

CcoResult ccoCreateVulkanCore(const CcoVulkanCoreDesc *desc, CcoVulkanCore *outCore);
void ccoDestroyVulkanCore(CcoVulkanCore core);
VkInstance ccoGetVulkanCoreInstance(CcoVulkanCore core);
VkPhysicalDevice ccoGetVulkanCorePhysicalDevice(CcoVulkanCore core);
VkDevice ccoGetVulkanCoreDevice(CcoVulkanCore core);