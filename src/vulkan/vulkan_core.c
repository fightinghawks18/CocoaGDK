//
// Created by fightinghawks18 on 12/12/25.
//

#include "vulkan_core.h"

#include <stdlib.h>
#include <vulkan/vulkan.h>

struct CcoVulkanCore_T {
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
};

static CcoResult createInstance(CcoVulkanCore core, const CcoVulkanCoreDesc *desc) {
    VkApplicationInfo appInfo = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                 .pNext = NULL,
                                 .pApplicationName = "Cocoa",
                                 .applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
                                 .pEngineName = "Cocoa",
                                 .engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
                                 .apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 0)};

    VkInstanceCreateInfo instanceInfo = {.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                         .pNext = NULL,
                                         .flags = 0,
                                         .pApplicationInfo = &appInfo,
                                         .enabledLayerCount = 0,
                                         .ppEnabledLayerNames = NULL,
                                         .enabledExtensionCount = 0,
                                         .ppEnabledExtensionNames = NULL};

    if (vkCreateInstance(&instanceInfo, NULL, &core->instance) != VK_SUCCESS) {
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }
    return CCO_SUCCESS;
}

static CcoResult findPhysicalDevice(CcoVulkanCore core, const CcoVulkanCoreDesc *desc) {}

CcoResult ccoCreateVulkanCore(const CcoVulkanCoreDesc *desc, CcoVulkanCore *outCore) {
    CcoVulkanCore core = malloc(sizeof(CcoVulkanCore_T));
    if (!core)
        return CCO_FAIL_OUT_OF_MEMORY;
    if (createInstance(core, desc) != CCO_SUCCESS) {
        CCO_LOG("Failed to create vulkan instance!");
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }

    *outCore = core;
    return CCO_SUCCESS;
}

void ccoDestroyVulkanCore(CcoVulkanCore core) {}
