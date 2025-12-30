//
// Created by fightinghawks18 on 12/30/2025.
//

#include "platform/win32/win32_mouse.h"

#include <hidusage.h>
#include <windows.h>

#include "core/log.h"
#include "platform/window.h"

static win32_mouse_state g_mouse = {0};

cco_result win32_mouse_init(void) {
    g_mouse.m.usUsagePage = HID_USAGE_PAGE_GENERIC;
    g_mouse.m.usUsage = HID_USAGE_GENERIC_MOUSE;
    g_mouse.m.dwFlags = 0;
    g_mouse.m.hwndTarget = NULL;

    if (!RegisterRawInputDevices(&g_mouse.m, 1, sizeof(RAWINPUTDEVICE))) {
        CCO_LOG("Failed to register mouse input for win32!");
        return CCO_FAIL_INPUT_INIT_ERROR;
    }

    CCO_LOG("Initialized mouse input!");
    return CCO_SUCCESS;
}

void win32_mouse_quit(void) {
    g_mouse.m.usUsagePage = HID_USAGE_PAGE_GENERIC;
    g_mouse.m.usUsage = HID_USAGE_GENERIC_MOUSE;
    g_mouse.m.dwFlags = RIDEV_REMOVE;
    g_mouse.m.hwndTarget = NULL;

    if (!RegisterRawInputDevices(&g_mouse.m, 1, sizeof(RAWINPUTDEVICE))) {
        CCO_LOG("Failed to unregister mouse input for win32!");
    }

    CCO_LOG("Closed mouse input!");
}

cco_mouse_delta cco_input_get_mouse_delta(void) { return g_mouse.delta; }
cco_mouse_point cco_input_get_mouse_point(void) { return g_mouse.point; }
cco_bool cco_input_mouse_button_is_pressed(const cco_input_mouse_button button) { return g_mouse.now[button]; }

cco_bool cco_input_mouse_button_was_just_pressed(const cco_input_mouse_button button) {
    return g_mouse.now[button] == CCO_YES && g_mouse.last[button] == CCO_NO;
}

cco_bool cco_input_mouse_button_was_just_released(const cco_input_mouse_button button) {
    return g_mouse.now[button] == CCO_NO && g_mouse.last[button] == CCO_YES;
}

void win32_handle_raw_mouse_input(RAWMOUSE *m) {
    if (m->usFlags == MOUSE_MOVE_RELATIVE) {
        const i32 delta_x = m->lLastX;
        const i32 delta_y = m->lLastY;

        g_mouse.delta_processing.x += (f32)delta_x;
        g_mouse.delta_processing.y += (f32)delta_y;
    }

    const u16 button_flags = m->usButtonFlags;
    if (button_flags & RI_MOUSE_LEFT_BUTTON_DOWN)
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_LEFT] = CCO_YES;
    if (button_flags & RI_MOUSE_LEFT_BUTTON_UP)
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_LEFT] = CCO_NO;
    if (button_flags & RI_MOUSE_RIGHT_BUTTON_DOWN)
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_RIGHT] = CCO_YES;
    if (button_flags & RI_MOUSE_RIGHT_BUTTON_UP)
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_RIGHT] = CCO_NO;
    if (button_flags & RI_MOUSE_BUTTON_3_DOWN)
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_3] = CCO_YES;
    if (button_flags & RI_MOUSE_BUTTON_3_UP)
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_3] = CCO_NO;
    if (button_flags & RI_MOUSE_BUTTON_4_DOWN)
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_4] = CCO_YES;
    if (button_flags & RI_MOUSE_BUTTON_4_UP)
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_4] = CCO_NO;
    if (button_flags & RI_MOUSE_BUTTON_5_DOWN)
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_5] = CCO_YES;
    if (button_flags & RI_MOUSE_BUTTON_5_UP)
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_5] = CCO_NO;
    if (button_flags & RI_MOUSE_WHEEL) {
        const i16 wheel_delta = (i16)m->usButtonData;

        // The delta of the mouse wheel is in multiples of 120.0f (WHEEL_DELTA)
        g_mouse.delta_processing.wheel += (f32)wheel_delta / WHEEL_DELTA;
    }
}

void win32_poll_mouse_input(void) {
    memcpy(g_mouse.last, g_mouse.now, sizeof(g_mouse.now));
    memcpy(g_mouse.now, g_mouse.processing, sizeof(g_mouse.processing));

    POINT cursor;
    GetCursorPos(&cursor);
    g_mouse.point.x = cursor.x;
    g_mouse.point.y = cursor.y;

    g_mouse.delta = g_mouse.delta_processing;

    g_mouse.delta_processing.x = 0;
    g_mouse.delta_processing.y = 0;
    g_mouse.delta_processing.wheel = 0;
}

void win32_mouse_clear_hardware_state(void) {
    memset(g_mouse.processing, 0, sizeof(g_mouse.processing));
    g_mouse.delta_processing.x = 0;
    g_mouse.delta_processing.y = 0;
    g_mouse.delta_processing.wheel = 0;
}

win32_mouse_state *win32_get_mouse_state(void) { return &g_mouse; }