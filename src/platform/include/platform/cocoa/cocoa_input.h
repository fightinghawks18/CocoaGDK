//
// Created by fightinghawks18 on 12/20/25.
//

#pragma once

#ifdef __APPLE__

typedef enum {
    CCO_COCOA_INPUT_EVENT_HANDLED,
    CCO_COCOA_INPUT_EVENT_UNHANDLED
} CcoCocoaInputEventResult;

#ifdef __OBJC__

#include <AppKit/AppKit.h>
CcoCocoaInputEventResult ccoInputHandleCocoaEvent(NSEvent *event);

#endif // __OBJC__

#endif // __APPLE__