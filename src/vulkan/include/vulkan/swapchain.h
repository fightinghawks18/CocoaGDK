//
// Created by fightinghawks18 on 12/24/2025.
//

#pragma once

#include "core/core_types.h"
#include "vulkan/instance.h"

typedef struct cco_vulkan_swapchain_t cco_vulkan_swapchain_t;
typedef cco_vulkan_swapchain_t *cco_vulkan_swapchain;

typedef struct {
    void *window_handle;
    u32 window_framebuffer_width;
    u32 window_framebuffer_height;
} cco_vulkan_swapchain_desc;

cco_result cco_create_vulkan_swapchain(cco_vulkan_instance instance, const cco_vulkan_swapchain_desc *desc, cco_vulkan_swapchain *out_swapchain);
void cco_destroy_vulkan_swapchain(cco_vulkan_swapchain swapchain);

cco_result cco_vulkan_swapchain_rebuild(cco_vulkan_swapchain swapchain);
cco_result cco_vulkan_swapchain_prepare(cco_vulkan_swapchain swapchain);
cco_result cco_vulkan_swapchain_present(cco_vulkan_swapchain swapchain);