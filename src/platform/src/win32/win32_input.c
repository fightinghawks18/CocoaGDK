//
// Created by fightinghawks18 on 12/20/2025.
//

#include "platform/win32/win32_input.h"
#include "platform/input.h"
#include <hidusage.h>

static const CcoInputKey vKeyToCcoInputKeyMap[] = {
    ['W'] = CCO_INPUT_KEY_W,
    ['A'] = CCO_INPUT_KEY_A,
    ['S'] = CCO_INPUT_KEY_S,
    ['D'] = CCO_INPUT_KEY_D,

    [VK_SPACE] = CCO_INPUT_KEY_SPACE,
    [VK_CONTROL] = CCO_INPUT_KEY_CTRL,
    [VK_ESCAPE] = CCO_INPUT_KEY_ESC,
};

static struct {
    RAWINPUTDEVICE rids[2];

    CcoWindow currentWindow;
    CcoBool enabled;

    CcoBool keysProcessing[KEY_COUNT];
    CcoBool keysNow[KEY_COUNT];
    CcoBool keysLast[KEY_COUNT];

    CcoBool mouseButtonsProcessing[MOUSE_BUTTON_COUNT];
    CcoBool mouseButtonsNow[MOUSE_BUTTON_COUNT];
    CcoBool mouseButtonsLast[MOUSE_BUTTON_COUNT];

    CcoMouseDelta mouseDeltaProcessing;

    CcoMousePoint mousePoint;
    CcoMouseDelta mouseDelta;
} gInputState;

CcoResult ccoInputInit(void) {
    memset(&gInputState, 0, sizeof(gInputState));

    RAWINPUTDEVICE *keyboard = &gInputState.rids[0];
    keyboard->usUsagePage = HID_USAGE_PAGE_GENERIC;
    keyboard->usUsage = HID_USAGE_GENERIC_KEYBOARD;
    keyboard->dwFlags = 0;
    keyboard->hwndTarget = NULL;

    RAWINPUTDEVICE *mouse = &gInputState.rids[1];
    mouse->usUsagePage = HID_USAGE_PAGE_GENERIC;
    mouse->usUsage = HID_USAGE_GENERIC_MOUSE;
    mouse->dwFlags = 0;
    mouse->hwndTarget = NULL;

    if (!RegisterRawInputDevices(gInputState.rids, 2, sizeof(RAWINPUTDEVICE))) {
        CCO_LOG("Failed to register raw input devices for win32!");
        return CCO_FAIL_INPUT_INIT_ERROR;
    }

    return CCO_SUCCESS;
}

void ccoInputQuit(void) {
    RAWINPUTDEVICE *keyboard = &gInputState.rids[0];
    keyboard->usUsagePage = HID_USAGE_PAGE_GENERIC;
    keyboard->usUsage = HID_USAGE_GENERIC_KEYBOARD;
    keyboard->dwFlags = RIDEV_REMOVE;
    keyboard->hwndTarget = NULL;

    RAWINPUTDEVICE *mouse = &gInputState.rids[1];
    mouse->usUsagePage = HID_USAGE_PAGE_GENERIC;
    mouse->usUsage = HID_USAGE_GENERIC_MOUSE;
    mouse->dwFlags = RIDEV_REMOVE;
    mouse->hwndTarget = NULL;

    if (!RegisterRawInputDevices(gInputState.rids, 2, sizeof(RAWINPUTDEVICE))) {
        CCO_LOG("Failed to unregister raw input devices for win32!");
    }

    memset(&gInputState, 0, sizeof(gInputState));
}

