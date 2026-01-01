//
// Created by fightinghawks18 on 12/31/25
//

#include "cocoa_gdk/platform/cocoa/cocoa_gamepad.h"
#include "cocoa_gdk/core/log.h"

static cocoa_gamepad_state g_gamepad = {0};

cocoa_gamepad *get_gamepad_from_controller(GCController *controller) {
    for (u8 g = 0; g < CCO_MAX_GAMEPAD_COUNT; ++g) {
        if (g_gamepad.gamepads[g].controller == controller)
            return &g_gamepad.gamepads[g];
    }
    return CCO_NIL;
}

cco_gamepad_id get_gamepad_id_from_controller(GCController *controller) {
    for (u8 g = 0; g < CCO_MAX_GAMEPAD_COUNT; ++g) {
        if (g_gamepad.gamepads[g].controller == controller)
            return g;
    }
    return UINT8_MAX;
}

void handle_gamepad_connection(GCController *controller) {
    for (u8 g = 0; g < CCO_MAX_GAMEPAD_COUNT; ++g) {
        cocoa_gamepad *gamepad = &g_gamepad.gamepads[g];
        if (!gamepad->controller) {
            gamepad->controller = controller;
            CCO_LOG("Connected new gamepad (%s) to slot %d!", controller.vendorName.UTF8String, g);
            g_gamepad.gamepad_count++;
            return;
        }
    }
    CCO_LOG("A new gamepad (%s) connection was recognized, but the max amount (%d) of gamepads has already been reached!",
            controller.vendorName.UTF8String, CCO_MAX_GAMEPAD_COUNT);
}

void handle_gamepad_disconnection(GCController *controller) {
    cocoa_gamepad *gamepad = get_gamepad_from_controller(controller);
    if (!gamepad) {
        CCO_LOG("A gamepad (%s) was attempting to disconnect, but it isn't registered? This may be a bug.", controller.vendorName.UTF8String);
        return;
    }
    CCO_LOG("Disconnected gamepad (%s) from slot %d!", controller.vendorName.UTF8String, get_gamepad_id_from_controller(controller));
    gamepad->controller = CCO_NIL;
    g_gamepad.gamepad_count--;
}

cco_result cocoa_gamepad_init(void) {
    [[NSNotificationCenter defaultCenter] 
        addObserverForName:GCControllerDidConnectNotification
        object:nil
        queue:nil
        usingBlock:^(NSNotification* note) {
            handle_gamepad_connection(note.object);
        }];
    
    [[NSNotificationCenter defaultCenter]
        addObserverForName:GCControllerDidDisconnectNotification
        object:nil
        queue:nil
        usingBlock:^(NSNotification* note) {
            handle_gamepad_disconnection(note.object);
        }];

    [GCController startWirelessControllerDiscoveryWithCompletionHandler:nil];
    
    for (u8 g = 0; g < CCO_MAX_GAMEPAD_COUNT; ++g) {
        cocoa_gamepad *gamepad = &g_gamepad.gamepads[g];
        gamepad->controller = CCO_NIL;
        gamepad->enabled = CCO_YES;
    }
    return CCO_SUCCESS;
}

void cocoa_gamepad_quit(void) {
    [GCController stopWirelessControllerDiscovery];
}

void query_gamepad_input(u8 id, cocoa_gamepad *gamepad) {
    if (!gamepad->controller.extendedGamepad)
        return;
    
    GCExtendedGamepad *state = gamepad->controller.extendedGamepad;

    cco_gamepad_stick_pos *left_stick = &gamepad->sticks[CCO_GAMEPAD_STICK_LEFT];
    left_stick->x_delta = state.leftThumbstick.xAxis.value - left_stick->x;
    left_stick->y_delta = state.leftThumbstick.yAxis.value - left_stick->y;
    left_stick->x = state.leftThumbstick.xAxis.value;
    left_stick->y = state.leftThumbstick.yAxis.value;

    cco_gamepad_stick_pos *right_stick = &gamepad->sticks[CCO_GAMEPAD_STICK_RIGHT];
    right_stick->x_delta = state.rightThumbstick.xAxis.value - right_stick->x;
    right_stick->y_delta = state.rightThumbstick.yAxis.value - right_stick->y;
    right_stick->x = state.rightThumbstick.xAxis.value;
    right_stick->y = state.rightThumbstick.yAxis.value;

    gamepad->triggers[CCO_GAMEPAD_TRIGGER_LEFT] = state.leftTrigger.value;
    gamepad->triggers[CCO_GAMEPAD_TRIGGER_RIGHT] = state.rightTrigger.value;

    memcpy(gamepad->last, gamepad->now, sizeof(gamepad->now));

    gamepad->now[CCO_GAMEPAD_BUTTON_DOWN] = state.buttonA.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_RIGHT] = state.buttonB.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_LEFT] = state.buttonX.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_UP] = state.buttonY.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_LEFT_BUMPER] = state.leftShoulder.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_RIGHT_BUMPER] = state.rightShoulder.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_LEFT_STICK] = state.leftThumbstickButton.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_RIGHT_STICK] = state.rightThumbstickButton.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_UP_DPAD] = state.dpad.up.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_DOWN_DPAD] = state.dpad.down.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_LEFT_DPAD] = state.dpad.left.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_RIGHT_DPAD] = state.dpad.right.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_MENU] = state.buttonMenu.pressed;
    gamepad->now[CCO_GAMEPAD_BUTTON_VIEW] = state.buttonOptions.pressed;
}

void cocoa_gamepad_poll(void) {
    for (u8 g = 0; g < CCO_MAX_GAMEPAD_COUNT; ++g) {
        cocoa_gamepad *gamepad = &g_gamepad.gamepads[g];
        if (!gamepad->controller || !gamepad->enabled)
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
    return g_gamepad.gamepads[gamepad_id].controller != CCO_NIL;
}

u8 cco_input_get_connected_gamepad_count(void) { return g_gamepad.gamepad_count; }
