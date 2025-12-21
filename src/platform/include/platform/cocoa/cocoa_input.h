//
// Created by fightinghawks18 on 12/20/25.
//

#pragma once

#ifdef __APPLE__

#include "platform/exports.h"

typedef enum {
    CCO_COCOA_INPUT_EVENT_HANDLED,
    CCO_COCOA_INPUT_EVENT_UNHANDLED
} cco_cocoa_input_event_result;

#ifdef __OBJC__

#include <AppKit/AppKit.h>
CCO_PLATFORM_API cco_cocoa_input_event_result cco_input_handle_cocoa_event(NSEvent *event);

#endif // __OBJC__

#endif // __APPLE__