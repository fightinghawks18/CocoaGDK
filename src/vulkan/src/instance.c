//
// Created by fightinghawks18 on 12/24/2025.
//

#include <stdlib.h>

#include "vulkan/instance.h"
#include "vulkan/instance_vulkan.h"

u32 get_platform_extensions(const char ***outExtensions) {
    static const char *extensions[] = {
        "VK_KHR_surface",
        "VK_KHR_swapchain",
#ifdef _WIN32
        "VK_KHR_win32_surface",
#endif
    };

    *outExtensions = extensions;
    return sizeof(extensions) / sizeof(extensions[0]);
}

cco_result create_vulkan_instance(const cco_vulkan_instance_desc *desc, cco_vulkan_instance instance) {
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.applicationVersion = VK_MAKE_API_VERSION(0, desc->appVersion[0], desc->appVersion[1], desc->appVersion[2]);
    app_info.pApplicationName = desc->appName;
    app_info.engineVersion =
        VK_MAKE_API_VERSION(0, desc->engineVersion[0], desc->engineVersion[1], desc->engineVersion[2]);
    app_info.pEngineName = desc->engineName;
    app_info.engineVersion = VK_MAKE_API_VERSION(0, 1, 3, 0);

    const char **platform_exts;
    const u32 platform_exts_count = get_platform_extensions(&platform_exts);

    const char *const layers[] = {
#ifdef DEBUG
        "VK_LAYER_KHRONOS_validation"
#endif
    };

    VkInstanceCreateInfo instance_create_info = {};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &app_info;
    instance_create_info.enabledExtensionCount = platform_exts_count;
    instance_create_info.ppEnabledExtensionNames = platform_exts;
    instance_create_info.enabledLayerCount = sizeof(layers) / sizeof(layers[0]);
    instance_create_info.ppEnabledLayerNames = layers;

    const VkResult result = vkCreateInstance(&instance_create_info, NULL, &instance->instance);
    if (result != VK_SUCCESS) {
        CCO_LOG("Vulkan instance creation failed!");
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }
    return CCO_SUCCESS;
}

u32 get_device_type_score(VkPhysicalDeviceProperties device_props, cco_vulkan_power_preference power_preference) {
    switch (device_props.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
        if (power_preference == CCO_VULKAN_POWER_HIGH) {
            return 1000;
        }
        return 10;
    }
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
        if (power_preference == CCO_VULKAN_POWER_HIGH) {
            return 350;
        }
        return 450;
    }
    case VK_PHYSICAL_DEVICE_TYPE_CPU: {
        if (power_preference == CCO_VULKAN_POWER_HIGH) {
            return 10;
        }
        return 350;
    }
    default:
        return 0;
    }
}

cco_result find_vulkan_physical_device(const cco_vulkan_instance_desc *desc, cco_vulkan_instance instance) {
    u32 physical_device_count = 0;
    vkEnumeratePhysicalDevices(instance->instance, &physical_device_count, NULL);
    if (physical_device_count == 0) {
        CCO_LOG("Failed to find any vulkan physical devices!");
        return CCO_FAIL_GRAPHICS_NO_ADAPTERS_ERROR;
    }

    VkPhysicalDevice *physical_devices = malloc(sizeof(VkPhysicalDevice) * physical_device_count);
    vkEnumeratePhysicalDevices(instance->instance, &physical_device_count, physical_devices);

    u32 high_score = 0;
    VkPhysicalDevice high_device = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties high_device_props;
    VkPhysicalDeviceFeatures high_device_features;
    for (u32 i = 0; i < physical_device_count; i++) {
        u32 score = 0;
        VkPhysicalDeviceProperties device_props;
        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceProperties(physical_devices[i], &device_props);
        vkGetPhysicalDeviceFeatures(physical_devices[i], &device_features);

        score += get_device_type_score(device_props, CCO_VULKAN_POWER_HIGH);

        if (score > high_score) {
            high_device = physical_devices[i];
            high_device_props = device_props;
            high_device_features = device_features;
            high_score = score;
        }
    }

    free(physical_devices);
    if (high_device == VK_NULL_HANDLE) {
        CCO_LOG("Ranking for most suitable vulkan physical device returned no physical device!");
        return CCO_FAIL_GRAPHICS_NO_SUITABLE_ADAPTERS_ERROR;
    }

    instance->physical_device = high_device;
    instance->device_features = high_device_features;
    instance->device_props = high_device_props;
    return CCO_SUCCESS;
}

