//
// Created by fightinghawks18 on 12/24/2025.
//

#include <stdlib.h>

#include "math/utilities.h"

#include "../include/vulkan/internal/instance_vulkan.h"
#include "vulkan/internal/swapchain_vulkan.h"

#ifdef _WIN32
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#endif

cco_result create_vulkan_surface(cco_vulkan_instance instance, void *window_handle, cco_vulkan_swapchain swapchain) {
#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR surface_create_info = {};
    surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surface_create_info.hwnd = window_handle;

    const VkResult result =
        vkCreateWin32SurfaceKHR(instance->instance, &surface_create_info, CCO_NIL, &swapchain->surface);
    if (result != VK_SUCCESS) {
        CCO_LOG("Failed to create a vulkan surface for a win32 window!");
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }
#endif
    return CCO_SUCCESS;
}

cco_result find_best_vulkan_surface_format(cco_vulkan_swapchain swapchain, VkSurfaceFormatKHR *out_surface_format) {
    u32 surface_format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(swapchain->instance->physical_device, swapchain->surface,
                                         &surface_format_count, CCO_NIL);
    if (surface_format_count == 0) {
        CCO_LOG("Vulkan failed to find any formats supported for this surface!");
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }

    VkSurfaceFormatKHR *surface_formats = malloc(sizeof(VkSurfaceFormatKHR) * surface_format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(swapchain->instance->physical_device, swapchain->surface,
                                         &surface_format_count, surface_formats);

    for (u32 i = 0; i < surface_format_count; i++) {
        if (surface_formats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
            surface_formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            *out_surface_format = surface_formats[i];
            free(surface_formats);
            return CCO_SUCCESS;
        }
    }

    *out_surface_format = surface_formats[0];
    free(surface_formats);
    return CCO_SUCCESS;
}

cco_result find_best_vulkan_present_mode(cco_vulkan_swapchain swapchain, VkPresentModeKHR *out_present_mode) {
    u32 present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(swapchain->instance->physical_device, swapchain->surface,
                                              &present_mode_count, CCO_NIL);
    if (present_mode_count == 0) {
        CCO_LOG("Vulkan failed to find any presentation modes supported for this surface!");
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }

    VkPresentModeKHR *surface_present_modes = malloc(sizeof(VkPresentModeKHR) * present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(swapchain->instance->physical_device, swapchain->surface,
                                              &present_mode_count, surface_present_modes);

    for (u32 i = 0; i < present_mode_count; i++) {
        if (surface_present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            *out_present_mode = surface_present_modes[i];
            free(surface_present_modes);
            return CCO_SUCCESS;
        }
    }

    free(surface_present_modes);
    *out_present_mode = VK_PRESENT_MODE_FIFO_KHR;
    return CCO_SUCCESS;
}

VkExtent2D find_best_vulkan_extent(const VkSurfaceCapabilitiesKHR *surface_capabilities, const u32 framebuffer_width,
                                   const u32 framebuffer_height) {
    // Could be UINT32_MAX
    // That means the surface's size is controlled by the platform (Windows, MacOS, etc..)
    if (surface_capabilities->currentExtent.width != UINT32_MAX) {
        return surface_capabilities->currentExtent;
    }

    // This allows us to be flexible with the surface's extent
    VkExtent2D actual_extent = {.width = framebuffer_width, .height = framebuffer_height};

    actual_extent.width = CCO_CLAMP(actual_extent.width, surface_capabilities->minImageExtent.width,
                                    surface_capabilities->maxImageExtent.width);
    actual_extent.height = CCO_CLAMP(actual_extent.height, surface_capabilities->minImageExtent.height,
                                     surface_capabilities->maxImageExtent.height);

    return actual_extent;
}

cco_result create_vulkan_swapchain(const cco_vulkan_swapchain_desc *desc, cco_vulkan_swapchain swapchain) {
    VkSurfaceCapabilitiesKHR surface_capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(swapchain->instance->physical_device, swapchain->surface,
                                              &surface_capabilities);

    VkSurfaceFormatKHR surface_format;
    const cco_result get_surface_format_result = find_best_vulkan_surface_format(swapchain, &surface_format);
    if (get_surface_format_result != CCO_SUCCESS)
        return get_surface_format_result;

    VkPresentModeKHR presentation_mode;
    const cco_result get_presentation_mode_result = find_best_vulkan_present_mode(swapchain, &presentation_mode);
    if (get_presentation_mode_result != CCO_SUCCESS)
        return get_presentation_mode_result;

    const VkExtent2D extent =
        find_best_vulkan_extent(&surface_capabilities, desc->window_framebuffer_width, desc->window_framebuffer_height);

    // If triple buffer is available, use it
    u32 image_count = surface_capabilities.minImageCount + 1;
    if (surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount) {
        image_count = surface_capabilities.maxImageCount;
    }

    // This function may be called after creation,
    // this is just for recreation if this swapchain is out of date
    VkSwapchainKHR old_swapchain = swapchain->swapchain;

    VkSwapchainCreateInfoKHR swapchain_create_info = {};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.presentMode = presentation_mode;
    swapchain_create_info.surface = swapchain->surface;
    swapchain_create_info.minImageCount = image_count;
    swapchain_create_info.imageFormat = surface_format.format;
    swapchain_create_info.imageColorSpace = surface_format.colorSpace;
    swapchain_create_info.imageExtent = extent;
    swapchain_create_info.imageArrayLayers = 1; // Single layer, maybe allow customization for cases like VR?
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount = 0;
    swapchain_create_info.pQueueFamilyIndices = CCO_NIL;
    swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.preTransform = surface_capabilities.currentTransform;
    swapchain_create_info.clipped = VK_TRUE;
    swapchain_create_info.oldSwapchain = old_swapchain;

    const VkResult result =
        vkCreateSwapchainKHR(swapchain->instance->device, &swapchain_create_info, CCO_NIL, &swapchain->swapchain);
    if (result != VK_SUCCESS) {
        CCO_LOG("Failed to create a swapchain!");
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }

    // Dispose of the old swapchain if one existed
    if (old_swapchain) {
        vkDestroySwapchainKHR(swapchain->instance->device, old_swapchain, CCO_NIL);
    }

    swapchain->extent = extent;
    swapchain->presentation_mode = presentation_mode;
    swapchain->surface_format = surface_format;

    return CCO_SUCCESS;
}

cco_result create_vulkan_swapchain_images(cco_vulkan_swapchain swapchain) {
    u32 image_count = 0;
    vkGetSwapchainImagesKHR(swapchain->instance->device, swapchain->swapchain, &image_count, CCO_NIL);
    if (image_count == 0) {
        CCO_LOG("Vulkan failed to find any images for this swapchain!");
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }

    VkImage *swapchain_images = malloc(sizeof(VkImage) * image_count);
    vkGetSwapchainImagesKHR(swapchain->instance->device, swapchain->swapchain, &image_count, swapchain_images);

    swapchain->swapchain_images = swapchain_images;
    swapchain->swapchain_images_count = image_count;

    swapchain->swapchain_image_views = malloc(sizeof(VkImageView) * image_count);
    for (u32 i = 0; i < image_count; i++) {
        VkImageViewCreateInfo image_view_create_info = {};
        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = swapchain->surface_format.format;
        image_view_create_info.image = swapchain_images[i];
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;

        const VkResult result = vkCreateImageView(swapchain->instance->device, &image_view_create_info, CCO_NIL, &swapchain->swapchain_image_views[i]);
        if (result != VK_SUCCESS) {
            CCO_LOG("Vulkan failed to create a corresponding image view for swapchain image %d!", i);
            return CCO_FAIL_GRAPHICS_CREATE_ERROR;
        }
    }
    return CCO_SUCCESS;
}

cco_result create_vulkan_sync_objects(cco_vulkan_swapchain swapchain) {
    swapchain->semaphore_count = 2;
    swapchain->fence_count = 2;

    swapchain->image_available_semaphores = malloc(sizeof(VkSemaphore) * swapchain->semaphore_count);
    swapchain->render_finished_semaphores = malloc(sizeof(VkSemaphore) * swapchain->semaphore_count);

    swapchain->in_flight_fences = malloc(sizeof(VkFence) * swapchain->fence_count);

    for (u32 i = 0; i < swapchain->semaphore_count; i++) {
        VkSemaphoreCreateInfo sync_semaphore_create_info = {};
        sync_semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        const VkResult image_available_semaphore_create_result = vkCreateSemaphore(swapchain->instance->device, &sync_semaphore_create_info, NULL, &swapchain->image_available_semaphores[i]);
        if (image_available_semaphore_create_result != VK_SUCCESS) {
            CCO_LOG("Vulkan failed to create image available semaphore %d!", i);
            return CCO_FAIL_GRAPHICS_CREATE_ERROR;
        }

        const VkResult render_finished_semaphore_create_result = vkCreateSemaphore(swapchain->instance->device, &sync_semaphore_create_info, NULL, &swapchain->render_finished_semaphores[i]);
        if (render_finished_semaphore_create_result != VK_SUCCESS) {
            CCO_LOG("Vulkan failed to create render finished semaphore %d!", i);
            return CCO_FAIL_GRAPHICS_CREATE_ERROR;
        }
    }

    for (u32 i = 0; i < swapchain->fence_count; i++) {
        VkFenceCreateInfo sync_fence_create_info = {};
        sync_fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        sync_fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        const VkResult in_flight_fence_create_result = vkCreateFence(swapchain->instance->device, &sync_fence_create_info, NULL, &swapchain->in_flight_fences[i]);
        if (in_flight_fence_create_result != VK_SUCCESS) {
            CCO_LOG("Vulkan failed to create flight fence %d!", i);
            return CCO_FAIL_GRAPHICS_CREATE_ERROR;
        }
    }
    return CCO_SUCCESS;
}

cco_result cco_create_vulkan_swapchain(cco_vulkan_instance instance, const cco_vulkan_swapchain_desc *desc,
                                       cco_vulkan_swapchain *out_swapchain) {
    cco_vulkan_swapchain swapchain = malloc(sizeof(cco_vulkan_swapchain_t));
    if (!swapchain)
        return CCO_FAIL_OUT_OF_MEMORY;

    swapchain->desc = desc;
    swapchain->swapchain_images = CCO_NIL;
    swapchain->swapchain_image_views = CCO_NIL;
    swapchain->image_available_semaphores = CCO_NIL;
    swapchain->render_finished_semaphores = CCO_NIL;
    swapchain->in_flight_fences = CCO_NIL;
    swapchain->surface = CCO_NIL;
    swapchain->swapchain = CCO_NIL;
    swapchain->instance = instance;
    swapchain->frame_count = 0;
    swapchain->image_index = 0;
    swapchain->frame_active = CCO_NO;
    swapchain->submitted = CCO_NO;

    const cco_result surface_create_result = create_vulkan_surface(instance, desc->window_handle, swapchain);
    if (surface_create_result != CCO_SUCCESS) {
        cco_destroy_vulkan_swapchain(swapchain);
        return surface_create_result;
    }

    const cco_result swapchain_create_result = create_vulkan_swapchain(desc, swapchain);
    if (swapchain_create_result != CCO_SUCCESS) {
        cco_destroy_vulkan_swapchain(swapchain);
        return swapchain_create_result;
    }

    const cco_result swapchain_create_images_result = create_vulkan_swapchain_images(swapchain);
    if (swapchain_create_images_result != CCO_SUCCESS) {
        cco_destroy_vulkan_swapchain(swapchain);
        return swapchain_create_images_result;
    }

    const cco_result sync_objects_create_result = create_vulkan_sync_objects(swapchain);
    if (sync_objects_create_result != CCO_SUCCESS) {
        cco_destroy_vulkan_swapchain(swapchain);
        return sync_objects_create_result;
    }

    *out_swapchain = swapchain;
    return CCO_SUCCESS;
}

void destroy_vulkan_swapchain_images(cco_vulkan_swapchain swapchain) {
    if (swapchain->swapchain_image_views) {
        for (u32 i = 0; i < swapchain->swapchain_images_count; i++) {
            vkDestroyImageView(swapchain->instance->device, swapchain->swapchain_image_views[i], CCO_NIL);
        }
        free(swapchain->swapchain_image_views);
        swapchain->swapchain_image_views = CCO_NIL;
    }

    if (swapchain->swapchain_images) {
        free(swapchain->swapchain_images);
        swapchain->swapchain_images = CCO_NIL;
    }
}

void destroy_vulkan_sync_objects(cco_vulkan_swapchain swapchain) {
    if (swapchain->in_flight_fences) {
        for (u32 i = 0; i < swapchain->fence_count; i++) {
            vkDestroyFence(swapchain->instance->device, swapchain->in_flight_fences[i], CCO_NIL);
        }
        free(swapchain->in_flight_fences);
        swapchain->in_flight_fences = CCO_NIL;
    }

    if (swapchain->image_available_semaphores) {
        for (u32 i = 0; i < swapchain->semaphore_count; i++) {
            vkDestroySemaphore(swapchain->instance->device, swapchain->image_available_semaphores[i], CCO_NIL);
        }
        free(swapchain->image_available_semaphores);
        swapchain->image_available_semaphores = CCO_NIL;
    }

    if (swapchain->render_finished_semaphores) {
        for (u32 i = 0; i < swapchain->semaphore_count; i++) {
            vkDestroySemaphore(swapchain->instance->device, swapchain->render_finished_semaphores[i], CCO_NIL);
        }
        free(swapchain->render_finished_semaphores);
        swapchain->render_finished_semaphores = CCO_NIL;
    }
}

void cco_destroy_vulkan_swapchain(cco_vulkan_swapchain swapchain) {
    destroy_vulkan_sync_objects(swapchain);
    destroy_vulkan_swapchain_images(swapchain);

    if (swapchain->swapchain) {
        vkDestroySwapchainKHR(swapchain->instance->device, swapchain->swapchain, CCO_NIL);
        swapchain->swapchain = CCO_NIL;
    }

    if (swapchain->surface) {
        vkDestroySurfaceKHR(swapchain->instance->instance, swapchain->surface, CCO_NIL);
        swapchain->surface = CCO_NIL;
    }

    free(swapchain);
}

cco_result cco_vulkan_swapchain_rebuild(cco_vulkan_swapchain swapchain) {
    destroy_vulkan_sync_objects(swapchain);
    destroy_vulkan_swapchain_images(swapchain);

    const cco_result rebuild_swapchain_result = create_vulkan_swapchain(swapchain->desc, swapchain);
    if (rebuild_swapchain_result != CCO_SUCCESS) {
        cco_destroy_vulkan_swapchain(swapchain);
        return rebuild_swapchain_result;
    }

    const cco_result rebuild_swapchain_images_result = create_vulkan_swapchain_images(swapchain);
    if (rebuild_swapchain_images_result != CCO_SUCCESS) {
        cco_destroy_vulkan_swapchain(swapchain);
        return rebuild_swapchain_images_result;
    }

    const cco_result rebuild_sync_objects_result = create_vulkan_sync_objects(swapchain);
    if (rebuild_sync_objects_result != CCO_SUCCESS) {
        cco_destroy_vulkan_swapchain(swapchain);
        return rebuild_sync_objects_result;
    }

    return CCO_SUCCESS;
}

cco_result cco_vulkan_swapchain_prepare(cco_vulkan_swapchain swapchain) {
    const VkResult wait_for_frame_result = vkWaitForFences(swapchain->instance->device, 1, &swapchain->in_flight_fences[swapchain->frame_count], VK_TRUE, UINT64_MAX);
    if (wait_for_frame_result != VK_SUCCESS) {
        CCO_LOG("Vulkan failed to wait for this frame's fence");
        return CCO_FAIL_GRAPHICS_FRAME_START_ERROR;
    }

    vkResetFences(swapchain->instance->device, 1, &swapchain->in_flight_fences[swapchain->frame_count]);

    const VkResult get_next_image_index_result = vkAcquireNextImageKHR(swapchain->instance->device, swapchain->swapchain, UINT64_MAX, swapchain->image_available_semaphores[swapchain->frame_count], NULL, &swapchain->image_index);
    if (get_next_image_index_result == VK_ERROR_OUT_OF_DATE_KHR) {
        const cco_result rebuild_swapchain_result = cco_vulkan_swapchain_rebuild(swapchain);
        if (rebuild_swapchain_result != CCO_SUCCESS) {
            CCO_LOG("Vulkan failed to rebuild swapchain for this frame!");
            return rebuild_swapchain_result;
        }
        CCO_LOG("Vulkan swapchain is out of date on frame %d, rebuilding resources...", swapchain->frame_count);
        return CCO_GRAPHICS_REBUILD_SWAPCHAIN;
    } else if (get_next_image_index_result != VK_SUCCESS) {
        CCO_LOG("Vulkan failed to acquire next image index");
        return CCO_FAIL_GRAPHICS_FRAME_START_ERROR;
    }

    swapchain->frame_active = CCO_YES;
    return CCO_SUCCESS;
}

cco_result cco_vulkan_swapchain_submit(cco_vulkan_swapchain swapchain, VkCommandBuffer buffer) {
    if (!swapchain->frame_active) {
        CCO_LOG("Vulkan is attempting to submit swapchain (%p), but isn't currently active!\n Hint: Run cco_vulkan_swapchain_prepare before submission!", swapchain);
        return CCO_FAIL_GRAPHICS_FRAME_SUBMIT_ERROR;
    }

    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &buffer;

    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &swapchain->render_finished_semaphores[swapchain->frame_count];

    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &swapchain->image_available_semaphores[swapchain->frame_count];
    submit_info.pWaitDstStageMask = &wait_stage;

    const VkResult submit_result = vkQueueSubmit(swapchain->instance->graphics_queue, 1, &submit_info, swapchain->in_flight_fences[swapchain->frame_count]);
    if (submit_result != VK_SUCCESS) {
        CCO_LOG("Vulkan failed to submit command buffer to swapchain (%p) on frame %d", swapchain, swapchain->frame_count);
        return CCO_FAIL_GRAPHICS_FRAME_SUBMIT_ERROR;
    }

    swapchain->submitted = CCO_YES;
    return CCO_SUCCESS;
}

cco_result cco_vulkan_swapchain_present(cco_vulkan_swapchain swapchain) {
    if (!swapchain->submitted) {
        CCO_LOG("Vulkan is attempting to present swapchain (%p), but hasn't submitted yet!\n Hint: Run cco_vulkan_swapchain_submit via swapchain_vulkan.h before presentation!", swapchain);
        return CCO_FAIL_GRAPHICS_FRAME_PRESENT_ERROR;
    }

    VkPresentInfoKHR present_info = {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &swapchain->render_finished_semaphores[swapchain->frame_count];
    present_info.pImageIndices = &swapchain->image_index;
    present_info.pResults = NULL;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain->swapchain;

    const VkResult present_result = vkQueuePresentKHR(swapchain->instance->graphics_queue, &present_info);
    if (present_result != VK_SUCCESS) {
        CCO_LOG("Vulkan failed to present swapchain (%p)!", swapchain);
        return CCO_FAIL_GRAPHICS_FRAME_PRESENT_ERROR;
    }

    swapchain->frame_active = CCO_NO;
    swapchain->submitted = CCO_NO;
    swapchain->frame_count = (swapchain->frame_count + 1) % 2;
    return CCO_SUCCESS;
}
