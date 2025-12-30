//
// Created by fightinghawks18 on 12/30/2025.
//

#include "platform/win32/win32_keyboard.h"

#include "core/log.h"

#include <hidusage.h>
#include <windows.h>

static const cco_input_key v_key_to_cco_input_key_map[256] = {
    // Function keys and top row
    [VK_ESCAPE] = CCO_INPUT_KEY_ESC,
    [VK_F1] = CCO_INPUT_KEY_F1,
    [VK_F2] = CCO_INPUT_KEY_F2,
    [VK_F3] = CCO_INPUT_KEY_F3,
    [VK_F4] = CCO_INPUT_KEY_F4,
    [VK_F5] = CCO_INPUT_KEY_F5,
    [VK_F6] = CCO_INPUT_KEY_F6,
    [VK_F7] = CCO_INPUT_KEY_F7,
    [VK_F8] = CCO_INPUT_KEY_F8,
    [VK_F9] = CCO_INPUT_KEY_F9,
    [VK_F10] = CCO_INPUT_KEY_F10,
    [VK_F11] = CCO_INPUT_KEY_F11,
    [VK_F12] = CCO_INPUT_KEY_F12,
    [VK_SCROLL] = CCO_INPUT_KEY_SCROLL_LOCK,

    // Media keys
    [VK_VOLUME_MUTE] = CCO_INPUT_KEY_MEDIA_MUTE,
    [VK_VOLUME_UP] = CCO_INPUT_KEY_MEDIA_VOLUME_UP,
    [VK_VOLUME_DOWN] = CCO_INPUT_KEY_MEDIA_VOLUME_DOWN,
    [VK_MEDIA_PLAY_PAUSE] = CCO_INPUT_KEY_MEDIA_PLAY_PAUSE,
    [VK_MEDIA_STOP] = CCO_INPUT_KEY_MEDIA_STOP,
    [VK_MEDIA_PREV_TRACK] = CCO_INPUT_KEY_MEDIA_PREVIOUS_TRACK,
    [VK_MEDIA_NEXT_TRACK] = CCO_INPUT_KEY_MEDIA_NEXT_TRACK,

    // Number row
    [VK_OEM_3] = CCO_INPUT_KEY_GRAVE,
    ['1'] = CCO_INPUT_KEY_1,
    ['2'] = CCO_INPUT_KEY_2,
    ['3'] = CCO_INPUT_KEY_3,
    ['4'] = CCO_INPUT_KEY_4,
    ['5'] = CCO_INPUT_KEY_5,
    ['6'] = CCO_INPUT_KEY_6,
    ['7'] = CCO_INPUT_KEY_7,
    ['8'] = CCO_INPUT_KEY_8,
    ['9'] = CCO_INPUT_KEY_9,
    ['0'] = CCO_INPUT_KEY_0,
    [VK_OEM_MINUS] = CCO_INPUT_KEY_MINUS,
    [VK_OEM_PLUS] = CCO_INPUT_KEY_EQUAL,
    [VK_BACK] = CCO_INPUT_KEY_BACKSPACE,
    [VK_INSERT] = CCO_INPUT_KEY_INSERT,
    [VK_HOME] = CCO_INPUT_KEY_HOME,
    [VK_PRIOR] = CCO_INPUT_KEY_PAGE_UP,

    // Tab row
    [VK_TAB] = CCO_INPUT_KEY_TAB,
    ['Q'] = CCO_INPUT_KEY_Q,
    ['W'] = CCO_INPUT_KEY_W,
    ['E'] = CCO_INPUT_KEY_E,
    ['R'] = CCO_INPUT_KEY_R,
    ['T'] = CCO_INPUT_KEY_T,
    ['Y'] = CCO_INPUT_KEY_Y,
    ['U'] = CCO_INPUT_KEY_U,
    ['I'] = CCO_INPUT_KEY_I,
    ['O'] = CCO_INPUT_KEY_O,
    ['P'] = CCO_INPUT_KEY_P,
    [VK_OEM_4] = CCO_INPUT_KEY_LEFT_BRACKET,
    [VK_OEM_6] = CCO_INPUT_KEY_RIGHT_BRACKET,
    [VK_OEM_5] = CCO_INPUT_KEY_LEFT_SLASH,
    [VK_DELETE] = CCO_INPUT_KEY_DELETE,
    [VK_END] = CCO_INPUT_KEY_END,
    [VK_NEXT] = CCO_INPUT_KEY_PAGE_DOWN,

    // Caps Lock row
    [VK_CAPITAL] = CCO_INPUT_KEY_CAPS_LOCK,
    ['A'] = CCO_INPUT_KEY_A,
    ['S'] = CCO_INPUT_KEY_S,
    ['D'] = CCO_INPUT_KEY_D,
    ['F'] = CCO_INPUT_KEY_F,
    ['G'] = CCO_INPUT_KEY_G,
    ['H'] = CCO_INPUT_KEY_H,
    ['J'] = CCO_INPUT_KEY_J,
    ['K'] = CCO_INPUT_KEY_K,
    ['L'] = CCO_INPUT_KEY_L,
    [VK_OEM_1] = CCO_INPUT_KEY_SEMICOLON,
    [VK_OEM_7] = CCO_INPUT_KEY_APOSTROPHE,
    [VK_RETURN] = CCO_INPUT_KEY_ENTER,

    // Shift row
    ['Z'] = CCO_INPUT_KEY_Z,
    ['X'] = CCO_INPUT_KEY_X,
    ['C'] = CCO_INPUT_KEY_C,
    ['V'] = CCO_INPUT_KEY_V,
    ['B'] = CCO_INPUT_KEY_B,
    ['N'] = CCO_INPUT_KEY_N,
    ['M'] = CCO_INPUT_KEY_M,
    [VK_OEM_COMMA] = CCO_INPUT_KEY_COMMA,
    [VK_OEM_PERIOD] = CCO_INPUT_KEY_PERIOD,
    [VK_OEM_2] = CCO_INPUT_KEY_RIGHT_SLASH,

    // Bottom row
    [VK_LCONTROL] = CCO_INPUT_KEY_LEFT_CTRL,
    [VK_LWIN] = CCO_INPUT_KEY_SUPER,
    [VK_SPACE] = CCO_INPUT_KEY_SPACE,
    [VK_APPS] = CCO_INPUT_KEY_MENU,
    [VK_RCONTROL] = CCO_INPUT_KEY_RIGHT_CTRL,

    // Arrow keys
    [VK_UP] = CCO_INPUT_KEY_UP_ARROW,
    [VK_LEFT] = CCO_INPUT_KEY_LEFT_ARROW,
    [VK_DOWN] = CCO_INPUT_KEY_DOWN_ARROW,
    [VK_RIGHT] = CCO_INPUT_KEY_RIGHT_ARROW,

    // Numpad
    [VK_NUMLOCK] = CCO_INPUT_KEY_NUMPAD_NUMLOCK,
    [VK_NUMPAD0] = CCO_INPUT_KEY_NUMPAD_0,
    [VK_NUMPAD1] = CCO_INPUT_KEY_NUMPAD_1,
    [VK_NUMPAD2] = CCO_INPUT_KEY_NUMPAD_2,
    [VK_NUMPAD3] = CCO_INPUT_KEY_NUMPAD_3,
    [VK_NUMPAD4] = CCO_INPUT_KEY_NUMPAD_4,
    [VK_NUMPAD5] = CCO_INPUT_KEY_NUMPAD_5,
    [VK_NUMPAD6] = CCO_INPUT_KEY_NUMPAD_6,
    [VK_NUMPAD7] = CCO_INPUT_KEY_NUMPAD_7,
    [VK_NUMPAD8] = CCO_INPUT_KEY_NUMPAD_8,
    [VK_NUMPAD9] = CCO_INPUT_KEY_NUMPAD_9,
    [VK_ADD] = CCO_INPUT_KEY_NUMPAD_PLUS,

    [VK_SUBTRACT] = CCO_INPUT_KEY_NUMPAD_MINUS,
    [VK_MULTIPLY] = CCO_INPUT_KEY_NUMPAD_ASTERISK,
    [VK_DIVIDE] = CCO_INPUT_KEY_NUMPAD_RIGHT_SLASH,
};

