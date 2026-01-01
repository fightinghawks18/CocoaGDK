//
// Created by fightinghawks18 on 12/31/25.
//

#include "platform/cocoa/cocoa_mouse.h"

static cocoa_mouse g_mouse = {0};

cco_mouse_delta cco_input_get_mouse_delta(void) {
    return g_mouse.delta;
}

cco_mouse_point cco_input_get_mouse_point(void) {
    return g_mouse.point;
}

cco_bool cco_input_mouse_button_is_pressed(cco_input_mouse_button button) {
    return g_mouse.now[button];
}

cco_bool cco_input_mouse_button_was_just_pressed(cco_input_mouse_button button) {
    return g_mouse.now[button] == CCO_YES && g_mouse.last[button] == CCO_NO;
}

cco_bool cco_input_mouse_button_was_just_released(cco_input_mouse_button button) {
    return g_mouse.now[button] == CCO_NO && g_mouse.last[button] == CCO_YES;
}

void cocoa_mouse_clear_hardware_state(void) {
    memset(g_mouse.processing, 0, sizeof(g_mouse.processing));
}

cco_cocoa_input_event_result cocoa_mouse_handle_event(NSEvent *event) {
    switch ([event type]) {
    case NSEventTypeLeftMouseDown:
    case NSEventTypeLeftMouseUp: {
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_LEFT] = event.type == NSEventTypeLeftMouseUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    case NSEventTypeRightMouseDown:
    case NSEventTypeRightMouseUp: {
        g_mouse.processing[CCO_INPUT_MOUSE_BUTTON_RIGHT] = event.type == NSEventTypeRightMouseUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    case NSEventTypeOtherMouseDown:
    case NSEventTypeOtherMouseUp: {
        g_mouse.processing[mouse_number_to_cco_input_mouse_button_map[[event buttonNumber]]] = event.type == NSEventTypeOtherMouseUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    case NSEventTypeScrollWheel: {
        bool using_trackpad = [event hasPreciseScrollingDeltas] == YES;
        CGFloat scroll_delta = [event scrollingDeltaY];
        g_mouse.delta_processing.wheel = using_trackpad ? scroll_delta / 10.0f : scroll_delta;
        return CCO_COCOA_INPUT_EVENT_HANDLED;
    }
    case NSEventTypeMouseMoved: {
        g_mouse.delta_processing.x += [event deltaX];
        g_mouse.delta_processing.y += [event deltaY];

        CGPoint mouse_point = [event locationInWindow];
        g_mouse.point.x = mouse_point.x;
        g_mouse.point.y = mouse_point.y;
        return CCO_COCOA_INPUT_EVENT_HANDLED;
    }
    default: {
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    }
}

void cocoa_mouse_poll(void) {
    memcpy(g_mouse.last, g_mouse.now, sizeof(g_mouse.now));
    memcpy(g_mouse.now, g_mouse.processing, sizeof(g_mouse.processing));

    g_mouse.delta = g_mouse.delta_processing;
    g_mouse.delta_processing.x = 0;
    g_mouse.delta_processing.y = 0;
}