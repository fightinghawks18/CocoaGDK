//
// Created by fightinghawks18 on 12/19/25.
//

#include "platform/window.h"
#import <Cocoa/Cocoa.h>
#import <CoreFoundation/CoreFoundation.h>
#include "platform/cocoa/cocoa_input.h"

struct CcoWindow_T {
    NSWindow *window;
    CcoBool willClose;
    CcoBool focused;
};

@interface CocoaWindow : NSObject <NSWindowDelegate>
@property(nonatomic, assign) CcoWindow window;
@end

@implementation CocoaWindow
- (BOOL)windowShouldClose:(NSWindow *)sender {
    self.window->willClose = CCO_YES;
    return NO;
}
- (void)windowDidBecomeKey:(NSNotification *)notification {
    self.window->focused = CCO_YES;
}
- (void)windowDidResignKey:(NSNotification *)notification {
    self.window->focused = CCO_NO;
}

@end

CcoResult ccoWindowingInit() {
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
    return CCO_SUCCESS;
}

void ccoWindowingQuit() {}

CcoResult ccoCreateWindow(i32 x, i32 y, i32 width, i32 height, const char *title, CcoWindow *outWindow) {
    CcoWindow window = malloc(sizeof(CcoWindow_T));
    if (!window)
        return CCO_FAIL_OUT_OF_MEMORY;

    NSRect rect = NSMakeRect(x, y, width, height);
    NSWindowStyleMask styleMask = NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled |
                                  NSWindowStyleMaskMiniaturizable;
    NSWindow *win = [[NSWindow alloc] initWithContentRect:rect
                                                styleMask:styleMask
                                                  backing:NSBackingStoreBuffered
                                                    defer:YES];

    window->window = win;
    window->willClose = false;
    ccoWindowRename(window, title);

    CocoaWindow *delg = [[CocoaWindow alloc] init];
    delg.window = window;

    [window->window setDelegate:delg];
    [window->window makeKeyAndOrderFront:nil];

    *outWindow = window;
    return CCO_SUCCESS;
}

void ccoDestroyWindow(CcoWindow window) {
    if (window->window) {
        [[window->window delegate] release];
        [window->window release];
        window->window = nil;
    }
    free(window);
}

void ccoWindowMove(CcoWindow window, i32 x, i32 y) {
    NSPoint point = NSMakePoint(x, y);
    [window->window setFrameOrigin:point];
}
void ccoWindowResize(CcoWindow window, i32 width, i32 height) {
    NSRect windowRect = [window->window frame];
    NSRect rect = NSMakeRect(windowRect.origin.x, windowRect.origin.y, width, height);
    [window->window setFrame:rect display:YES];
}

void ccoWindowRename(CcoWindow window, const char *title) {
    @autoreleasepool {
        NSString *str = [NSString stringWithUTF8String:title];
        [window->window setTitle:str];
    }
}

void ccoWindowPumpEvents(CcoWindow window) {
    NSEvent *event;
    while ((event = [window->window nextEventMatchingMask:NSEventMaskAny
                                                untilDate:[NSDate distantPast]
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES])) {
        CcoCocoaInputEventResult inputResult = ccoInputHandleCocoaEvent(event);
        if (inputResult == CCO_COCOA_INPUT_EVENT_HANDLED)
            continue;
        [NSApp sendEvent:event];
    }
}

CcoWindowFrame ccoWindowGetFrame(CcoWindow window) {
    NSRect rect = [window->window frame];
    return (CcoWindowFrame){rect.origin.x, rect.origin.y, rect.size.width, rect.size.height};
}

CcoWindowContentSize ccoWindowGetContentSize(CcoWindow window) {
    NSRect bounds = [[window->window contentView] bounds];
    CGFloat dpiScale = [window->window backingScaleFactor];

    CGFloat width = bounds.size.width * dpiScale;
    CGFloat height = bounds.size.height * dpiScale;

    return (CcoWindowContentSize){width, height};
}

void *ccoWindowGetHandle(CcoWindow window) { return (__bridge void *)window->window; }
CcoBool ccoWindowWillClose(CcoWindow window) { return window->willClose; }
CcoBool ccoWindowIsFocus(CcoWindow window) { return window->focused; }