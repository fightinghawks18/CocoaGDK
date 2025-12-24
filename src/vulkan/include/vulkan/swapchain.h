//
// Created by fightinghawks18 on 12/24/2025.
//

#pragma once

#include "core/core_types.h"

typedef struct cco_vulkan_swapchain_t cco_vulkan_swapchain_t;
typedef cco_vulkan_swapchain_t *cco_vulkan_swapchain;

cco_result cco_create_vulkan_swapchain(void *window_handle, cco_vulkan_swapchain *out_swapchain);
