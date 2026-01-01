//
// Created by fightinghawks18 on 12/30/25.
//

#pragma once

#ifdef __APPLE__

#ifdef __OBJC__

#include "platform/input_keyboard.h"

#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

#include "platform/cocoa/cocoa_input.h"
typedef struct {
    cco_bool now[KEY_COUNT];
    cco_bool last[KEY_COUNT];
    cco_bool processing[KEY_COUNT];
} cocoa_keyboard;

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

void cocoa_keyboard_clear_hardware_state(void);
cco_cocoa_input_event_result cocoa_keyboard_handle_event(NSEvent *event);
void cocoa_keyboard_poll(void);

#endif

#endif