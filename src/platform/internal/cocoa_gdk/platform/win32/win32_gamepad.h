//
// Created by fightinghawks18 on 12/30/2025.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32

#include "cocoa_gdk/platform/input_gamepad.h"
#include <windows.h>

#include <stdbool.h>
#include <GameInput.h>

static cco_gamepad_button bm_to_cco_gamepad_button_map[] = {
    [GameInputGamepadA] = CCO_GAMEPAD_BUTTON_DOWN,
    [GameInputGamepadB] = CCO_GAMEPAD_BUTTON_RIGHT,
    [GameInputGamepadX] = CCO_GAMEPAD_BUTTON_LEFT,
    [GameInputGamepadY] = CCO_GAMEPAD_BUTTON_UP,
    [GameInputGamepadLeftShoulder] = CCO_GAMEPAD_BUTTON_LEFT_BUMPER,
    [GameInputGamepadRightShoulder] = CCO_GAMEPAD_BUTTON_RIGHT_BUMPER,
    [GameInputGamepadLeftThumbstick] = CCO_GAMEPAD_BUTTON_LEFT_STICK,
    [GameInputGamepadRightThumbstick] = CCO_GAMEPAD_BUTTON_RIGHT_STICK,
    [GameInputGamepadDPadUp] = CCO_GAMEPAD_BUTTON_UP_DPAD,
    [GameInputGamepadDPadDown] = CCO_GAMEPAD_BUTTON_DOWN_DPAD,
    [GameInputGamepadDPadLeft] = CCO_GAMEPAD_BUTTON_LEFT_DPAD,
    [GameInputGamepadDPadRight] = CCO_GAMEPAD_BUTTON_RIGHT_DPAD,
    [GameInputGamepadMenu] = CCO_GAMEPAD_BUTTON_MENU,
    [GameInputGamepadView] = CCO_GAMEPAD_BUTTON_VIEW,
};

typedef struct {
    cco_bool enabled;

    IGameInputDevice *input_device;

    cco_gamepad_stick_pos sticks[2];
    f32 triggers[2];

    cco_gamepad_button now[CCO_GAMEPAD_BUTTON_COUNT];
    cco_gamepad_button last[CCO_GAMEPAD_BUTTON_COUNT];
} win32_gamepad;

typedef struct {
    IGameInput *game_input;
    GameInputCallbackToken cb_token;

    win32_gamepad gamepads[CCO_MAX_GAMEPAD_COUNT];
    u8 gamepad_count;
} win32_gamepad_state;

cco_result win32_gamepad_init(void);
void win32_gamepad_quit(void);
void win32_gamepad_poll(void);

#endif

#ifdef __cplusplus
}
#endif