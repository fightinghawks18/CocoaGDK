//
// Created by fightinghawks18 on 12/20/25.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__

typedef enum {
    CCO_COCOA_INPUT_EVENT_HANDLED,
    CCO_COCOA_INPUT_EVENT_UNHANDLED
} cco_cocoa_input_event_result;

#ifdef __OBJC__

#include <AppKit/AppKit.h>
cco_cocoa_input_event_result cco_input_handle_cocoa_event(NSEvent *event);

#endif // __OBJC__

#endif // __APPLE__

#ifdef __cplusplus
}
#endif