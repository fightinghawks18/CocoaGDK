//
// Created by fightinghawks18 on 12/29/25.
//

#include "platform/platform.h"
#import <Cocoa/Cocoa.h>

#include "platform/audio.h"
#include "platform/input.h"
#include "platform/window.h"

static cco_platform_init_flags g_init_flags;

cco_result cco_platform_init(cco_platform_init_flags init_flags) {
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activate];
    
    if (init_flags & CCO_PLATFORM_INIT_AUDIO_BIT) {
        const cco_result audio_init_result = cco_audio_initialize();
        if (audio_init_result != CCO_SUCCESS) {
            CCO_LOG("Failed to initialize audio subsystem!");
            return audio_init_result;
        }
    }
    
    if (init_flags & CCO_PLATFORM_INIT_WINDOWING_BIT) {
        const cco_result window_init_result = cco_windowing_init();
        if (window_init_result != CCO_SUCCESS) {
            CCO_LOG("Failed to initialize window subsystem!");
            return window_init_result;
        }
    }
    
    if (init_flags & CCO_PLATFORM_INIT_INPUT_BIT) {
        const cco_result input_init_result = cco_input_init(CCO_INPUT_INIT_KEYBOARD_BIT | CCO_INPUT_INIT_MOUSE_BIT | CCO_INPUT_INIT_GAMEPAD_BIT);
        if (input_init_result != CCO_SUCCESS) {
            CCO_LOG("Failed to initialize input subsystem!");
            return input_init_result;
        }
    }
    
    g_init_flags = init_flags;
    return CCO_SUCCESS;
}

void cco_platform_quit() {
    if (g_init_flags & CCO_PLATFORM_INIT_INPUT_BIT)
        cco_input_quit();
    if (g_init_flags & CCO_PLATFORM_INIT_WINDOWING_BIT)
        cco_windowing_quit();
    if (g_init_flags & CCO_PLATFORM_INIT_AUDIO_BIT)
        cco_audio_quit();
}
