//
// Created by fightinghawks18 on 12/12/25.
//

#include "core.h"
#include "../tools/dynamic_array.h"
#include <stdlib.h>
#include <vulkan/vulkan.h>

typedef struct CcoVulkanCore_T {
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;

    CcoDynamicArray *queues;
} CcoVulkanCore_T;

typedef struct {
    CcoGPUQueueTypeFlags queueType;
    u32 familyIndex;
    u32 queueCount;
    VkQueue *queues;
} CcoVulkanQueueObject;

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
                                         .ppEnabledLayerNames = (const char *const *)ccoGetDynamicArrayObjects(layers),
                                         .enabledExtensionCount = ccoGetDynamicArrayCount(extensions),
                                         .ppEnabledExtensionNames =
                                             (const char *const *)ccoGetDynamicArrayObjects(extensions)};
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

static u32 determinePointsOnGPUPower(VkPhysicalDevice device, CcoGPUPowerPreference powerPreference) {
    u32 points = 0;
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    switch (powerPreference) {
    case CCO_GPU_POWER_PREFERENCE_LOW: {
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
            points += 1000;
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            points += 500;
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            points += 10;
    }
    case CCO_GPU_POWER_PREFERENCE_MED: {
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
            points += 10;
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            points += 1000;
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            points += 500;
    }
    case CCO_GPU_POWER_PREFERENCE_HI: {
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
            points += 10;
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            points += 500;
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            points += 1000;
    }
    }

    return points;
}

