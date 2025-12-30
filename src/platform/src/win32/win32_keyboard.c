//
// Created by fightinghawks18 on 12/30/2025.
//

#include "platform/win32/win32_keyboard.h"

#include "core/log.h"

#include <hidusage.h>
#include <windows.h>

static win32_keyboard_state g_keyboard = {0};

cco_result win32_keyboard_init(void) {
    g_keyboard.kb.usUsagePage = HID_USAGE_PAGE_GENERIC;
    g_keyboard.kb.usUsage = HID_USAGE_GENERIC_KEYBOARD;
    g_keyboard.kb.dwFlags = 0;
    g_keyboard.kb.hwndTarget = NULL;

    if (!RegisterRawInputDevices(&g_keyboard.kb, 1, sizeof(RAWINPUTDEVICE))) {
        CCO_LOG("Failed to register keyboard input for win32!");
        return CCO_FAIL_INPUT_KEYBOARD_INIT_ERROR;
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
        return;
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

void win32_keyboard_poll(void) {
    memcpy(g_keyboard.last, g_keyboard.now, sizeof(g_keyboard.now));
    memcpy(g_keyboard.now, g_keyboard.processing, sizeof(g_keyboard.processing));
}

void win32_keyboard_clear_hardware_state(void) {
    memset(g_keyboard.processing, 0, sizeof(g_keyboard.processing));
}

win32_keyboard_state *win32_get_keyboard_state(void) { return &g_keyboard; }