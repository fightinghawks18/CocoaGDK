#include <stdio.h>

#include "platform/windowing.h"
#include "vulkan/core.h"
#include "vulkan/swapchain.h"

int main() {
    if (ccoWindowingInit() != CCO_SUCCESS) {
        CCO_LOG("Failed to initialize windowing!");
        return -1;
    }

    CcoWindow window;
    CcoResult windowResult =
        ccoCreateWindow(&(CcoWindowDesc){CCO_WINDOW_POS_CENTER, CCO_WINDOW_POS_CENTER, 800, 600, "cocoa",
                                         CCO_WINDOW_FLAG_DECOR_BIT | CCO_WINDOW_FLAG_RESIZE_BIT},
                        &window);
    if (windowResult != CCO_SUCCESS) {
        CCO_LOG("Failed to create window!");
        return -1;
    }

    CcoVulkanCore vulkan;
    ccoCreateVulkanCore(&(CcoVulkanCoreDesc){.gpuPowerPreference = CCO_GPU_POWER_PREFERENCE_HI,
                                             .desiredQueues = &(CcoGPUQueueTypeFlags){CCO_GPU_QUEUE_GRAPHICS},
                                             .desiredQueueCount = 1},
                        &vulkan);

    VkSurfaceKHR surface = ccoCreateWindowVulkanSurface(ccoGetVulkanCoreInstance(vulkan), window);
    CcoVulkanSwapChain vulkanSwapChain;
    ccoCreateVulkanSwapChain(&(CcoVulkanSwapChainDesc){ .core = vulkan, .surface = surface, .extent = {800, 600} }, &vulkanSwapChain);
    
    while (!ccoShouldWindowClose(window)) {
        ccoWindowingPoll();
    }

    ccoDestroyVulkanSwapChain(vulkan, vulkanSwapChain);
    ccoDestroyVulkanCore(vulkan);

    ccoCloseWindow(window);
    ccoWindowingQuit();
    return 0;
}