//
// Created by fightinghawks18 on 12/30/2025.
//

#include "platform/win32/win32_gamepad.h"

#include "core/log.h"

static win32_gamepad_state g_gamepad = {0};

win32_gamepad *get_gamepad_from_input_device(IGameInputDevice *device) {
    for (u8 g = 0; g < CCO_MAX_GAMEPAD_COUNT; ++g) {
        if (g_gamepad.gamepads[g].input_device == device)
            return &g_gamepad.gamepads[g];
    }
    return CCO_NIL;
}

cco_gamepad_id get_gamepad_id_from_input_device(IGameInputDevice *device) {
    for (u8 g = 0; g < CCO_MAX_GAMEPAD_COUNT; ++g) {
        if (g_gamepad.gamepads[g].input_device == device)
            return g;
    }
    return UINT8_MAX;
}

void handle_gamepad_connection(IGameInputDevice *device) {
    for (u8 g = 0; g < CCO_MAX_GAMEPAD_COUNT; ++g) {
        win32_gamepad *gamepad = &g_gamepad.gamepads[g];
        if (!gamepad->input_device) {
            gamepad->input_device = device;
            CCO_LOG("Connected new gamepad to slot %d!", g);
            g_gamepad.gamepad_count++;
            return;
        }
    }
    CCO_LOG("A new gamepad connection was recognized, but the max amount (%d) of gamepads has already been reached!",
            CCO_MAX_GAMEPAD_COUNT);
}

void handle_gamepad_disconnection(IGameInputDevice *device) {
    win32_gamepad *gamepad = get_gamepad_from_input_device(device);
    if (!gamepad) {
        CCO_LOG("A gamepad was attempting to disconnect, but it isn't registered? This may be a bug.");
        return;
    }
    CCO_LOG("Disconnected gamepad from slot %d!", get_gamepad_id_from_input_device(device));
    gamepad->input_device = CCO_NIL;
    g_gamepad.gamepad_count--;
}

static void CALLBACK gamepad_change_callback(GameInputCallbackToken callback_token, void *context,
                                             IGameInputDevice *device, uint64_t timestamp,
                                             GameInputDeviceStatus current_status,
                                             GameInputDeviceStatus previous_status) {
    const cco_bool is_connected = current_status & GameInputDeviceConnected;
    const cco_bool was_connected = previous_status & GameInputDeviceConnected;
    if (is_connected)
        handle_gamepad_connection(device);
    if (!is_connected && was_connected)
        handle_gamepad_disconnection(device);
}

cco_result win32_gamepad_init(void) {
    HRESULT hr = GameInputCreate(&g_gamepad.game_input);
    if (FAILED(hr)) {
        CCO_LOG("Failed to initialize GameInput for gamepad input! %ld", hr);
        return CCO_FAIL_INPUT_GAMEPAD_INIT_ERROR;
    }

    for (u8 g = 0; g < CCO_MAX_GAMEPAD_COUNT; ++g) {
        g_gamepad.gamepads[g].enabled = CCO_YES;
    }

    hr = g_gamepad.game_input->lpVtbl->RegisterDeviceCallback(g_gamepad.game_input, CCO_NIL, GameInputKindGamepad,
                                                              GameInputDeviceConnected, GameInputBlockingEnumeration,
                                                              CCO_NIL, gamepad_change_callback, &g_gamepad.cb_token);
    if (FAILED(hr)) {
        CCO_LOG("Failed to initialize device callback for GameInput! %ld", hr);
        win32_gamepad_quit();
        return CCO_FAIL_INPUT_GAMEPAD_INIT_ERROR;
    }

    return CCO_SUCCESS;
}

void win32_gamepad_quit(void) {
    if (g_gamepad.game_input) {
        cco_bool success =
            g_gamepad.game_input->lpVtbl->UnregisterCallback(g_gamepad.game_input, g_gamepad.cb_token, UINT64_MAX);
        if (!success) {
            CCO_LOG("Failed to unregister device callback for GameInput!");
        }

        g_gamepad.game_input->lpVtbl->Release(g_gamepad.game_input);
        g_gamepad.game_input = CCO_NIL;
    }
}

void query_gamepad_input(u8 id, win32_gamepad *gamepad) {
    IGameInputReading *reading;
    const HRESULT hr = g_gamepad.game_input->lpVtbl->GetCurrentReading(g_gamepad.game_input, GameInputKindGamepad,
                                                                       gamepad->input_device, &reading);
    if (FAILED(hr)) {
        CCO_LOG("Failed to query gamepad input from gamepad (%d)! %ld", id, hr);
        return;
    }

    GameInputGamepadState state;
    reading->lpVtbl->GetGamepadState(reading, &state);
    reading->lpVtbl->Release(reading);

    cco_gamepad_stick_pos *left_stick = &gamepad->sticks[CCO_GAMEPAD_STICK_LEFT];
    left_stick->x_delta = state.leftThumbstickX - left_stick->x;
    left_stick->y_delta = state.leftThumbstickY - left_stick->y;
    left_stick->x = state.leftThumbstickX;
    left_stick->y = state.leftThumbstickY;

    cco_gamepad_stick_pos *right_stick = &gamepad->sticks[CCO_GAMEPAD_STICK_RIGHT];
    right_stick->x_delta = state.rightThumbstickX - right_stick->x;
    right_stick->y_delta = state.rightThumbstickY - right_stick->y;
    right_stick->x = state.rightThumbstickX;
    right_stick->y = state.rightThumbstickY;

    gamepad->triggers[CCO_GAMEPAD_TRIGGER_LEFT] = state.leftTrigger;
    gamepad->triggers[CCO_GAMEPAD_TRIGGER_RIGHT] = state.rightTrigger;

    memcpy(gamepad->last, gamepad->now, sizeof(gamepad->now));

    gamepad->now[CCO_GAMEPAD_BUTTON_DOWN] = (state.buttons & GameInputGamepadA) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_RIGHT] = (state.buttons & GameInputGamepadB) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_LEFT] = (state.buttons & GameInputGamepadX) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_UP] = (state.buttons & GameInputGamepadY) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_LEFT_BUMPER] = (state.buttons & GameInputGamepadLeftShoulder) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_RIGHT_BUMPER] = (state.buttons & GameInputGamepadRightShoulder) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_LEFT_STICK] = (state.buttons & GameInputGamepadLeftThumbstick) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_RIGHT_STICK] = (state.buttons & GameInputGamepadRightThumbstick) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_UP_DPAD] = (state.buttons & GameInputGamepadDPadUp) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_DOWN_DPAD] = (state.buttons & GameInputGamepadDPadDown) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_LEFT_DPAD] = (state.buttons & GameInputGamepadDPadLeft) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_RIGHT_DPAD] = (state.buttons & GameInputGamepadDPadRight) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_MENU] = (state.buttons & GameInputGamepadMenu) != 0;
    gamepad->now[CCO_GAMEPAD_BUTTON_VIEW] = (state.buttons & GameInputGamepadView) != 0;
}

