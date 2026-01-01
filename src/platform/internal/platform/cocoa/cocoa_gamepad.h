//
// Created by fightinghawks18 on 12/30/25.
//

#pragma once

#ifdef __APPLE__

#ifdef __OBJC__

#include "platform/input_gamepad.h"
#include <GameController/GameController.h>

typedef struct {
    cco_bool enabled;
    GCController *controller;

    cco_gamepad_stick_pos sticks[2];
    f32 triggers[2];

    cco_gamepad_button now[CCO_GAMEPAD_BUTTON_COUNT];
    cco_gamepad_button last[CCO_GAMEPAD_BUTTON_COUNT];
} cocoa_gamepad;

typedef struct {
    cocoa_gamepad gamepads[CCO_MAX_GAMEPAD_COUNT];
    u8 gamepad_count;
} cocoa_gamepad_state;

cco_result cocoa_gamepad_init(void);
void cocoa_gamepad_quit(void);
void cocoa_gamepad_poll(void);

#endif

#endif