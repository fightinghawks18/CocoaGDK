//
// Created by fightinghawks18 on 12/20/2025.
//

#include "platform/win32/win32_input.h"
#include "platform/input.h"
#include <hidusage.h>

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

static struct {
    RAWINPUTDEVICE rids[2];

    cco_window current_window;
    cco_bool enabled;

    cco_bool keys_processing[KEY_COUNT];
    cco_bool keys_now[KEY_COUNT];
    cco_bool keys_last[KEY_COUNT];

    cco_bool mouse_buttons_processing[MOUSE_BUTTON_COUNT];
    cco_bool mouse_buttons_now[MOUSE_BUTTON_COUNT];
    cco_bool mouse_buttons_last[MOUSE_BUTTON_COUNT];

    cco_mouse_delta mouse_delta_processing;

    cco_mouse_point mouse_point;
    cco_mouse_delta mouse_delta;
} g_input_state;

cco_result cco_input_init(void) {
    memset(&g_input_state, 0, sizeof(g_input_state));

    RAWINPUTDEVICE *keyboard = &g_input_state.rids[0];
    keyboard->usUsagePage = HID_USAGE_PAGE_GENERIC;
    keyboard->usUsage = HID_USAGE_GENERIC_KEYBOARD;
    keyboard->dwFlags = 0;
    keyboard->hwndTarget = NULL;

    RAWINPUTDEVICE *mouse = &g_input_state.rids[1];
    mouse->usUsagePage = HID_USAGE_PAGE_GENERIC;
    mouse->usUsage = HID_USAGE_GENERIC_MOUSE;
    mouse->dwFlags = 0;
    mouse->hwndTarget = NULL;

    if (!RegisterRawInputDevices(g_input_state.rids, 2, sizeof(RAWINPUTDEVICE))) {
        CCO_LOG("Failed to register raw input devices for win32!");
        return CCO_FAIL_INPUT_INIT_ERROR;
    }

    return CCO_SUCCESS;
}

void cco_input_quit(void) {
    RAWINPUTDEVICE *keyboard = &g_input_state.rids[0];
    keyboard->usUsagePage = HID_USAGE_PAGE_GENERIC;
    keyboard->usUsage = HID_USAGE_GENERIC_KEYBOARD;
    keyboard->dwFlags = RIDEV_REMOVE;
    keyboard->hwndTarget = NULL;

    RAWINPUTDEVICE *mouse = &g_input_state.rids[1];
    mouse->usUsagePage = HID_USAGE_PAGE_GENERIC;
    mouse->usUsage = HID_USAGE_GENERIC_MOUSE;
    mouse->dwFlags = RIDEV_REMOVE;
    mouse->hwndTarget = NULL;

    if (!RegisterRawInputDevices(g_input_state.rids, 2, sizeof(RAWINPUTDEVICE))) {
        CCO_LOG("Failed to unregister raw input devices for win32!");
    }

    memset(&g_input_state, 0, sizeof(g_input_state));
}

