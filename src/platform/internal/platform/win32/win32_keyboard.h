//
// Created by fightinghawks18 on 12/30/2025.
//

#pragma once

#ifdef _WIN32

#include <windows.h>
#include "platform/input_keyboard.h"
#include "core/results.h"

typedef struct {
    RAWINPUTDEVICE kb;
    cco_bool processing[KEY_COUNT];
    cco_bool now[KEY_COUNT];
    cco_bool last[KEY_COUNT];
} win32_keyboard_state;

cco_result win32_keyboard_init(void);
void win32_keyboard_quit(void);

void win32_handle_raw_keyboard_input(const RAWKEYBOARD *kb);
void win32_poll_keyboard_input(void);
void win32_keyboard_clear_hardware_state(void);

win32_keyboard_state *win32_get_keyboard_state(void);

#endif // _WIN32