void ccoInputPoll(void) {
    // Cache old key/button state
    memcpy(gInputState.keysLast, gInputState.keysNow, sizeof(gInputState.keysNow));
    memcpy(gInputState.mouseButtonsLast, gInputState.mouseButtonsNow, sizeof(gInputState.mouseButtonsNow));

    // Cache new key/button state
    memcpy(gInputState.keysNow, gInputState.keysProcessing, sizeof(gInputState.keysProcessing));
    memcpy(gInputState.mouseButtonsNow, gInputState.mouseButtonsProcessing, sizeof(gInputState.mouseButtonsProcessing));

    POINT cursor;
    GetCursorPos(&cursor);

    gInputState.mouseDelta = gInputState.mouseDeltaProcessing;
    gInputState.mousePoint.x = cursor.x;
    gInputState.mousePoint.y = cursor.y;

    gInputState.mouseDeltaProcessing.x = 0;
    gInputState.mouseDeltaProcessing.y = 0;
    gInputState.mouseDeltaProcessing.wheel = 0;
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
    gInputState.mouseDeltaProcessing.wheel = 0;
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

void ccoInputEnable(void) { gInputState.enabled = CCO_YES; }

void ccoInputDisable(void) { gInputState.enabled = CCO_NO; }

CcoWindow ccoInputGetActiveWindow(void) { return gInputState.currentWindow; }

CcoMouseDelta ccoInputGetMouseDelta(void) { return gInputState.mouseDelta; }

CcoMousePoint ccoInputGetMousePoint(void) { return gInputState.mousePoint; }

CcoBool ccoInputKeyIsPressed(CcoInputKey key) { return gInputState.keysNow[key]; }

CcoBool ccoInputKeyWasJustPressed(CcoInputKey key) {
    return gInputState.keysNow[key] == CCO_YES && gInputState.keysLast[key] == CCO_NO;
}

CcoBool ccoInputKeyWasJustReleased(CcoInputKey key) {
    return gInputState.keysNow[key] == CCO_NO && gInputState.keysLast[key] == CCO_YES;
}

CcoBool ccoInputMouseButtonIsPressed(CcoInputMouseButton button) { return gInputState.mouseButtonsNow[button]; }

CcoBool ccoInputMouseButtonWasJustPressed(CcoInputMouseButton button) {
    return gInputState.mouseButtonsNow[button] == CCO_YES && gInputState.mouseButtonsLast[button] == CCO_NO;
}

CcoBool ccoInputMouseButtonWasJustReleased(CcoInputMouseButton button) {
    return gInputState.mouseButtonsNow[button] == CCO_NO && gInputState.mouseButtonsLast[button] == CCO_YES;
}

static void handleRawMouseInput(RAWMOUSE *mouse) {
    if (mouse->usFlags == MOUSE_MOVE_RELATIVE) {
        const i32 deltaX = mouse->lLastX;
        const i32 deltaY = mouse->lLastY;

        gInputState.mouseDeltaProcessing.x += deltaX;
        gInputState.mouseDeltaProcessing.y += deltaY;
    }

    const u16 buttonFlags = mouse->usButtonFlags;
    if (buttonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_LEFT] = CCO_YES;
    if (buttonFlags & RI_MOUSE_LEFT_BUTTON_UP)
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_LEFT] = CCO_NO;
    if (buttonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_RIGHT] = CCO_YES;
    if (buttonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_RIGHT] = CCO_NO;
    if (buttonFlags & RI_MOUSE_BUTTON_3_DOWN)
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_3] = CCO_YES;
    if (buttonFlags & RI_MOUSE_BUTTON_3_UP)
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_3] = CCO_NO;
    if (buttonFlags & RI_MOUSE_BUTTON_4_DOWN)
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_4] = CCO_YES;
    if (buttonFlags & RI_MOUSE_BUTTON_4_UP)
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_4] = CCO_NO;
    if (buttonFlags & RI_MOUSE_BUTTON_5_DOWN)
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_5] = CCO_YES;
    if (buttonFlags & RI_MOUSE_BUTTON_5_UP)
        gInputState.mouseButtonsProcessing[CCO_INPUT_MOUSE_BUTTON_5] = CCO_NO;
    if (buttonFlags & RI_MOUSE_WHEEL) {
        const i16 wheelDelta = (i16)mouse->usButtonData;

        // The delta of the mouse wheel is in multiples of 120.0f (WHEEL_DELTA)
        gInputState.mouseDeltaProcessing.wheel += (f32)wheelDelta / WHEEL_DELTA;
    }
}

static void handleRawKeyboardInput(RAWKEYBOARD *keyboard) {
    const u16 vkey = keyboard->VKey;
    const u16 flags = keyboard->Flags;

    const CcoBool pressed = (flags & RI_KEY_BREAK) == 0;
    gInputState.keysProcessing[vKeyToCcoInputKeyMap[vkey]] = pressed;
}

void ccoInputHandleRawInput(HWND hWnd, LPARAM lParam) {
    HRAWINPUT rawInputHeader = (HRAWINPUT)lParam;

    u32 rawDataSize = 0;
    GetRawInputData(rawInputHeader, RID_INPUT, NULL, &rawDataSize, sizeof(RAWINPUTHEADER));

    LPBYTE rawInputBuffer = malloc(rawDataSize);
    if (!rawInputBuffer)
        return;

    if (GetRawInputData(rawInputHeader, RID_INPUT, rawInputBuffer, &rawDataSize, sizeof(RAWINPUTHEADER)) !=
        rawDataSize) {
        free(rawInputBuffer);
        return;
    }

    RAWINPUT *rawInput = (RAWINPUT *)rawInputBuffer;

    if (rawInput->header.dwType == RIM_TYPEMOUSE) {
        handleRawMouseInput(&rawInput->data.mouse);
    } else if (rawInput->header.dwType == RIM_TYPEKEYBOARD) {
        handleRawKeyboardInput(&rawInput->data.keyboard);
    }
}