void cco_input_poll(void) {
    // Cache old key/button state
    memcpy(g_input_state.keys_last, g_input_state.keys_now, sizeof(g_input_state.keys_now));
    memcpy(g_input_state.mouse_buttons_last, g_input_state.mouse_buttons_now, sizeof(g_input_state.mouse_buttons_now));

    // Cache new key/button state
    memcpy(g_input_state.keys_now, g_input_state.keys_processing, sizeof(g_input_state.keys_processing));
    memcpy(g_input_state.mouse_buttons_now, g_input_state.mouse_buttons_processing, sizeof(g_input_state.mouse_buttons_processing));

    POINT cursor;
    GetCursorPos(&cursor);
    g_input_state.mouse_point.x = cursor.x;
    g_input_state.mouse_point.y = cursor.y;

    g_input_state.mouse_delta = g_input_state.mouse_delta_processing;

    g_input_state.mouse_delta_processing.x = 0;
    g_input_state.mouse_delta_processing.y = 0;
    g_input_state.mouse_delta_processing.wheel = 0;
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
    g_input_state.mouse_delta_processing.wheel = 0;
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

void cco_input_enable(void) { g_input_state.enabled = CCO_YES; }

void cco_input_disable(void) { g_input_state.enabled = CCO_NO; }

cco_window cco_input_get_active_window(void) { return g_input_state.current_window; }

cco_mouse_delta cco_input_get_mouse_delta(void) { return g_input_state.mouse_delta; }

cco_mouse_point cco_input_get_mouse_point(void) { return g_input_state.mouse_point; }

cco_bool cco_input_key_is_pressed(cco_input_key key) { return g_input_state.keys_now[key]; }

cco_bool cco_input_key_was_just_pressed(cco_input_key key) {
    return g_input_state.keys_now[key] == CCO_YES && g_input_state.keys_last[key] == CCO_NO;
}

cco_bool cco_input_key_was_just_released(cco_input_key key) {
    return g_input_state.keys_now[key] == CCO_NO && g_input_state.keys_last[key] == CCO_YES;
}

cco_bool cco_input_mouse_button_is_pressed(cco_input_mouse_button button) { return g_input_state.mouse_buttons_now[button]; }

cco_bool cco_input_mouse_button_was_just_pressed(cco_input_mouse_button button) {
    return g_input_state.mouse_buttons_now[button] == CCO_YES && g_input_state.mouse_buttons_last[button] == CCO_NO;
}

cco_bool cco_input_mouse_button_was_just_released(cco_input_mouse_button button) {
    return g_input_state.mouse_buttons_now[button] == CCO_NO && g_input_state.mouse_buttons_last[button] == CCO_YES;
}

static void handle_raw_mouse_input(RAWMOUSE *mouse) {
    if (mouse->usFlags == MOUSE_MOVE_RELATIVE) {
        const i32 delta_x = mouse->lLastX;
        const i32 delta_y = mouse->lLastY;

        g_input_state.mouse_delta_processing.x += (f32)delta_x;
        g_input_state.mouse_delta_processing.y += (f32)delta_y;
    }

    const u16 button_flags = mouse->usButtonFlags;
    if (button_flags & RI_MOUSE_LEFT_BUTTON_DOWN)
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_LEFT] = CCO_YES;
    if (button_flags & RI_MOUSE_LEFT_BUTTON_UP)
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_LEFT] = CCO_NO;
    if (button_flags & RI_MOUSE_RIGHT_BUTTON_DOWN)
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_RIGHT] = CCO_YES;
    if (button_flags & RI_MOUSE_RIGHT_BUTTON_UP)
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_RIGHT] = CCO_NO;
    if (button_flags & RI_MOUSE_BUTTON_3_DOWN)
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_3] = CCO_YES;
    if (button_flags & RI_MOUSE_BUTTON_3_UP)
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_3] = CCO_NO;
    if (button_flags & RI_MOUSE_BUTTON_4_DOWN)
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_4] = CCO_YES;
    if (button_flags & RI_MOUSE_BUTTON_4_UP)
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_4] = CCO_NO;
    if (button_flags & RI_MOUSE_BUTTON_5_DOWN)
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_5] = CCO_YES;
    if (button_flags & RI_MOUSE_BUTTON_5_UP)
        g_input_state.mouse_buttons_processing[CCO_INPUT_MOUSE_BUTTON_5] = CCO_NO;
    if (button_flags & RI_MOUSE_WHEEL) {
        const i16 wheel_delta = (i16)mouse->usButtonData;

        // The delta of the mouse wheel is in multiples of 120.0f (WHEEL_DELTA)
        g_input_state.mouse_delta_processing.wheel += (f32)wheel_delta / WHEEL_DELTA;
    }
}

static void handle_raw_keyboard_input(RAWKEYBOARD *keyboard) {
    const u16 vkey = keyboard->VKey;
    const u16 flags = keyboard->Flags;
    const u16 scancode = keyboard->MakeCode;

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
        return; // Past iteration didn't handle input here well, this helps avoid a crash

    g_input_state.keys_processing[key] = pressed;
}

void cco_input_handle_raw_input(HWND hWnd, LPARAM lparam) {
    HRAWINPUT raw_input_header = (HRAWINPUT)lparam;

    u32 raw_data_size = 0;
    GetRawInputData(raw_input_header, RID_INPUT, NULL, &raw_data_size, sizeof(RAWINPUTHEADER));

    LPBYTE raw_input_buffer = malloc(raw_data_size);
    if (!raw_input_buffer)
        return;

    if (GetRawInputData(raw_input_header, RID_INPUT, raw_input_buffer, &raw_data_size, sizeof(RAWINPUTHEADER)) !=
        raw_data_size) {
        free(raw_input_buffer);
        return;
    }

    RAWINPUT *raw_input = (RAWINPUT *)raw_input_buffer;

    if (raw_input->header.dwType == RIM_TYPEMOUSE) {
        handle_raw_mouse_input(&raw_input->data.mouse);
    } else if (raw_input->header.dwType == RIM_TYPEKEYBOARD) {
        handle_raw_keyboard_input(&raw_input->data.keyboard);
    }
}