static win32_keyboard_state g_keyboard = {0};

cco_result win32_keyboard_init(void) {
    g_keyboard.kb.usUsagePage = HID_USAGE_PAGE_GENERIC;
    g_keyboard.kb.usUsage = HID_USAGE_GENERIC_KEYBOARD;
    g_keyboard.kb.dwFlags = 0;
    g_keyboard.kb.hwndTarget = NULL;

    if (!RegisterRawInputDevices(&g_keyboard.kb, 1, sizeof(RAWINPUTDEVICE))) {
        CCO_LOG("Failed to register keyboard input for win32!");
        return CCO_FAIL_INPUT_INIT_ERROR;
    }

    CCO_LOG("Initialized keyboard input!");
    return CCO_SUCCESS;
}

void win32_keyboard_quit(void) {
    g_keyboard.kb.usUsagePage = HID_USAGE_PAGE_GENERIC;
    g_keyboard.kb.usUsage = HID_USAGE_GENERIC_KEYBOARD;
    g_keyboard.kb.dwFlags = RIDEV_REMOVE;
    g_keyboard.kb.hwndTarget = NULL;

    if (!RegisterRawInputDevices(&g_keyboard.kb, 1, sizeof(RAWINPUTDEVICE))) {
        CCO_LOG("Failed to unregister keyboard input for win32!");
    }

    CCO_LOG("Closed keyboard input!");
}

