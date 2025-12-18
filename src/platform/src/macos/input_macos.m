//
// Created by fightinghawks18 on 12/18/25.
//

#include "platform/input.h"
#include <stdlib.h>
#import <AppKit/AppKit.h>
#import <Carbon/Carbon.h>

static const u32 keysToCcoInputKeyMap[] = {
    [kVK_ANSI_W] = CCO_INPUT_KEY_W,
    [kVK_ANSI_A] = CCO_INPUT_KEY_A,
    [kVK_ANSI_S] = CCO_INPUT_KEY_S,
    [kVK_ANSI_D] = CCO_INPUT_KEY_D,
    [kVK_Space] = CCO_INPUT_KEY_SPACE,
    [kVK_Control] = CCO_INPUT_KEY_CTRL,
    [kVK_Escape] = CCO_INPUT_KEY_ESC,
};

static const u32 mouseKeysToCcoInputMouseKeyMap[] = {
    [0] = CCO_INPUT_MOUSE_KEY_M1,
    [1] = CCO_INPUT_MOUSE_KEY_M2,
    [2] = CCO_INPUT_MOUSE_KEY_M3,
    [3] = CCO_INPUT_MOUSE_KEY_M4,
    [4] = CCO_INPUT_MOUSE_KEY_M5,
};

// NOTE: We're using snapshots because NSResponder updates the field as soon as input was pressed/released
// We use this so when we update input, the input states that the user reads whatever it was at that time
struct CcoWindowInput_T {
    CcoWindow window;
    NSResponder *responder;

    CcoBool keysSnapshot[50];
    CcoBool keysNow[50];
    CcoBool keysLast[50];
    CcoBool mouseKeysSnapshot[10];
    CcoBool mouseKeysNow[10];
    CcoBool mouseKeysLast[10];
    
    i32 mouseXSnapshot;
    i32 mouseYSnapshot;
    i32 mouseWheelSnapshot;

    i32 mouseX;
    i32 mouseY;
    i32 mouseXLast;
    i32 mouseYLast;
    CcoMouseDelta mouseDelta;
};

@interface CcoWindowInputReader : NSResponder
@property (assign) CcoWindowInput input;
@end

@implementation CcoWindowInputReader
- (void)keyDown:(NSEvent *)event {
    NSWindow *window = (__bridge NSWindow *)ccoGetNativeWindowHandle(self.input->window);
    if (![window isKeyWindow])
        return;
    u16 code = [event keyCode];
    self.input->keysSnapshot[keysToCcoInputKeyMap[code]] = CCO_YES;
}
- (void)keyUp:(NSEvent *)event {
    NSWindow *window = (__bridge NSWindow *)ccoGetNativeWindowHandle(self.input->window);
    if (![window isKeyWindow])
        return;
    u16 code = [event keyCode];
    self.input->keysSnapshot[keysToCcoInputKeyMap[code]] = CCO_NO;
}
- (void)mouseUp:(NSEvent *)event {
    NSWindow *window = (__bridge NSWindow *)ccoGetNativeWindowHandle(self.input->window);
    if (![window isKeyWindow])
        return;
    u16 code = [event buttonNumber];
    self.input->mouseKeysSnapshot[CCO_INPUT_MOUSE_KEY_M1] = CCO_YES;
}
- (void)mouseDown:(NSEvent *)event {
    NSWindow *window = (__bridge NSWindow *)ccoGetNativeWindowHandle(self.input->window);
    if (![window isKeyWindow])
        return;
    u16 code = [event buttonNumber];
    self.input->mouseKeysSnapshot[CCO_INPUT_MOUSE_KEY_M1] = CCO_NO;
}
- (void)otherMouseUp:(NSEvent *)event {
    NSWindow *window = (__bridge NSWindow *)ccoGetNativeWindowHandle(self.input->window);
    if (![window isKeyWindow])
        return;
    u16 code = [event buttonNumber];
    self.input->mouseKeysSnapshot[mouseKeysToCcoInputMouseKeyMap[code]] = CCO_YES;
}
- (void)otherMouseDown:(NSEvent *)event {
    NSWindow *window = (__bridge NSWindow *)ccoGetNativeWindowHandle(self.input->window);
    if (![window isKeyWindow])
        return;
    u16 code = [event buttonNumber];
    self.input->mouseKeysSnapshot[mouseKeysToCcoInputMouseKeyMap[code]] = CCO_NO;
}
- (void)mouseMoved:(NSEvent *)event {
    self.input->mouseXSnapshot = [event locationInWindow].x;
    self.input->mouseYSnapshot = [event locationInWindow].y;
}
- (void)scrollWheel:(NSEvent *)event {
    self.input->mouseWheelSnapshot = event.scrollingDeltaY;
}
@end

