//
// Created by fightinghawks18 on 12/20/25.
//

#pragma once

#ifdef _WIN32

#include <windows.h>
#include "platform/input_mouse.h"
#include "core/results.h"

typedef struct {
    RAWINPUTDEVICE m;
    cco_bool processing[MOUSE_BUTTON_COUNT];
    cco_bool now[MOUSE_BUTTON_COUNT];
    cco_bool last[MOUSE_BUTTON_COUNT];

    cco_mouse_delta delta_processing;

    cco_mouse_point point;
    cco_mouse_delta delta;
} win32_mouse_state;

cco_result win32_mouse_init(void);
void win32_mouse_quit(void);

void win32_handle_raw_mouse_input(RAWMOUSE *m);
void win32_poll_mouse_input(void);
void win32_mouse_clear_hardware_state(void);

win32_mouse_state *win32_get_mouse_state(void);

#endif // _WIN32