static CcoResult findPhysicalDevice(CcoVulkanCore core, const CcoVulkanCoreDesc *desc) {
    u32 count = 0;
    vkEnumeratePhysicalDevices(core->instance, &count, NULL);
    if (count == 0) {
        CCO_LOG("Failed to find any available adapters!");
        return CCO_FAIL_GRAPHICS_NO_ADAPTERS_ERROR;
    }

    VkPhysicalDevice *physicalDevices = malloc(count * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(core->instance, &count, physicalDevices);
    CCO_LOG("Vulkan loaded %d adapter%s.", count, count > 1 ? "s" : "");

    u32 highestScore = 0;
    VkPhysicalDevice highestDevice = VK_NULL_HANDLE;
    for (u32 i = 0; i < count; i++) {
        VkPhysicalDevice device = physicalDevices[i];
        u32 score = determinePointsOnGPUPower(device, desc->gpuPowerPreference);
        if (score > highestScore) {
            highestScore = score;
            highestDevice = device;
        }
    }

    free(physicalDevices);
    if (highestDevice == VK_NULL_HANDLE) {
        CCO_LOG("Failed to get any suitable adapters!");
        return CCO_FAIL_GRAPHICS_NO_SUITABLE_ADAPTERS_ERROR;
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(highestDevice, &deviceProperties);

    CCO_LOG("Found adapter %s!", deviceProperties.deviceName);
    core->physicalDevice = highestDevice;
    return CCO_SUCCESS;
}

static void checkQueueSupport(const VkQueueFamilyProperties *queueFamilies, u32 index, VkQueueFlags queueFlags,
                              CcoGPUQueueType queueType, CcoDynamicArray *queues) {
    if (!queueFamilies)
        return;
    for (u32 i = 0; i < ccoGetDynamicArrayCount(queues); i++) {
        CcoVulkanQueueObject *queueObject = (CcoVulkanQueueObject *)ccoGetDynamicArrayObject(queues, i);

        // Avoid adding duplicate queue types
        if (queueObject->queueType & queueType)
            return;

        // Only append queue type if the queue family was inserted
        if (queueObject->familyIndex == index) {
            queueObject->queueType |= queueType;
            return;
        }
    }

    // Check if the queue in that family has the specific queue type
    VkQueueFamilyProperties queue = queueFamilies[index];
    if (queue.queueFlags & queueFlags) {
        CcoVulkanQueueObject *queueObject = malloc(sizeof(CcoVulkanQueueObject));
        queueObject->queueCount = queue.queueCount;
        queueObject->familyIndex = index;
        queueObject->queueType = queueType;
        queueObject->queues = NULL;
        ccoPushBackDynamicArray(queues, queueObject);
    }
}

static CcoResult getDesiredQueues(CcoVulkanCore core, const CcoVulkanCoreDesc *desc) {
    CcoDynamicArray *queues = ccoCreateDynamicArray(3, sizeof(CcoVulkanQueueObject), NULL, NULL);

    u32 count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(core->physicalDevice, &count, NULL);
    if (count == 0) {
        CCO_LOG("Vulkan failed to find any queue families");
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }

    VkQueueFamilyProperties *queueFamilies = malloc(count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(core->physicalDevice, &count, queueFamilies);

    for (u32 i = 0; i < desc->desiredQueueCount; i++) {
        VkQueueFlags queueFlags = 0;
        switch (desc->desiredQueues[i]) {
        case CCO_GPU_QUEUE_GRAPHICS:
            queueFlags |= VK_QUEUE_GRAPHICS_BIT;
            break;
        case CCO_GPU_QUEUE_COMPUTE:
            queueFlags |= VK_QUEUE_COMPUTE_BIT;
            break;
        case CCO_GPU_QUEUE_TRANSFER:
            queueFlags |= VK_QUEUE_TRANSFER_BIT;
            break;
        default:
            break;
        }

        // Look at every queue family and check for support of this queueType
        for (u32 j = 0; j < count; j++) {
            checkQueueSupport(queueFamilies, j, queueFlags, desc->desiredQueues[i], queues);
        }
    }

    free(queueFamilies);
    if (ccoGetDynamicArrayCount(queues) == 0) {
        CCO_LOG("Vulkan failed to find desired queues!");
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }

    core->queues = queues;
    return CCO_SUCCESS;
}

static CcoResult createDevice(CcoVulkanCore core, const CcoVulkanCoreDesc *desc) {
    CcoDynamicArray *extensions = ccoCreateDynamicArray(2, sizeof(const char *), NULL, NULL);
#ifdef __APPLE__
    ccoPushBackDynamicArray(extensions, "VK_KHR_portability_subset");
#endif
    ccoPushBackDynamicArray(extensions, "VK_KHR_swapchain");

    CcoResult getQueues = getDesiredQueues(core, desc);
    if (getQueues != CCO_SUCCESS) {
        CCO_LOG("Vulkan failed to get queues");
        return getQueues;
    }

    float priorities = 1.0f;
    VkDeviceQueueCreateInfo *queueInfos = malloc(ccoGetDynamicArrayCount(core->queues) * sizeof(VkDeviceQueueCreateInfo));
    for (u32 i = 0; i < ccoGetDynamicArrayCount(core->queues); i++) {
        CcoVulkanQueueObject *queueObject = (CcoVulkanQueueObject *)ccoGetDynamicArrayObject(core->queues, i);

        VkDeviceQueueCreateInfo queueInfo = {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                             .pNext = NULL,
                                             .flags = 0,
                                             .queueFamilyIndex = queueObject->familyIndex,
                                             .queueCount = queueObject->queueCount,
                                             .pQueuePriorities = &priorities};
        queueInfos[i] = queueInfo;
    }

    const VkDeviceCreateInfo deviceInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .queueCreateInfoCount = ccoGetDynamicArrayCount(core->queues),
        .pQueueCreateInfos = queueInfos,
        .enabledExtensionCount = ccoGetDynamicArrayCount(extensions),
        .ppEnabledExtensionNames = (const char *const *)ccoGetDynamicArrayObjects(extensions)};

    const VkResult result = vkCreateDevice(core->physicalDevice, &deviceInfo, NULL, &core->device);
    if (result != VK_SUCCESS) {
        CCO_LOG("Failed to create vulkan device!");
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }

    for (u32 i = 0; i < ccoGetDynamicArrayCount(core->queues); i++) {
        CcoVulkanQueueObject *queueObject = (CcoVulkanQueueObject *)ccoGetDynamicArrayObject(core->queues, i);
        queueObject->queues = malloc(queueObject->queueCount * sizeof(VkQueue));
        for (u32 j = 0; j < queueObject->queueCount; j++) {
            queueObject->queues[j] = VK_NULL_HANDLE;
            vkGetDeviceQueue(core->device, queueObject->familyIndex, j, &queueObject->queues[j]);
        }
    }

    const u32 queueCount = ccoGetDynamicArrayCount(core->queues);
    CCO_LOG("Vulkan created %d queue%s!", queueCount, queueCount > 1 ? "s" : "");

    ccoDestroyDynamicArray(extensions);
    return CCO_SUCCESS;
}

CcoResult ccoCreateVulkanCore(const CcoVulkanCoreDesc *desc, CcoVulkanCore *outCore) {
    CcoVulkanCore core = malloc(sizeof(CcoVulkanCore_T));
    if (!core)
        return CCO_FAIL_OUT_OF_MEMORY;
    CcoResult result = createInstance(core, desc);
    if (result != CCO_SUCCESS) {
        CCO_LOG("Failed to create vulkan instance!");
        return result;
    }

    result = findPhysicalDevice(core, desc);
    if (result != CCO_SUCCESS) {
        CCO_LOG("Failed to find a suitable vulkan physical device!");
        return result;
    }

    result = createDevice(core, desc);
    if (result != CCO_SUCCESS) {
        CCO_LOG("Failed to create a vulkan device!");
        return result;
    }

    *outCore = core;
    return CCO_SUCCESS;
}

void ccoDestroyVulkanCore(CcoVulkanCore core) {
    for (u32 i = 0; i < ccoGetDynamicArrayCount(core->queues); i++) {
        CcoVulkanQueueObject *queueObject = (CcoVulkanQueueObject *)ccoGetDynamicArrayObject(core->queues, i);
        free(queueObject->queues);
        free(queueObject);
    }
    ccoDestroyDynamicArray(core->queues);

    if (core->device) {
        vkDestroyDevice(core->device, NULL);
    }

    if (core->instance) {
        vkDestroyInstance(core->instance, NULL);
    }

    free(core);
}

VkInstance ccoGetVulkanCoreInstance(CcoVulkanCore core) { return core->instance; }
VkPhysicalDevice ccoGetVulkanCorePhysicalDevice(CcoVulkanCore core) { return core->physicalDevice; }
VkDevice ccoGetVulkanCoreDevice(CcoVulkanCore core) { return core->device; }
