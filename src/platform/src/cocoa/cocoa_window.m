//
// Created by fightinghawks18 on 12/19/25.
//

#include "platform/window.h"
#import <Cocoa/Cocoa.h>
#import <CoreFoundation/CoreFoundation.h>
#include "platform/cocoa/cocoa_input.h"
#include "platform/input.h"

struct cco_window_t {
    NSWindow *window;
    cco_bool will_close;
    cco_bool focused;
};

@interface CocoaWindow : NSObject <NSWindowDelegate>
@property(nonatomic, assign) cco_window window;
@end

@implementation CocoaWindow
- (BOOL)windowShouldClose:(NSWindow *)sender {
    self.window->will_close = CCO_YES;
    return NO;
}
- (void)windowDidBecomeKey:(NSNotification *)notification {
    self.window->focused = CCO_YES;
    cco_input_give_window_focus(self.window);
}
- (void)windowDidResignKey:(NSNotification *)notification {
    self.window->focused = CCO_NO;
    cco_input_give_window_focus(CCO_NIL);
}

@end

cco_result cco_windowing_init() {
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
    return CCO_SUCCESS;
}

void cco_windowing_quit() {}

cco_result cco_create_window(i32 x, i32 y, i32 width, i32 height, const char *title, cco_window *out_window) {
    cco_window window = malloc(sizeof(cco_window_t));
    if (!window)
        return CCO_FAIL_OUT_OF_MEMORY;

    NSRect rect = NSMakeRect(x, y, width, height);
    NSWindowStyleMask style_mask = NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled |
                                  NSWindowStyleMaskMiniaturizable;
    NSWindow *win = [[NSWindow alloc] initWithContentRect:rect
                                                styleMask:style_mask
                                                  backing:NSBackingStoreBuffered
                                                    defer:YES];

    window->window = win;
    window->will_close = false;
    cco_window_rename(window, title);

    CocoaWindow *delg = [[CocoaWindow alloc] init];
    delg.window = window;

    [window->window setDelegate:delg];
    [window->window makeKeyAndOrderFront:nil];

    *out_window = window;
    return CCO_SUCCESS;
}

void cco_destroy_window(cco_window window) {
    if (window->window) {
        [[window->window delegate] release];
        [window->window release];
        window->window = nil;
    }
    free(window);
}

void cco_window_move(cco_window window, i32 x, i32 y) {
    NSPoint point = NSMakePoint(x, y);
    [window->window setFrameOrigin:point];
}
void cco_window_resize(cco_window window, i32 width, i32 height) {
    NSRect window_rect = [window->window frame];
    NSRect rect = NSMakeRect(window_rect.origin.x, window_rect.origin.y, width, height);
    [window->window setFrame:rect display:YES];
}

void cco_window_rename(cco_window window, const char *title) {
    @autoreleasepool {
        NSString *str = [NSString stringWithUTF8String:title];
        [window->window setTitle:str];
    }
}

void cco_window_request_close(cco_window window) { window->will_close = CCO_YES; }

void cco_window_pump_events(cco_window window) {
    NSEvent *event;
    while ((event = [window->window nextEventMatchingMask:NSEventMaskAny
                                                untilDate:[NSDate distantPast]
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES])) {
        cco_cocoa_input_event_result input_result = cco_input_handle_cocoa_event(event);
        if (input_result == CCO_COCOA_INPUT_EVENT_HANDLED)
            continue;
        [NSApp sendEvent:event];
    }
}

cco_window_frame cco_window_get_frame(cco_window window) {
    NSRect rect = [window->window frame];
    return (cco_window_frame){rect.origin.x, rect.origin.y, rect.size.width, rect.size.height};
}

cco_window_content_size cco_window_get_content_size(cco_window window) {
    NSRect bounds = [[window->window contentView] bounds];
    CGFloat dpi_scale = [window->window backingScaleFactor];

    CGFloat width = bounds.size.width * dpi_scale;
    CGFloat height = bounds.size.height * dpi_scale;

    return (cco_window_content_size){width, height};
}

void *cco_window_get_handle(cco_window window) { return (__bridge void *)window->window; }
cco_bool cco_window_will_close(cco_window window) { return window->will_close; }
cco_bool cco_window_is_focus(cco_window window) { return window->focused; }
