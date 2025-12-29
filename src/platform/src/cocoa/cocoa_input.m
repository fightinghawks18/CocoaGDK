//
// Created by fightinghawks18 on 12/20/25.
//

#include "platform/cocoa/cocoa_input.h"
#include "platform/input.h"

#import <Carbon/Carbon.h>

static const cco_input_key k_vk_to_cco_input_key_map[128] = {
    // Function keys
    [kVK_Escape] = CCO_INPUT_KEY_ESC,
    [kVK_F1] = CCO_INPUT_KEY_F1,
    [kVK_F2] = CCO_INPUT_KEY_F2,
    [kVK_F3] = CCO_INPUT_KEY_F3,
    [kVK_F4] = CCO_INPUT_KEY_F4,
    [kVK_F5] = CCO_INPUT_KEY_F5,
    [kVK_F6] = CCO_INPUT_KEY_F6,
    [kVK_F7] = CCO_INPUT_KEY_F7,
    [kVK_F8] = CCO_INPUT_KEY_F8,
    [kVK_F9] = CCO_INPUT_KEY_F9,
    [kVK_F10] = CCO_INPUT_KEY_F10,
    [kVK_F11] = CCO_INPUT_KEY_F11,
    [kVK_F12] = CCO_INPUT_KEY_F12,

    // Volume/Media keys
    [kVK_Mute] = CCO_INPUT_KEY_MEDIA_MUTE,
    [kVK_VolumeUp] = CCO_INPUT_KEY_MEDIA_VOLUME_UP,
    [kVK_VolumeDown] = CCO_INPUT_KEY_MEDIA_VOLUME_DOWN,

    // Number row
    [kVK_ANSI_Grave] = CCO_INPUT_KEY_GRAVE,
    [kVK_ANSI_1] = CCO_INPUT_KEY_1,
    [kVK_ANSI_2] = CCO_INPUT_KEY_2,
    [kVK_ANSI_3] = CCO_INPUT_KEY_3,
    [kVK_ANSI_4] = CCO_INPUT_KEY_4,
    [kVK_ANSI_5] = CCO_INPUT_KEY_5,
    [kVK_ANSI_6] = CCO_INPUT_KEY_6,
    [kVK_ANSI_7] = CCO_INPUT_KEY_7,
    [kVK_ANSI_8] = CCO_INPUT_KEY_8,
    [kVK_ANSI_9] = CCO_INPUT_KEY_9,
    [kVK_ANSI_0] = CCO_INPUT_KEY_0,
    [kVK_ANSI_Minus] = CCO_INPUT_KEY_MINUS,
    [kVK_ANSI_Equal] = CCO_INPUT_KEY_EQUAL,
    [kVK_Delete] = CCO_INPUT_KEY_BACKSPACE,

    // Tab row
    [kVK_Tab] = CCO_INPUT_KEY_TAB,
    [kVK_ANSI_Q] = CCO_INPUT_KEY_Q,
    [kVK_ANSI_W] = CCO_INPUT_KEY_W,
    [kVK_ANSI_E] = CCO_INPUT_KEY_E,
    [kVK_ANSI_R] = CCO_INPUT_KEY_R,
    [kVK_ANSI_T] = CCO_INPUT_KEY_T,
    [kVK_ANSI_Y] = CCO_INPUT_KEY_Y,
    [kVK_ANSI_U] = CCO_INPUT_KEY_U,
    [kVK_ANSI_I] = CCO_INPUT_KEY_I,
    [kVK_ANSI_O] = CCO_INPUT_KEY_O,
    [kVK_ANSI_P] = CCO_INPUT_KEY_P,
    [kVK_ANSI_LeftBracket] = CCO_INPUT_KEY_LEFT_BRACKET,
    [kVK_ANSI_RightBracket] = CCO_INPUT_KEY_RIGHT_BRACKET,
    [kVK_ANSI_Backslash] = CCO_INPUT_KEY_LEFT_SLASH,

    // Caps Lock row
    [kVK_CapsLock] = CCO_INPUT_KEY_CAPS_LOCK,
    [kVK_ANSI_A] = CCO_INPUT_KEY_A,
    [kVK_ANSI_S] = CCO_INPUT_KEY_S,
    [kVK_ANSI_D] = CCO_INPUT_KEY_D,
    [kVK_ANSI_F] = CCO_INPUT_KEY_F,
    [kVK_ANSI_G] = CCO_INPUT_KEY_G,
    [kVK_ANSI_H] = CCO_INPUT_KEY_H,
    [kVK_ANSI_J] = CCO_INPUT_KEY_J,
    [kVK_ANSI_K] = CCO_INPUT_KEY_K,
    [kVK_ANSI_L] = CCO_INPUT_KEY_L,
    [kVK_ANSI_Semicolon] = CCO_INPUT_KEY_SEMICOLON,
    [kVK_ANSI_Quote] = CCO_INPUT_KEY_APOSTROPHE,
    [kVK_Return] = CCO_INPUT_KEY_ENTER,

    // Shift row
    [kVK_Shift] = CCO_INPUT_KEY_LEFT_SHIFT,
    [kVK_ANSI_Z] = CCO_INPUT_KEY_Z,
    [kVK_ANSI_X] = CCO_INPUT_KEY_X,
    [kVK_ANSI_C] = CCO_INPUT_KEY_C,
    [kVK_ANSI_V] = CCO_INPUT_KEY_V,
    [kVK_ANSI_B] = CCO_INPUT_KEY_B,
    [kVK_ANSI_N] = CCO_INPUT_KEY_N,
    [kVK_ANSI_M] = CCO_INPUT_KEY_M,
    [kVK_ANSI_Comma] = CCO_INPUT_KEY_COMMA,
    [kVK_ANSI_Period] = CCO_INPUT_KEY_PERIOD,
    [kVK_ANSI_Slash] = CCO_INPUT_KEY_RIGHT_SLASH,
    [kVK_RightShift] = CCO_INPUT_KEY_RIGHT_SHIFT,

    // Bottom row
    [kVK_Control] = CCO_INPUT_KEY_LEFT_CTRL,
    [kVK_Command] = CCO_INPUT_KEY_SUPER,
    [kVK_Option] = CCO_INPUT_KEY_LEFT_ALT,
    [kVK_Space] = CCO_INPUT_KEY_SPACE,
    [kVK_RightOption] = CCO_INPUT_KEY_RIGHT_ALT,
    [kVK_RightControl] = CCO_INPUT_KEY_RIGHT_CTRL,

    // Navigation cluster
    [kVK_Help] = CCO_INPUT_KEY_INSERT,
    [kVK_Home] = CCO_INPUT_KEY_HOME,
    [kVK_PageUp] = CCO_INPUT_KEY_PAGE_UP,
    [kVK_ForwardDelete] = CCO_INPUT_KEY_DELETE,
    [kVK_End] = CCO_INPUT_KEY_END,
    [kVK_PageDown] = CCO_INPUT_KEY_PAGE_DOWN,

    // Arrow keys
    [kVK_UpArrow] = CCO_INPUT_KEY_UP_ARROW,
    [kVK_LeftArrow] = CCO_INPUT_KEY_LEFT_ARROW,
    [kVK_DownArrow] = CCO_INPUT_KEY_DOWN_ARROW,
    [kVK_RightArrow] = CCO_INPUT_KEY_RIGHT_ARROW,

    // Numpad
    [kVK_ANSI_KeypadClear] = CCO_INPUT_KEY_NUMPAD_NUMLOCK,
    [kVK_ANSI_Keypad0] = CCO_INPUT_KEY_NUMPAD_0,
    [kVK_ANSI_Keypad1] = CCO_INPUT_KEY_NUMPAD_1,
    [kVK_ANSI_Keypad2] = CCO_INPUT_KEY_NUMPAD_2,
    [kVK_ANSI_Keypad3] = CCO_INPUT_KEY_NUMPAD_3,
    [kVK_ANSI_Keypad4] = CCO_INPUT_KEY_NUMPAD_4,
    [kVK_ANSI_Keypad5] = CCO_INPUT_KEY_NUMPAD_5,
    [kVK_ANSI_Keypad6] = CCO_INPUT_KEY_NUMPAD_6,
    [kVK_ANSI_Keypad7] = CCO_INPUT_KEY_NUMPAD_7,
    [kVK_ANSI_Keypad8] = CCO_INPUT_KEY_NUMPAD_8,
    [kVK_ANSI_Keypad9] = CCO_INPUT_KEY_NUMPAD_9,
    [kVK_ANSI_KeypadPlus] = CCO_INPUT_KEY_NUMPAD_PLUS,
    [kVK_ANSI_KeypadEnter] = CCO_INPUT_KEY_NUMPAD_ENTER,
    [kVK_ANSI_KeypadMinus] = CCO_INPUT_KEY_NUMPAD_MINUS,
    [kVK_ANSI_KeypadMultiply] = CCO_INPUT_KEY_NUMPAD_ASTERISK,
    [kVK_ANSI_KeypadDivide] = CCO_INPUT_KEY_NUMPAD_RIGHT_SLASH,
    [kVK_ANSI_KeypadDecimal] = CCO_INPUT_KEY_NUMPAD_0,
};

