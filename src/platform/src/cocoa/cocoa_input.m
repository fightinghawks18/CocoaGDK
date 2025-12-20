//
// Created by fightinghawks18 on 12/20/25.
//

#include "platform/cocoa/cocoa_input.h"
#include "platform/input.h"

#import <Carbon/Carbon.h>

static const CcoInputKey kVkToCcoInputKeyMap[] = {
    [kVK_ANSI_W] = CCO_INPUT_KEY_W,
    [kVK_ANSI_A] = CCO_INPUT_KEY_A,
    [kVK_ANSI_S] = CCO_INPUT_KEY_S,
    [kVK_ANSI_D] = CCO_INPUT_KEY_D,

    [kVK_Space] = CCO_INPUT_KEY_SPACE,
    [kVK_Control] = CCO_INPUT_KEY_CTRL,
    [kVK_Escape] = CCO_INPUT_KEY_ESC,
};

static const CcoInputMouseButton mouseNumberToCcoInputMouseButtonMap[] = {
    [0] = CCO_INPUT_MOUSE_BUTTON_LEFT,
    [2] = CCO_INPUT_MOUSE_BUTTON_RIGHT,
    [3] = CCO_INPUT_MOUSE_BUTTON_3,
    [4] = CCO_INPUT_MOUSE_BUTTON_4,
    [5] = CCO_INPUT_MOUSE_BUTTON_5,
};

static struct {
    CcoWindow currentWindow;
    CcoBool enabled;

    CcoBool keysProcessing[10];
    CcoBool keysNow[10];
    CcoBool keysLast[10];

    CcoBool mouseButtonsProcessing[5];
    CcoBool mouseButtonsNow[5];
    CcoBool mouseButtonsLast[5];

    CcoMousePoint mousePointProcessing;
    CcoMouseDelta mouseDeltaProcessing;

    CcoMousePoint mousePoint;
    CcoMouseDelta mouseDelta;
} gInputState;

CcoResult ccoInputInit(void) {
    memset(&gInputState, 0, sizeof(gInputState));
    return CCO_SUCCESS;
}

void ccoInputQuit(void) {
    memset(&gInputState, 0, sizeof(gInputState));
}

void ccoInputPoll(void) {
    // Cache old key/button state
    memcpy(gInputState.keysLast, gInputState.keysNow, sizeof(gInputState.keysNow));
    memcpy(gInputState.mouseButtonsLast, gInputState.mouseButtonsNow, sizeof(gInputState.mouseButtonsNow));

    // Cache new key/button state
    memcpy(gInputState.keysNow, gInputState.keysProcessing, sizeof(gInputState.keysProcessing));
    memcpy(gInputState.mouseButtonsNow, gInputState.mouseButtonsProcessing, sizeof(gInputState.mouseButtonsProcessing));

    gInputState.mouseDelta = gInputState.mouseDeltaProcessing;
    gInputState.mousePoint = gInputState.mousePointProcessing;

    gInputState.mouseDeltaProcessing.x = 0;
    gInputState.mouseDeltaProcessing.y = 0;
}

void ccoInputClearFrameState(void) {
    memset(gInputState.keysNow, 0, sizeof(gInputState.keysNow));
    memset(gInputState.keysLast, 0, sizeof(gInputState.keysLast));
    memset(gInputState.mouseButtonsNow, 0, sizeof(gInputState.mouseButtonsNow));
    memset(gInputState.mouseButtonsLast, 0, sizeof(gInputState.mouseButtonsLast));

    gInputState.mouseDelta.x = 0;
    gInputState.mouseDelta.y = 0;
    gInputState.mouseDeltaProcessing.x = 0;
    gInputState.mouseDeltaProcessing.y = 0;
}

void ccoInputClearHardwareState(void) {
    memset(gInputState.keysProcessing, 0, sizeof(gInputState.keysProcessing));
    memset(gInputState.mouseButtonsProcessing, 0, sizeof(gInputState.mouseButtonsProcessing));
}

void ccoInputGiveWindowFocus(CcoWindow window) {
    ccoInputClearFrameState();
    ccoInputClearHardwareState();

    gInputState.currentWindow = window;
}

void ccoInputEnable(void) {
    gInputState.enabled = CCO_YES;
}

void ccoInputDisable(void) {
    gInputState.enabled = CCO_NO;
}

CcoWindow ccoInputGetActiveWindow(void) {
    return gInputState.currentWindow;
}

CcoMouseDelta ccoInputGetMouseDelta(void) {
    return gInputState.mouseDelta;
}

CcoMousePoint ccoInputGetMousePoint(void) {
    return gInputState.mousePoint;
}

CcoBool ccoInputKeyIsPressed(CcoInputKey key) {
    return gInputState.keysNow[key];
}

CcoBool ccoInputKeyWasJustPressed(CcoInputKey key) {
    return gInputState.keysNow[key] == CCO_YES && gInputState.keysLast[key] == CCO_NO;
}

CcoBool ccoInputKeyWasJustReleased(CcoInputKey key) {
    return gInputState.keysNow[key] == CCO_NO && gInputState.keysLast[key] == CCO_YES;
}

CcoBool ccoInputMouseButtonIsPressed(CcoInputMouseButton button) {
    return gInputState.mouseButtonsNow[button];
}

CcoBool ccoInputMouseButtonWasJustPressed(CcoInputMouseButton button) {
    return gInputState.mouseButtonsNow[button] == CCO_YES && gInputState.mouseButtonsLast[button] == CCO_NO;
}

CcoBool ccoInputMouseButtonWasJustReleased(CcoInputMouseButton button) {
    return gInputState.mouseButtonsNow[button] == CCO_NO && gInputState.mouseButtonsLast[button] == CCO_YES;
}

CcoCocoaInputEventResult ccoInputHandleCocoaEvent(NSEvent *event) {
    if (!gInputState.enabled || !gInputState.currentWindow)
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;

    NSWindow *cocoaWindow = [event window];
    if (ccoWindowGetHandle(gInputState.currentWindow) != cocoaWindow)
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    
    switch (event.type) {
    case NSEventTypeKeyDown:
    case NSEventTypeKeyUp: {
        gInputState.keysProcessing[kVkToCcoInputKeyMap[event.keyCode]] = event.type == NSEventTypeKeyUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_HANDLED;
    }
    case NSEventTypeLeftMouseDown:
    case NSEventTypeLeftMouseUp: {
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_LEFT] = event.type == NSEventTypeLeftMouseUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    case NSEventTypeRightMouseDown:
    case NSEventTypeRightMouseUp: {
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_RIGHT] = event.type == NSEventTypeRightMouseUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    case NSEventTypeOtherMouseDown:
    case NSEventTypeOtherMouseUp: {
        gInputState.mouseButtonsProcessing[mouseNumberToCcoInputMouseButtonMap[event.buttonNumber]] = event.type == NSEventTypeOtherMouseUp ? CCO_NO : CCO_YES;
        return CCO_COCOA_INPUT_EVENT_UNHANDLED;
    }
    case NSEventTypeMouseMoved: {
        gInputState.mouseDeltaProcessing.x += [event deltaX];
        gInputState.mouseDeltaProcessing.y += [event deltaY];

        CGPoint mousePoint = [event locationInWindow];
        gInputState.mousePointProcessing.x = mousePoint.x;
        gInputState.mousePointProcessing.y = mousePoint.y;
        return CCO_COCOA_INPUT_EVENT_HANDLED;
    }
    default: break;
    }
    return CCO_COCOA_INPUT_EVENT_UNHANDLED;
}