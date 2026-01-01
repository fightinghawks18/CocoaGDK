//
// Created by fightinghawks18 on 12/31/25.
//

#include "cocoa_gdk/platform/cocoa/cocoa_keyboard.h"

static cocoa_keyboard g_keyboard = {0};

cco_bool cco_input_key_is_pressed(cco_input_key key) {
    return g_keyboard.now[key];
}

cco_bool cco_input_key_was_just_pressed(cco_input_key key) {
    return g_keyboard.now[key] == CCO_YES && g_keyboard.last[key] == CCO_NO;
}

cco_bool cco_input_key_was_just_released(cco_input_key key) {
    return g_keyboard.now[key] == CCO_NO && g_keyboard.last[key] == CCO_YES;
}

void cocoa_keyboard_clear_hardware_state(void) {
    memset(g_keyboard.processing, 0, sizeof(g_keyboard.processing));
}

cco_cocoa_input_event_result cocoa_keyboard_handle_event(NSEvent *event) {
    switch ([event type]) {
    case NSEventTypeFlagsChanged: {
        u16 key_code = [event keyCode];
        NSEventModifierFlags modifier_flags = [event modifierFlags];
        
        const cco_input_key key = k_vk_to_cco_input_key_map[key_code];
        if (key == CCO_INPUT_KEY_UNKNOWN)
            return CCO_COCOA_INPUT_EVENT_UNHANDLED;
        
        NSEventModifierFlags relevant_flag = 0;
        switch (key_code) {
        case kVK_Shift:
        case kVK_RightShift: {
            relevant_flag = NSEventModifierFlagShift;
            break;
        }
        case kVK_Control:
        case kVK_RightControl: {
            relevant_flag = NSEventModifierFlagControl;
            break;
        }
        case kVK_Command:
        case kVK_RightCommand: {
            relevant_flag = NSEventModifierFlagCommand;
            break;
        }
        case kVK_Option:
        case kVK_RightOption: {
            relevant_flag = NSEventModifierFlagOption;
            break;
        }
        case kVK_CapsLock: {
            relevant_flag = NSEventModifierFlagCapsLock;
            break;
        }
        }
        
        bool is_pressed = (modifier_flags & relevant_flag) != 0;
        g_keyboard.processing[key] = is_pressed ? CCO_YES : CCO_NO;
        return CCO_COCOA_INPUT_EVENT_HANDLED;
    }
    case NSEventTypeKeyDown:
    case NSEventTypeKeyUp: {
        const cco_input_key key = k_vk_to_cco_input_key_map[[event keyCode]];
        if (key == CCO_INPUT_KEY_UNKNOWN)
            return CCO_COCOA_INPUT_EVENT_UNHANDLED;
        
        g_keyboard.processing[key] = event.type == NSEventTypeKeyUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_HANDLED;
    }
    default: {
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    }
}

void cocoa_keyboard_poll(void) {
    memcpy(g_keyboard.last, g_keyboard.now, sizeof(g_keyboard.now));
    memcpy(g_keyboard.now, g_keyboard.processing, sizeof(g_keyboard.processing));
}