static const cco_input_mouse_button mouse_number_to_cco_input_mouse_button_map[] = {
    [0] = CCO_INPUT_MOUSE_BUTTON_LEFT,
    [2] = CCO_INPUT_MOUSE_BUTTON_RIGHT,
    [3] = CCO_INPUT_MOUSE_BUTTON_3,
    [4] = CCO_INPUT_MOUSE_BUTTON_4,
    [5] = CCO_INPUT_MOUSE_BUTTON_5,
};

static struct {
    cco_window current_window;
    cco_bool enabled;

    cco_bool keys_processing[KEY_COUNT];
    cco_bool keys_now[KEY_COUNT];
    cco_bool keys_last[KEY_COUNT];

    cco_bool mouse_buttons_processing[MOUSE_BUTTON_COUNT];
    cco_bool mouse_buttons_now[MOUSE_BUTTON_COUNT];
    cco_bool mouse_buttons_last[MOUSE_BUTTON_COUNT];

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
    
    for (u32 i = 0; i < KEY_COUNT; ++i) {
        if (cco_input_key_was_just_pressed(i))
            CCO_LOG("%s", cco_input_key_as_text(i));
    }
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

void cco_input_give_window_focus(cco_window window) {
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

cco_window cco_input_get_active_window(void) {
    return g_input_state.current_window;
}

cco_mouse_delta cco_input_get_mouse_delta(void) {
    return g_input_state.mouse_delta;
}

cco_mouse_point cco_input_get_mouse_point(void) {
    return g_input_state.mouse_point;
}

cco_bool cco_input_key_is_pressed(cco_input_key key) {
    return g_input_state.keys_now[key];
}

cco_bool cco_input_key_was_just_pressed(cco_input_key key) {
    return g_input_state.keys_now[key] == CCO_YES && g_input_state.keys_last[key] == CCO_NO;
}

cco_bool cco_input_key_was_just_released(cco_input_key key) {
    return g_input_state.keys_now[key] == CCO_NO && g_input_state.keys_last[key] == CCO_YES;
}

cco_bool cco_input_mouse_button_is_pressed(cco_input_mouse_button button) {
    return g_input_state.mouse_buttons_now[button];
}

cco_bool cco_input_mouse_button_was_just_pressed(cco_input_mouse_button button) {
    return g_input_state.mouse_buttons_now[button] == CCO_YES && g_input_state.mouse_buttons_last[button] == CCO_NO;
}

cco_bool cco_input_mouse_button_was_just_released(cco_input_mouse_button button) {
    return g_input_state.mouse_buttons_now[button] == CCO_NO && g_input_state.mouse_buttons_last[button] == CCO_YES;
}

cco_cocoa_input_event_result cco_input_handle_cocoa_event(NSEvent *event) {
    if (!g_input_state.enabled || !g_input_state.current_window)
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;

    NSWindow *cocoa_window = [event window];
    if (cco_window_get_handle(g_input_state.current_window) != cocoa_window)
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    
    switch (event.type) {
    case NSEventTypeFlagsChanged: {
        u16 key_code = [event keyCode];
        NSEventModifierFlags modifier_flags = [event modifierFlags];
        
        const cco_input_key key = k_vk_to_cco_input_key_map[key_code];
        if (key == CCO_INPUT_KEY_UNKNOWN)
            return CCO_COCOA_INPUT_EVENT_UNHANDLED;
        
        NSEventModifierFlags relevant_flag = 0;
        switch (key_code) {
        case kVK_Shift:
        case kVK_RightShift: {
            relevant_flag = NSEventModifierFlagShift;
            break;
        }
        case kVK_Control:
        case kVK_RightControl: {
            relevant_flag = NSEventModifierFlagControl;
            break;
        }
        case kVK_Command:
        case kVK_RightCommand: {
            relevant_flag = NSEventModifierFlagCommand;
            break;
        }
        case kVK_Option:
        case kVK_RightOption: {
            relevant_flag = NSEventModifierFlagOption;
            break;
        }
        case kVK_CapsLock: {
            relevant_flag = NSEventModifierFlagCapsLock;
            break;
        }
        }
        
        bool is_pressed = (modifier_flags & relevant_flag) != 0;
        g_input_state.keys_processing[key] = is_pressed ? CCO_YES : CCO_NO;
        return CCO_COCOA_INPUT_EVENT_HANDLED;
    }
    case NSEventTypeKeyDown:
    case NSEventTypeKeyUp: {
        const cco_input_key key = k_vk_to_cco_input_key_map[[event keyCode]];
        if (key == CCO_INPUT_KEY_UNKNOWN)
            return CCO_COCOA_INPUT_EVENT_UNHANDLED;
        
        g_input_state.keys_processing[key] = event.type == NSEventTypeKeyUp ? CCO_NO : CCO_YES;
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
    case NSEventTypeScrollWheel: {
        bool using_trackpad = [event hasPreciseScrollingDeltas] == YES;
        CGFloat scroll_delta = [event scrollingDeltaY];
        g_input_state.mouse_delta_processing.wheel = using_trackpad ? scroll_delta / 10.0f : scroll_delta;
        return;
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
