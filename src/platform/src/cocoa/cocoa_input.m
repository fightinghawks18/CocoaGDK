//
// Created by fightinghawks18 on 12/20/25.
//

#include "platform/cocoa/cocoa_input.h"
#include "platform/input.h"
#include "platform/cocoa/cocoa_keyboard.h"
#include "platform/cocoa/cocoa_mouse.h"
#include "platform/cocoa/cocoa_gamepad.h"

typedef struct {
    cco_window current_window;
    cco_bool enabled;

    cco_input_init_flags init_flags;
} input_state;

static input_state g_input_state = {0};

cco_result cco_input_init(cco_input_init_flags init_flags) {
    if (init_flags & CCO_INPUT_INIT_GAMEPAD_BIT) {
        const cco_result gamepad_init_result = cocoa_gamepad_init();
        if (gamepad_init_result != CCO_SUCCESS) {
            CCO_LOG("Failed to initialize gamepad system!");
            return gamepad_init_result;
        }
    }

    g_input_state.init_flags = init_flags;
    return CCO_SUCCESS;
}

void cco_input_quit(void) {
    if (g_input_state.init_flags & CCO_INPUT_INIT_GAMEPAD_BIT)
        cocoa_gamepad_quit();

    memset(&g_input_state, 0, sizeof(g_input_state));
}

void cco_input_poll(void) {
    if (!g_input_state.enabled)
        return;

    if (g_input_state.init_flags & CCO_INPUT_INIT_KEYBOARD_BIT)
        cocoa_keyboard_poll();
    if (g_input_state.init_flags & CCO_INPUT_INIT_MOUSE_BIT)
        cocoa_mouse_poll();
    if (g_input_state.init_flags & CCO_INPUT_INIT_GAMEPAD_BIT)
        cocoa_gamepad_poll();
}

void cco_input_clear_hardware_state(void) {
    if (g_input_state.init_flags & CCO_INPUT_INIT_KEYBOARD_BIT)
        cocoa_keyboard_clear_hardware_state();
    if (g_input_state.init_flags * CCO_INPUT_INIT_MOUSE_BIT)
        cocoa_mouse_clear_hardware_state();
}

void cco_input_give_window_focus(cco_window window) {
    cco_input_clear_hardware_state();

    g_input_state.current_window = window;
}

void cco_input_enable(void) {
    g_input_state.enabled = CCO_YES;
}

void cco_input_disable(void) {
    g_input_state.enabled = CCO_NO;
}

cco_window cco_input_get_active_window(void) {
    return g_input_state.current_window;
}

cco_cocoa_input_event_result cco_input_handle_cocoa_event(NSEvent *event) {
    if (!g_input_state.enabled || !g_input_state.current_window)
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;

    NSWindow *cocoa_window = [event window];
    if (cco_window_get_handle(g_input_state.current_window) != cocoa_window)
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;

    i32 handled = 0;
    if (g_input_state.init_flags & CCO_INPUT_INIT_KEYBOARD_BIT)
        handled += cocoa_keyboard_handle_event(event) == CCO_COCOA_INPUT_EVENT_HANDLED ? 1 : 0;
    if (g_input_state.init_flags & CCO_INPUT_INIT_MOUSE_BIT)
        handled += cocoa_mouse_handle_event(event) == CCO_COCOA_INPUT_EVENT_HANDLED ? 1 : 0;
    return handled > 0 ? CCO_COCOA_INPUT_EVENT_HANDLED : CCO_COCOA_INPUT_EVENT_UNHANDLED;
}
