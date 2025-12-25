//
// Created by fightinghawks18 on 12/24/2025.
//

#pragma once

#include <vulkan/vulkan.h>

#include "../swapchain.h"
#include "instance_vulkan.h"

struct cco_vulkan_swapchain_t {
    cco_vulkan_instance instance;

    const cco_vulkan_swapchain_desc *desc;

    VkSwapchainKHR swapchain;
    VkSurfaceKHR surface;
    VkExtent2D extent;
    VkPresentModeKHR presentation_mode;
    VkSurfaceFormatKHR surface_format;

    VkSemaphore *image_available_semaphores;
    VkSemaphore *render_finished_semaphores;
    u32 semaphore_count;

    VkFence *in_flight_fences;
    u32 fence_count;

    VkImage *swapchain_images;
    u32 swapchain_images_count;
    VkImageView *swapchain_image_views;

    u32 frame_count;
    u32 image_index;

    cco_bool submitted;
    cco_bool frame_active;
};

cco_result cco_vulkan_swapchain_submit(cco_vulkan_swapchain swapchain, VkCommandBuffer buffer);