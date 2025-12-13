//
// Created by fightinghawks18 on 12/12/25.
//

#include "vulkan_core.h"

#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "../tools/dynamic_array.h"

struct CcoVulkanCore_T {
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
};

static void plugPlatformExtensions(CcoDynamicArray *extensions) {
    ccoPushBackDynamicArray(extensions, "VK_KHR_surface");
#ifdef __APPLE__
    ccoPushBackDynamicArray(extensions, "VK_EXT_metal_surface");
    ccoPushBackDynamicArray(extensions, "VK_KHR_portability_enumeration");
#endif
}

static CcoResult createInstance(CcoVulkanCore core, const CcoVulkanCoreDesc *desc) {
    VkApplicationInfo appInfo = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                 .pNext = NULL,
                                 .pApplicationName = "Cocoa",
                                 .applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
                                 .pEngineName = "Cocoa",
                                 .engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
                                 .apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 0)};

    CcoDynamicArray *extensions = ccoCreateDynamicArray(5, sizeof(const char *), NULL, NULL);
    plugPlatformExtensions(extensions);

    CcoDynamicArray *layers = ccoCreateDynamicArray(2, sizeof(const char *), NULL, NULL);
#ifdef DEBUG
    ccoPushBackDynamicArray(layers, "VK_LAYER_KHRONOS_validation");
#endif

    VkInstanceCreateInfo instanceInfo = {.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                         .pNext = NULL,
                                         .flags = 0,
                                         .pApplicationInfo = &appInfo,
                                         .enabledLayerCount = ccoGetDynamicArrayCount(layers),
                                         .ppEnabledLayerNames = (const char * const *)ccoGetDynamicArrayObjects(layers),
                                         .enabledExtensionCount = ccoGetDynamicArrayCount(extensions),
                                         .ppEnabledExtensionNames = (const char * const *)ccoGetDynamicArrayObjects(extensions)};
#ifdef __APPLE__
    instanceInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
    VkResult result = vkCreateInstance(&instanceInfo, NULL, &core->instance);
    if (result != VK_SUCCESS) {
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }

    ccoDestroyDynamicArray(layers);
    ccoDestroyDynamicArray(extensions);
    return CCO_SUCCESS;
}

static CcoResult findPhysicalDevice(CcoVulkanCore core, const CcoVulkanCoreDesc *desc) {}

CcoResult ccoCreateVulkanCore(const CcoVulkanCoreDesc *desc, CcoVulkanCore *outCore) {
    CcoVulkanCore core = malloc(sizeof(CcoVulkanCore_T));
    if (!core)
        return CCO_FAIL_OUT_OF_MEMORY;
    CcoResult result = createInstance(core, desc);
    if (result != CCO_SUCCESS) {
        CCO_LOG("Failed to create vulkan instance!");
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }

    *outCore = core;
    return CCO_SUCCESS;
}

void ccoDestroyVulkanCore(CcoVulkanCore core) {}
