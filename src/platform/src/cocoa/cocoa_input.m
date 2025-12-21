//
// Created by fightinghawks18 on 12/20/25.
//

#include "platform/cocoa/cocoa_input.h"
#include "platform/input.h"

#import <Carbon/Carbon.h>

static const cco_input_key k_vk_to_cco_input_key_map[] = {
    [kVK_ANSI_W] = CCO_INPUT_KEY_W,
    [kVK_ANSI_A] = CCO_INPUT_KEY_A,
    [kVK_ANSI_S] = CCO_INPUT_KEY_S,
    [kVK_ANSI_D] = CCO_INPUT_KEY_D,

    [kVK_Space] = CCO_INPUT_KEY_SPACE,
    [kVK_Control] = CCO_INPUT_KEY_CTRL,
    [kVK_Escape] = CCO_INPUT_KEY_ESC,
};

static const cco_input_mouse_button mouse_number_to_cco_input_mouse_button_map[] = {
    [0] = CCO_INPUT_MOUSE_BUTTON_LEFT,
    [2] = CCO_INPUT_MOUSE_BUTTON_RIGHT,
    [3] = CCO_INPUT_MOUSE_BUTTON_3,
    [4] = CCO_INPUT_MOUSE_BUTTON_4,
    [5] = CCO_INPUT_MOUSE_BUTTON_5,
};

static struct {
    CcoWindow current_window;
    CcoBool enabled;

    CcoBool keys_processing[KEY_COUNT];
    CcoBool keys_now[KEY_COUNT];
    CcoBool keys_last[KEY_COUNT];

    CcoBool mouse_buttons_processing[MOUSE_BUTTON_COUNT];
    CcoBool mouse_buttons_now[MOUSE_BUTTON_COUNT];
    CcoBool mouse_buttons_last[MOUSE_BUTTON_COUNT];

    cco_mouse_point mouse_point_processing;
    cco_mouse_delta mouse_delta_processing;

    cco_mouse_point mouse_point;
    cco_mouse_delta mouse_delta;
} g_input_state;

cco_result cco_input_init(void) {
    memset(&g_input_state, 0, sizeof(g_input_state));
    return CCO_SUCCESS;
}

void cco_input_quit(void) {
    memset(&g_input_state, 0, sizeof(g_input_state));
}

void cco_input_poll(void) {
    // Cache old key/button state
    memcpy(g_input_state.keys_last, g_input_state.keys_now, sizeof(g_input_state.keys_now));
    memcpy(g_input_state.mouse_buttons_last, g_input_state.mouse_buttons_now, sizeof(g_input_state.mouse_buttons_now));

    // Cache new key/button state
    memcpy(g_input_state.keys_now, g_input_state.keys_processing, sizeof(g_input_state.keys_processing));
    memcpy(g_input_state.mouse_buttons_now, g_input_state.mouse_buttons_processing, sizeof(g_input_state.mouse_buttons_processing));

    g_input_state.mouse_delta = g_input_state.mouse_delta_processing;
    g_input_state.mouse_point = g_input_state.mouse_point_processing;

    g_input_state.mouse_delta_processing.x = 0;
    g_input_state.mouse_delta_processing.y = 0;
}

void cco_input_clear_frame_state(void) {
    memset(g_input_state.keys_now, 0, sizeof(g_input_state.keys_now));
    memset(g_input_state.keys_last, 0, sizeof(g_input_state.keys_last));
    memset(g_input_state.mouse_buttons_now, 0, sizeof(g_input_state.mouse_buttons_now));
    memset(g_input_state.mouse_buttons_last, 0, sizeof(g_input_state.mouse_buttons_last));

    g_input_state.mouse_delta.x = 0;
    g_input_state.mouse_delta.y = 0;
    g_input_state.mouse_delta_processing.x = 0;
    g_input_state.mouse_delta_processing.y = 0;
}

void cco_input_clear_hardware_state(void) {
    memset(g_input_state.keys_processing, 0, sizeof(g_input_state.keys_processing));
    memset(g_input_state.mouse_buttons_processing, 0, sizeof(g_input_state.mouse_buttons_processing));
}

void cco_input_give_window_focus(CcoWindow window) {
    cco_input_clear_frame_state();
    cco_input_clear_hardware_state();

    g_input_state.current_window = window;
}

void cco_input_enable(void) {
    g_input_state.enabled = CCO_YES;
}

void cco_input_disable(void) {
    g_input_state.enabled = CCO_NO;
}

CcoWindow cco_input_get_active_window(void) {
    return g_input_state.current_window;
}

cco_mouse_delta cco_input_get_mouse_delta(void) {
    return g_input_state.mouse_delta;
}

cco_mouse_point cco_input_get_mouse_point(void) {
    return g_input_state.mouse_point;
}

CcoBool cco_input_key_is_pressed(cco_input_key key) {
    return g_input_state.keys_now[key];
}

CcoBool cco_input_key_was_just_pressed(cco_input_key key) {
    return g_input_state.keys_now[key] == CCO_YES && g_input_state.keys_last[key] == CCO_NO;
}

CcoBool cco_input_key_was_just_released(cco_input_key key) {
    return g_input_state.keys_now[key] == CCO_NO && g_input_state.keys_last[key] == CCO_YES;
}

CcoBool cco_input_mouse_button_is_pressed(cco_input_mouse_button button) {
    return g_input_state.mouse_buttons_now[button];
}

CcoBool cco_input_mouse_button_was_just_pressed(cco_input_mouse_button button) {
    return g_input_state.mouse_buttons_now[button] == CCO_YES && g_input_state.mouse_buttons_last[button] == CCO_NO;
}

CcoBool cco_input_mouse_button_was_just_released(cco_input_mouse_button button) {
    return g_input_state.mouse_buttons_now[button] == CCO_NO && g_input_state.mouse_buttons_last[button] == CCO_YES;
}

cco_cocoa_input_event_result cco_input_handle_cocoa_event(NSEvent *event) {
    if (!g_input_state.enabled || !g_input_state.current_window)
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;

    NSWindow *cocoa_window = [event window];
    if (cco_window_get_handle(g_input_state.current_window) != cocoa_window)
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    
    switch (event.type) {
    case NSEventTypeKeyDown:
    case NSEventTypeKeyUp: {
        g_input_state.keys_processing[k_vk_to_cco_input_key_map[[event keyCode]]] = event.type == NSEventTypeKeyUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_HANDLED;
    }
    case NSEventTypeLeftMouseDown:
    case NSEventTypeLeftMouseUp: {
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_LEFT] = event.type == NSEventTypeLeftMouseUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    case NSEventTypeRightMouseDown:
    case NSEventTypeRightMouseUp: {
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_RIGHT] = event.type == NSEventTypeRightMouseUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    case NSEventTypeOtherMouseDown:
    case NSEventTypeOtherMouseUp: {
        g_input_state.mouse_buttons_processing[mouse_number_to_cco_input_mouse_button_map[[event buttonNumber]]] = event.type == NSEventTypeOtherMouseUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    case NSEventTypeMouseMoved: {
        g_input_state.mouse_delta_processing.x += [event deltaX];
        g_input_state.mouse_delta_processing.y += [event deltaY];

        CGPoint mouse_point = [event locationInWindow];
        g_input_state.mouse_point_processing.x = mouse_point.x;
        g_input_state.mouse_point_processing.y = mouse_point.y;
        return CCO_COCOA_INPUT_EVENT_HANDLED;
    }
    default: break;
    }
    return CCO_COCOA_INPUT_EVENT_UNHANDLED;
}