CcoResult ccoCreateWindowInput(CcoWindow *window, CcoWindowInput *outInput) {
    CcoWindowInput input = calloc(1, sizeof(CcoWindowInput_T));
    if (!input)
        return CCO_FAIL_OUT_OF_MEMORY;
    
    NSWindow *windowHandle = (__bridge NSWindow*)ccoGetNativeWindowHandle(window);
    
    CcoWindowInputReader *reader = [[CcoWindowInputReader alloc] init];
    reader.input = input;
    
    [windowHandle makeFirstResponder:reader];
    input->window = window;
    input->responder = reader;
    
    *outInput = input;
    return CCO_SUCCESS;
}

void ccoDestroyWindowInput(CcoWindow *window, CcoWindowInput input) {
    if (input->responder) {
        [input->responder release];
        input->responder = nil;
        
        NSWindow *windowHandle = (__bridge NSWindow*)ccoGetNativeWindowHandle(window);
        [windowHandle resignFirstResponder];
    }
    free(input);
}

void ccoUpdateWindowInput(CcoWindowInput input) {
    memcpy(input->keysLast, input->keysNow, 50 * sizeof(CcoBool));
    memcpy(input->keysNow, input->keysSnapshot, 50 * sizeof(CcoBool));
    memcpy(input->mouseKeysLast, input->mouseKeysNow, 10 * sizeof(CcoBool));
    memcpy(input->mouseKeysNow, input->mouseKeysSnapshot, 10 * sizeof(CcoBool));
    
    input->mouseXLast = input->mouseX;
    input->mouseYLast = input->mouseY;
    
    input->mouseX = input->mouseXSnapshot;
    input->mouseY = input->mouseYSnapshot;
    
    input->mouseDelta.x = input->mouseXLast - input->mouseX;
    input->mouseDelta.y = input->mouseYLast - input->mouseY;
    input->mouseDelta.wheel = input->mouseWheelSnapshot;
}

CcoMouseDelta ccoWindowInputGetMouseDelta(CcoWindowInput input) {
    return input->mouseDelta;
}

CcoMousePosition ccoWindowInputGetMousePosition(CcoWindowInput input) {
    return (CcoMousePosition){ input->mouseX, input->mouseY };
}

CcoBool ccoWindowInputKeyIsPressed(CcoWindowInput input, CcoInputKey key) {
    return input->keysNow[key] == CCO_YES;
}

CcoBool ccoWindowInputKeyWasJustPressed(CcoWindowInput input, CcoInputKey key) {
    return input->keysNow[key] == CCO_YES
        && input->keysLast[key] == CCO_NO;
}

CcoBool ccoWindowInputKeyWasJustReleased(CcoWindowInput input, CcoInputKey key) {
    return input->keysLast[key] == CCO_YES
        && input->keysNow[key] == CCO_NO;
}

CcoBool ccoWindowInputMouseKeyIsPressed(CcoWindowInput input, CcoInputMouseKey mouseKey) {
    return input->keysNow[mouseKey];
}

CcoBool ccoWindowInputMouseKeyWasJustPressed(CcoWindowInput input, CcoInputMouseKey mouseKey) {
    return input->mouseKeysNow[mouseKey] == CCO_YES
        && input->mouseKeysLast[mouseKey] == CCO_NO;
}

CcoBool ccoWindowInputMouseKeyWasJustReleased(CcoWindowInput input, CcoInputMouseKey mouseKey) {
    return input->mouseKeysLast[mouseKey] == CCO_YES
        && input->mouseKeysNow[mouseKey] == CCO_NO;
}
