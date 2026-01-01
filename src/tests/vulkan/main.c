#include <stdio.h>

#include "cocoa_gdk/vulkan/instance.h"
#include "cocoa_gdk/vulkan/swapchain.h"
#include "cocoa_gdk/platform/input.h"
#include "cocoa_gdk/platform/utils.h"
#include "cocoa_gdk/platform/window.h"

int main() {
    if (cco_windowing_init() != CCO_SUCCESS) {
        CCO_LOG("Failed to initialize windowing!");
        return -1;
    }

    if (cco_input_init() != CCO_SUCCESS) {
        CCO_LOG("Failed to initialize input!");
        return -1;
    }
    cco_input_enable();

    cco_window window = CCO_NIL;
    cco_result window_result = cco_create_window(0, 0, 800, 600, "cocoa", &window);
    if (window_result != CCO_SUCCESS) {
        CCO_LOG("Failed to create window!");
        return -1;
    }

    cco_input_give_window_focus(window);

    const cco_vulkan_instance_desc desc = {
        .power_preference = CCO_VULKAN_POWER_HIGH,
        .app_name = "Cocoa",
        .engine_name = "CocoaGameKit",
        .app_version = {1, 0, 0},
        .engine_version = {1, 0, 0}
    };

    cco_vulkan_instance instance = CCO_NIL;
    cco_result instance_result = cco_create_vulkan_instance(&desc, &instance);
    if (instance_result != CCO_SUCCESS) {
        CCO_LOG("Failed to create instance!");
        return -1;
    }

    cco_window_content_size content = cco_window_get_content_size(window);

    const cco_vulkan_swapchain_desc swapchain_desc = {
        .window_handle = cco_window_get_handle(window),
        .window_framebuffer_width = content.width,
        .window_framebuffer_height = content.height
    };

    cco_vulkan_swapchain swapchain = CCO_NIL;
    cco_result swapchain_result = cco_create_vulkan_swapchain(instance, &swapchain_desc, &swapchain);
    if (swapchain_result != CCO_SUCCESS) {
        CCO_LOG("Failed to create a swapchain!");
        return -1;
    }

    while (!cco_window_will_close(window)) {
        cco_window_pump_events(window);
        cco_input_poll();

        if (cco_input_key_was_just_pressed(CCO_INPUT_KEY_W)) {
            CCO_LOG("INPUT");
        }

        cco_sleep(5);
    }

    cco_destroy_vulkan_swapchain(swapchain);
    cco_destroy_vulkan_instance(instance);

    cco_destroy_window(window);
    cco_windowing_quit();
    return 0;
}