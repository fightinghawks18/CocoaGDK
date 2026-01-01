//
// Created by fightinghawks18 on 12/30/25.
//

#pragma once

#ifdef __APPLE__

#ifdef __OBJC__

#include "platform/input_mouse.h"

#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

#include "platform/cocoa/cocoa_input.h"

static const cco_input_mouse_button mouse_number_to_cco_input_mouse_button_map[] = {
    [0] = CCO_INPUT_MOUSE_BUTTON_LEFT,
    [2] = CCO_INPUT_MOUSE_BUTTON_RIGHT,
    [3] = CCO_INPUT_MOUSE_BUTTON_3,
    [4] = CCO_INPUT_MOUSE_BUTTON_4,
    [5] = CCO_INPUT_MOUSE_BUTTON_5,
};
typedef struct {
    cco_bool now[MOUSE_BUTTON_COUNT];
    cco_bool last[MOUSE_BUTTON_COUNT];
    cco_bool processing[MOUSE_BUTTON_COUNT];

    cco_mouse_point point;
    cco_mouse_delta delta;
    cco_mouse_delta delta_processing;
} cocoa_mouse;

void cocoa_mouse_clear_hardware_state(void);
cco_cocoa_input_event_result cocoa_mouse_handle_event(NSEvent *event);
void cocoa_mouse_poll(void);

#endif

#endif