cco_result discover_vulkan_queues(cco_vulkan_instance instance) {
    instance->graphics_family_index = UINT32_MAX;
    instance->transfer_family_index = UINT32_MAX;
    instance->compute_family_index = UINT32_MAX;

    u32 queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(instance->physical_device, &queue_family_count, NULL);

    VkQueueFamilyProperties *queue_families = malloc(sizeof(VkQueueFamilyProperties) * queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(instance->physical_device, &queue_family_count, queue_families);

    for (u32 i = 0; i < queue_family_count; i++) {
        if (instance->graphics_family_index == UINT32_MAX && queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            instance->graphics_family_index = i;

        if (instance->compute_family_index == UINT32_MAX && queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            instance->compute_family_index = i;

        if (instance->transfer_family_index == UINT32_MAX && queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            instance->transfer_family_index = i;
    }

    free(queue_families);
    if (instance->graphics_family_index == UINT32_MAX) {
        CCO_LOG("Failed to find a vulkan graphics queue!");
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }

    return CCO_SUCCESS;
}

void add_queue_family(VkDeviceQueueCreateInfo *queue_infos, u32 *queue_count, const u32 family_index) {
    if (family_index == UINT32_MAX)
        return;
    for (u32 i = 0; i < 3; i++) {
        VkDeviceQueueCreateInfo *queue_create_info = &queue_infos[i];
        if (queue_create_info->queueFamilyIndex != family_index)
            continue;
        queue_create_info->queueCount++;
    }

    VkDeviceQueueCreateInfo *device_queue_create_info = &queue_infos[*queue_count];
    device_queue_create_info->queueCount = 1;
    device_queue_create_info->queueFamilyIndex = family_index;
    (*queue_count)++;
}

void load_queue_family(VkDevice device, const u32 queue_family, const u32 queue_index, VkQueue *out_queue) {
    vkGetDeviceQueue(device, queue_family, queue_index, out_queue);
}

cco_result create_vulkan_device(cco_vulkan_instance instance) {
    const cco_result discover_queues_result = discover_vulkan_queues(instance);
    if (discover_queues_result != CCO_SUCCESS)
        return discover_queues_result;

    const f32 priority = 1.0f;
    VkDeviceQueueCreateInfo queue_infos[3];
    u32 queue_count = 0;

    queue_infos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_infos[0].pQueuePriorities = &priority;
    queue_infos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_infos[1].pQueuePriorities = &priority;
    queue_infos[2].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_infos[2].pQueuePriorities = &priority;

    add_queue_family(queue_infos, &queue_count, instance->graphics_family_index);
    add_queue_family(queue_infos, &queue_count, instance->transfer_family_index);
    add_queue_family(queue_infos, &queue_count, instance->compute_family_index);

    const char *device_extensions[] = {
        "VK_KHR_swapchain"
    };

    VkPhysicalDeviceFeatures device_features = {0};

    VkDeviceCreateInfo device_create_info = {};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext = NULL;
    device_create_info.pQueueCreateInfos = queue_infos;
    device_create_info.queueCreateInfoCount = queue_count;
    device_create_info.pEnabledFeatures = &device_features;
    device_create_info.enabledExtensionCount = 1;
    device_create_info.ppEnabledExtensionNames = device_extensions;

    const VkResult result = vkCreateDevice(instance->physical_device, &device_create_info, NULL, &instance->device);
    if (result != VK_SUCCESS) {
        CCO_LOG("Failed to create vulkan device!");
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }

    load_queue_family(instance->device, instance->graphics_family_index, 0, &instance->graphics_queue);
    load_queue_family(instance->device, instance->transfer_family_index, 0, &instance->transfer_queue);
    load_queue_family(instance->device, instance->compute_family_index, 0, &instance->compute_queue);

    return CCO_SUCCESS;
}

cco_result cco_create_vulkan_instance(const cco_vulkan_instance_desc *desc, cco_vulkan_instance *out_instance) {
    cco_vulkan_instance instance = malloc(sizeof(cco_vulkan_instance_t));
    if (!instance)
        return CCO_FAIL_OUT_OF_MEMORY;

    const cco_result instance_result = create_vulkan_instance(desc, instance);
    if (instance_result != CCO_SUCCESS) {
        cco_destroy_vulkan_instance(instance);
        return instance_result;
    }

    const cco_result physical_device_result = find_vulkan_physical_device(desc, instance);
    if (physical_device_result != CCO_SUCCESS) {
        cco_destroy_vulkan_instance(instance);
        return physical_device_result;
    }

    const cco_result device_creation = create_vulkan_device(instance);
    if (device_creation != CCO_SUCCESS) {
        cco_destroy_vulkan_instance(instance);
        return instance_result;
    }

    *out_instance = instance;
    return CCO_SUCCESS;
}

void cco_destroy_vulkan_instance(cco_vulkan_instance instance) {
    if (instance->device) {
        vkDestroyDevice(instance->device, NULL);
        instance->device = NULL;
    }

    if (instance->instance) {
        vkDestroyInstance(instance->instance, NULL);
        instance->instance = NULL;
    }
    free(instance);
}