cco_bool cco_input_key_is_pressed(const cco_input_key key) { return g_keyboard.now[key]; }

cco_bool cco_input_key_was_just_pressed(const cco_input_key key) {
    return g_keyboard.now[key] == CCO_YES && g_keyboard.last[key] == CCO_NO;
}

cco_bool cco_input_key_was_just_released(const cco_input_key key) {
    return g_keyboard.now[key] == CCO_NO && g_keyboard.last[key] == CCO_YES;
}

void win32_handle_raw_keyboard_input(const RAWKEYBOARD *kb) {
    const u16 vkey = kb->VKey;
    const u16 flags = kb->Flags;
    const u16 scancode = kb->MakeCode;

    const cco_bool pressed = (flags & RI_KEY_BREAK) == 0;
    cco_input_key key = v_key_to_cco_input_key_map[vkey];

    if (vkey == VK_CONTROL) // Windows maps right control and left control as the same
        key = flags & RI_KEY_E0 ? CCO_INPUT_KEY_RIGHT_CTRL : CCO_INPUT_KEY_LEFT_CTRL;
    if (vkey == VK_MENU) // Windows has VK_LALT and VK_RALT, but it doesn't work
        key = flags & RI_KEY_E0 ? CCO_INPUT_KEY_RIGHT_ALT : CCO_INPUT_KEY_LEFT_ALT;
    if (vkey == VK_SHIFT) // Windows has VK_LSHIFT and VK_RSHIFT, but it doesn't work
        key = scancode == 0x36 ? CCO_INPUT_KEY_RIGHT_SHIFT : CCO_INPUT_KEY_LEFT_SHIFT;
    if (vkey == VK_RETURN) // Windows maps numpad and regular enter as the same
        key = flags & RI_KEY_E0 ? CCO_INPUT_KEY_NUMPAD_ENTER : CCO_INPUT_KEY_ENTER;
    if (key == CCO_INPUT_KEY_UNKNOWN)
        return;

    g_keyboard.processing[key] = pressed;
}

void win32_poll_keyboard_input(void) {
    memcpy(g_keyboard.last, g_keyboard.now, sizeof(g_keyboard.now));
    memcpy(g_keyboard.now, g_keyboard.processing, sizeof(g_keyboard.processing));
}

void win32_keyboard_clear_hardware_state(void) {
    memset(g_keyboard.processing, 0, sizeof(g_keyboard.processing));
}

win32_keyboard_state *win32_get_keyboard_state(void) { return &g_keyboard; }