void win32_gamepad_poll(void) {
    for (u8 g = 0; g < CCO_MAX_GAMEPAD_COUNT; ++g) {
        win32_gamepad *gamepad = &g_gamepad.gamepads[g];
        if (!gamepad->input_device || !gamepad->enabled)
            continue;
        query_gamepad_input(g, gamepad);
    }
}

cco_bool cco_input_is_gamepad_button_pressed(const cco_gamepad_id gamepad_id, const cco_gamepad_button button) {
    if (!cco_input_has_gamepad(gamepad_id)) {
        CCO_LOG("Gamepad (%d) doesn't exist, failed to check if a button is pressed", gamepad_id);
        return CCO_NO;
    }
    return g_gamepad.gamepads[gamepad_id].now[button] == CCO_YES;
}

cco_bool cco_input_was_gamepad_button_just_released(const cco_gamepad_id gamepad_id, const cco_gamepad_button button) {
    if (!cco_input_has_gamepad(gamepad_id)) {
        CCO_LOG("Gamepad (%d) doesn't exist, failed to check if a button was just released", gamepad_id);
        return CCO_NO;
    }
    return g_gamepad.gamepads[gamepad_id].now[button] == CCO_NO &&
           g_gamepad.gamepads[gamepad_id].last[button] == CCO_YES;
}

cco_bool cco_input_was_gamepad_button_just_pressed(const cco_gamepad_id gamepad_id, const cco_gamepad_button button) {
    if (!cco_input_has_gamepad(gamepad_id)) {
        CCO_LOG("Gamepad (%d) doesn't exist, failed to check if a button was just pressed", gamepad_id);
        return CCO_NO;
    }
    return g_gamepad.gamepads[gamepad_id].now[button] == CCO_YES &&
           g_gamepad.gamepads[gamepad_id].last[button] == CCO_NO;
}

cco_bool cco_input_read_gamepad_stick_pos(const cco_gamepad_id gamepad_id, const cco_gamepad_stick stick,
                                          cco_gamepad_stick_pos *out_data) {
    if (!cco_input_has_gamepad(gamepad_id)) {
        CCO_LOG("Gamepad (%d) doesn't exist, failed to read stick position", gamepad_id);
        return CCO_NO;
    }
    *out_data = g_gamepad.gamepads[gamepad_id].sticks[stick];
    return CCO_YES;
}

cco_bool cco_input_read_gamepad_trigger_pressure(const cco_gamepad_id gamepad_id, const cco_gamepad_trigger trigger,
                                                 f32 *out_pressure) {
    if (!cco_input_has_gamepad(gamepad_id)) {
        CCO_LOG("Gamepad (%d) doesn't exist, failed to read trigger pressure", gamepad_id);
        return CCO_NO;
    }
    *out_pressure = g_gamepad.gamepads[gamepad_id].triggers[trigger];
    return CCO_YES;
}

cco_bool cco_input_is_gamepad_trigger_pressed(const cco_gamepad_id gamepad_id, const cco_gamepad_trigger trigger,
                                              const f32 threshold) {
    f32 pressure;
    if (!cco_input_read_gamepad_trigger_pressure(gamepad_id, trigger, &pressure)) {
        CCO_LOG("Failed to check trigger pressure threshold! Does this gamepad (%d) exist?", gamepad_id);
        return CCO_NO;
    }
    return pressure > threshold;
}

void cco_input_disable_gamepad(const cco_gamepad_id gamepad_id) { g_gamepad.gamepads[gamepad_id].enabled = CCO_NO; }

void cco_input_enable_gamepad(const cco_gamepad_id gamepad_id) { g_gamepad.gamepads[gamepad_id].enabled = CCO_YES; }

cco_bool cco_input_has_gamepad(const cco_gamepad_id gamepad_id) {
    if (gamepad_id > CCO_MAX_GAMEPAD_COUNT) {
        CCO_LOG("Attempted to check gamepad (%d), but the id exceeded the max amount of gamepads allowed (%d)",
                gamepad_id, CCO_MAX_GAMEPAD_COUNT);
        return CCO_NO;
    }
    return g_gamepad.gamepads[gamepad_id].input_device != CCO_NIL;
}

u8 cco_input_get_connected_gamepad_count(void) { return g_gamepad.gamepad_count; }