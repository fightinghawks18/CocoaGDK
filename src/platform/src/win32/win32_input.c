//
// Created by fightinghawks18 on 12/20/2025.
//

#include "platform/win32/win32_input.h"
#include "platform/input.h"
#include <hidusage.h>

static const cco_input_key v_key_to_cco_input_key_map[] = {
    ['W'] = CCO_INPUT_KEY_W,
    ['A'] = CCO_INPUT_KEY_A,
    ['S'] = CCO_INPUT_KEY_S,
    ['D'] = CCO_INPUT_KEY_D,

    [VK_SPACE] = CCO_INPUT_KEY_SPACE,
    [VK_CONTROL] = CCO_INPUT_KEY_CTRL,
    [VK_ESCAPE] = CCO_INPUT_KEY_ESC,
};

static struct {
    RAWINPUTDEVICE rids[2];

    CcoWindow current_window;
    CcoBool enabled;

    CcoBool keys_processing[KEY_COUNT];
    CcoBool keys_now[KEY_COUNT];
    CcoBool keys_last[KEY_COUNT];

    CcoBool mouse_buttons_processing[MOUSE_BUTTON_COUNT];
    CcoBool mouse_buttons_now[MOUSE_BUTTON_COUNT];
    CcoBool mouse_buttons_last[MOUSE_BUTTON_COUNT];

    cco_mouse_delta mouse_delta_processing;

    cco_mouse_point mouse_point;
    cco_mouse_delta mouse_delta;
} g_input_state;

cco_result cco_input_init(void) {
    memset(&g_input_state, 0, sizeof(g_input_state));

    RAWINPUTDEVICE *keyboard = &g_input_state.rids[0];
    keyboard->us_usage_page = HID_USAGE_PAGE_GENERIC;
    keyboard->us_usage = HID_USAGE_GENERIC_KEYBOARD;
    keyboard->dw_flags = 0;
    keyboard->hwnd_target = NULL;

    RAWINPUTDEVICE *mouse = &g_input_state.rids[1];
    mouse->us_usage_page = HID_USAGE_PAGE_GENERIC;
    mouse->us_usage = HID_USAGE_GENERIC_MOUSE;
    mouse->dw_flags = 0;
    mouse->hwnd_target = NULL;

    if (!RegisterRawInputDevices(g_input_state.rids, 2, sizeof(RAWINPUTDEVICE))) {
        CCO_LOG("Failed to register raw input devices for win32!");
        return CCO_FAIL_INPUT_INIT_ERROR;
    }

    return CCO_SUCCESS;
}

void cco_input_quit(void) {
    RAWINPUTDEVICE *keyboard = &g_input_state.rids[0];
    keyboard->us_usage_page = HID_USAGE_PAGE_GENERIC;
    keyboard->us_usage = HID_USAGE_GENERIC_KEYBOARD;
    keyboard->dw_flags = RIDEV_REMOVE;
    keyboard->hwnd_target = NULL;

    RAWINPUTDEVICE *mouse = &g_input_state.rids[1];
    mouse->us_usage_page = HID_USAGE_PAGE_GENERIC;
    mouse->us_usage = HID_USAGE_GENERIC_MOUSE;
    mouse->dw_flags = RIDEV_REMOVE;
    mouse->hwnd_target = NULL;

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

    g_input_state.mouse_delta = g_input_state.mouse_delta_processing;
    g_input_state.mouse_point.x = cursor.x;
    g_input_state.mouse_point.y = cursor.y;

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

void cco_input_give_window_focus(CcoWindow window) {
    cco_input_clear_frame_state();
    cco_input_clear_hardware_state();

    g_input_state.current_window = window;
}

void cco_input_enable(void) { g_input_state.enabled = CCO_YES; }

void cco_input_disable(void) { g_input_state.enabled = CCO_NO; }

CcoWindow cco_input_get_active_window(void) { return g_input_state.current_window; }

cco_mouse_delta cco_input_get_mouse_delta(void) { return g_input_state.mouse_delta; }

cco_mouse_point cco_input_get_mouse_point(void) { return g_input_state.mouse_point; }

CcoBool cco_input_key_is_pressed(cco_input_key key) { return g_input_state.keys_now[key]; }

CcoBool cco_input_key_was_just_pressed(cco_input_key key) {
    return g_input_state.keys_now[key] == CCO_YES && g_input_state.keys_last[key] == CCO_NO;
}

CcoBool cco_input_key_was_just_released(cco_input_key key) {
    return g_input_state.keys_now[key] == CCO_NO && g_input_state.keys_last[key] == CCO_YES;
}

CcoBool cco_input_mouse_button_is_pressed(cco_input_mouse_button button) { return g_input_state.mouse_buttons_now[button]; }

CcoBool cco_input_mouse_button_was_just_pressed(cco_input_mouse_button button) {
    return g_input_state.mouse_buttons_now[button] == CCO_YES && g_input_state.mouse_buttons_last[button] == CCO_NO;
}

CcoBool cco_input_mouse_button_was_just_released(cco_input_mouse_button button) {
    return g_input_state.mouse_buttons_now[button] == CCO_NO && g_input_state.mouse_buttons_last[button] == CCO_YES;
}

static void handle_raw_mouse_input(RAWMOUSE *mouse) {
    if (mouse->us_flags == MOUSE_MOVE_RELATIVE) {
        const i32 delta_x = mouse->l_last_x;
        const i32 delta_y = mouse->l_last_y;

        g_input_state.mouse_delta_processing.x += delta_x;
        g_input_state.mouse_delta_processing.y += delta_y;
    }

    const u16 button_flags = mouse->us_button_flags;
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
        const i16 wheel_delta = (i16)mouse->us_button_data;

        // The delta of the mouse wheel is in multiples of 120.0f (WHEEL_DELTA)
        g_input_state.mouse_delta_processing.wheel += (f32)wheel_delta / WHEEL_DELTA;
    }
}

static void handle_raw_keyboard_input(RAWKEYBOARD *keyboard) {
    const u16 vkey = keyboard->VKey;
    const u16 flags = keyboard->Flags;

    const CcoBool pressed = (flags & RI_KEY_BREAK) == 0;
    g_input_state.keys_processing[v_key_to_cco_input_key_map[vkey]] = pressed;
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

    if (raw_input->header.dw_type == RIM_TYPEMOUSE) {
        handle_raw_mouse_input(&raw_input->data.mouse);
    } else if (raw_input->header.dw_type == RIM_TYPEKEYBOARD) {
        handle_raw_keyboard_input(&raw_input->data.keyboard);
    }
}