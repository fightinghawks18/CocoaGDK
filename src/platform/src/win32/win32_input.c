//
// Created by fightinghawks18 on 12/20/2025.
//

#include "cocoa_gdk/platform/input.h"

#include "cocoa_gdk/platform/win32/win32_input.h"

#include "cocoa_gdk/platform/win32/win32_gamepad.h"
#include "cocoa_gdk/platform/win32/win32_keyboard.h"
#include "cocoa_gdk/platform/win32/win32_mouse.h"

typedef struct {
    cco_window current_window;
    cco_bool enabled;
    cco_input_init_flags init_flags;
} g_input_state;

static g_input_state g_input = {0};

cco_result cco_input_init(const cco_input_init_flags init_flags) {
    if (init_flags & CCO_INPUT_INIT_KEYBOARD_BIT) {
        const cco_result keyboard_init_result = win32_keyboard_init();
        if (keyboard_init_result != CCO_SUCCESS) {
            CCO_LOG("Failed to initialize keyboard input!");
            return keyboard_init_result;
        }
    }
    if (init_flags & CCO_INPUT_INIT_MOUSE_BIT) {
        const cco_result mouse_init_result = win32_mouse_init();
        if (mouse_init_result != CCO_SUCCESS) {
            CCO_LOG("Failed to initialize mouse input!");
            return mouse_init_result;
        }
    }
    if (init_flags & CCO_INPUT_INIT_GAMEPAD_BIT) {
        const cco_result gamepad_init_result = win32_gamepad_init();
        if (gamepad_init_result != CCO_SUCCESS) {
            CCO_LOG("Failed to initialize gamepad input!");
            return gamepad_init_result;
        }
    }

    g_input.init_flags = init_flags;
    return CCO_SUCCESS;
}

void cco_input_quit(void) {
    if (g_input.init_flags & CCO_INPUT_INIT_KEYBOARD_BIT)
        win32_keyboard_quit();
    if (g_input.init_flags & CCO_INPUT_INIT_MOUSE_BIT)
        win32_mouse_quit();
    if (g_input.init_flags & CCO_INPUT_INIT_GAMEPAD_BIT)
        win32_gamepad_quit();
}

void cco_input_poll(void) {
    if (!g_input.enabled)
        return;

    if (g_input.init_flags & CCO_INPUT_INIT_KEYBOARD_BIT)
        win32_keyboard_poll();
    if (g_input.init_flags & CCO_INPUT_INIT_MOUSE_BIT)
        win32_mouse_poll();
    if (g_input.init_flags & CCO_INPUT_INIT_GAMEPAD_BIT)
        win32_gamepad_poll();
}

void cco_input_clear_hardware_state(void) {
    if (g_input.init_flags & CCO_INPUT_INIT_KEYBOARD_BIT)
        win32_keyboard_clear_hardware_state();
    if (g_input.init_flags & CCO_INPUT_INIT_MOUSE_BIT)
        win32_mouse_clear_hardware_state();
}

void cco_input_give_window_focus(cco_window window) {
    cco_input_clear_hardware_state();

    g_input.current_window = window;
}

void cco_input_enable(void) { g_input.enabled = CCO_YES; }

void cco_input_disable(void) { g_input.enabled = CCO_NO; }

void cco_win32_handle_raw_input(LPARAM lparam) {
    if (!g_input.enabled)
        return;

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

    if (raw_input->header.dwType == RIM_TYPEMOUSE && g_input.init_flags & CCO_INPUT_INIT_MOUSE_BIT)
        win32_mouse_handle_raw_input(&raw_input->data.mouse);
    else if (raw_input->header.dwType == RIM_TYPEKEYBOARD && g_input.init_flags & CCO_INPUT_INIT_KEYBOARD_BIT)
        win32_handle_raw_keyboard_input(&raw_input->data.keyboard);
}

cco_window cco_input_get_active_window(void) {
    return g_input.current_window;
}