//
// Created by fightinghawks18 on 12/24/2025.
//

#pragma once

#include "core/core_types.h"
#include "core/results.h"
#include "core/log.h"
#include "export.h"

typedef struct cco_vulkan_instance_t cco_vulkan_instance_t;
typedef cco_vulkan_instance_t *cco_vulkan_instance;

typedef enum {
    CCO_VULKAN_POWER_HIGH,
    CCO_VULKAN_POWER_LOW
} cco_vulkan_power_preference;

typedef struct {
    cco_vulkan_power_preference power_preference;
    const char *app_name;
    const char *engine_name;
    u32 app_version[3];
    u32 engine_version[3];
} cco_vulkan_instance_desc;

CCO_VULKAN_API cco_result cco_create_vulkan_instance(const cco_vulkan_instance_desc *desc, cco_vulkan_instance *out_instance);
CCO_VULKAN_API void cco_destroy_vulkan_instance(cco_vulkan_instance instance);