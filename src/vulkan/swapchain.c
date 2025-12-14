#include "swapchain.h"

#include "../tools/dynamic_array.h"

#include <stdlib.h>

typedef struct CcoVulkanSwapChain_T {
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;

    VkImage *images;
    u32 imageCount;
    CcoDynamicArray *imageViews;
    CcoDynamicArray *fences;
    CcoDynamicArray *semaphores;
} CcoVulkanSwapChain_T;

CcoResult ccoCreateVulkanSwapChain(const CcoVulkanSwapChainDesc *desc, CcoVulkanSwapChain *outSwapChain) {
    CcoVulkanSwapChain swapChain = malloc(sizeof(CcoVulkanSwapChain_T));
    if (!swapChain)
        return CCO_FAIL_OUT_OF_MEMORY;

    swapChain->surface = desc->surface;

    const VkSwapchainCreateInfoKHR swapChainInfo = {.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                                    .pNext = NULL,
                                                    .flags = 0,
                                                    .surface = swapChain->surface,
                                                    .minImageCount = 2,
                                                    .imageFormat = VK_FORMAT_B8G8R8A8_UNORM,
                                                    .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
                                                    .imageExtent = {desc->extent.w, desc->extent.h},
                                                    .imageArrayLayers = 1,
                                                    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                                    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
                                                    .queueFamilyIndexCount = 0,
                                                    .pQueueFamilyIndices = NULL,
                                                    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                                                    .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                                    .presentMode = VK_PRESENT_MODE_FIFO_KHR,
                                                    .clipped = VK_FALSE,
                                                    .oldSwapchain = NULL};

    const VkResult result =
        vkCreateSwapchainKHR(ccoGetVulkanCoreDevice(desc->core), &swapChainInfo, NULL, &swapChain->swapChain);
    if (result != VK_SUCCESS) {
        CCO_LOG("Vulkan failed to create swapchain!");
        free(swapChain);
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }

    vkGetSwapchainImagesKHR(ccoGetVulkanCoreDevice(desc->core), swapChain->swapChain, &swapChain->imageCount, NULL);
    vkGetSwapchainImagesKHR(ccoGetVulkanCoreDevice(desc->core), swapChain->swapChain, &swapChain->imageCount,
                            swapChain->images);

    *outSwapChain = swapChain;
    return CCO_SUCCESS;
}
void ccoDestroyVulkanSwapChain(CcoVulkanCore core, CcoVulkanSwapChain swapChain) {
    if (swapChain->swapChain) {
        vkDestroySwapchainKHR(ccoGetVulkanCoreDevice(core), swapChain->swapChain, NULL);
        swapChain->swapChain = NULL;
    }

    if (swapChain->surface) {
        vkDestroySurfaceKHR(ccoGetVulkanCoreInstance(core), swapChain->surface, NULL);
        swapChain->surface = NULL;
    }
    free(swapChain);
}
