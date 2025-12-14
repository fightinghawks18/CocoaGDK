#pragma once

#include "../core.h"
#include "core.h"
#include "../graphics/types.h"
#include <vulkan/vulkan.h>

typedef struct CcoVulkanSwapChain_T CcoVulkanSwapChain_T;
typedef CcoVulkanSwapChain_T* CcoVulkanSwapChain;

typedef struct {
    CcoVulkanCore core;
    VkSurfaceKHR surface;
    CcoExtent extent;
} CcoVulkanSwapChainDesc;

CcoResult ccoCreateVulkanSwapChain(const CcoVulkanSwapChainDesc *desc, CcoVulkanSwapChain *outSwapChain);
void ccoDestroyVulkanSwapChain(CcoVulkanCore core, CcoVulkanSwapChain swapChain);

