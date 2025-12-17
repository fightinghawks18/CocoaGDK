#include "platform/windowing.h"

#import <Cocoa/Cocoa.h>
#import <CoreFoundation/CoreFoundation.h>
#import <QuartzCore/CAMetalLayer.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_metal.h>

typedef struct CcoWindow_T {
    NSWindow *window;

    CcoBool shouldClose;
    CcoBool focused;

    CcoBool keysPressed[512];
    CcoBool keysPressedLast[512];
    i32 mouseScroll;
    i32 mousePosX;
    i32 mousePosY;
    i32 mouseDeltaX;
    i32 mouseDeltaY;
} CcoWindow_T;

@interface Window : NSObject <NSWindowDelegate>
@property(assign) CcoWindow window;
@end

@implementation Window
- (BOOL)windowShouldClose:(NSWindow *)sender {
    self.window->shouldClose = CCO_YES;
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
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps:YES];
    }
    return CCO_SUCCESS;
}

void ccoWindowingPoll() {
    @autoreleasepool {
        NSEvent *event;
        while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                           untilDate:nil
                                              inMode:NSDefaultRunLoopMode
                                             dequeue:YES])) {
            [NSApp sendEvent:event];
        }
    }
}

void ccoWindowingQuit() { [NSApp stop:nil]; }

CcoResult ccoCreateWindow(const CcoWindowDesc *desc, CcoWindow *outWindow) {
    CcoWindow window = calloc(1, sizeof(CcoWindow_T));
    if (!window)
        return CCO_FAIL_OUT_OF_MEMORY;

    NSWindowStyleMask styleMask = NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskTitled;
    if (desc->flags & CCO_WINDOW_FLAG_RESIZE_BIT)
        styleMask |= NSWindowStyleMaskResizable;
    if (!(desc->flags & CCO_WINDOW_FLAG_DECOR_BIT))
        styleMask |= NSWindowStyleMaskBorderless;

    NSScreen *screen = [NSScreen mainScreen];
    NSRect screenRect = [screen frame];
    NSRect rect = NSMakeRect(desc->x, desc->y, desc->w, desc->h);
    if (desc->x == CCO_WINDOW_POS_CENTER)
        rect.origin.x = (screenRect.size.width - desc->w) / 2.0f;
    if (desc->y == CCO_WINDOW_POS_CENTER)
        rect.origin.y = (screenRect.size.height - desc->h) / 2.0f;
    if (desc->w == CCO_WINDOW_SIZE_FILL) {
        rect.origin.y = 0;
        rect.size.width = screenRect.size.width;
    }
    if (desc->h == CCO_WINDOW_SIZE_FILL) {
        rect.origin.x = 0;
        rect.size.height = screenRect.size.height;
    }

    window->window = [[NSWindow alloc] initWithContentRect:rect
                                                 styleMask:styleMask
                                                   backing:NSBackingStoreBuffered
                                                     defer:NO];
    [window->window setTitle:[NSString stringWithUTF8String:desc->title]];

    Window *windowDelegate = [[Window alloc] init];
    windowDelegate.window = window;
    [window->window setDelegate:windowDelegate];
    [window->window makeKeyAndOrderFront:nil];

    *outWindow = window;
    return CCO_SUCCESS;
}

void ccoSetWindowShouldClose(CcoWindow window, const CcoBool close) { window->shouldClose = close; }

void ccoCloseWindow(CcoWindow window) {
    if (window->window == NULL)
        return;
    [window->window close];
    window->window = NULL;
    CCO_LOG("Closed window");
}

CcoWindowNativeHandle ccoGetNativeWindowHandle(CcoWindow window) {
    return (__bridge CcoWindowNativeHandle)window->window;
}

CcoWindowDimensions ccoGetWindowDimensions(CcoWindow window) {
    NSRect rect = [window->window frame];
    return (CcoWindowDimensions){(i32)rect.origin.x, (i32)rect.origin.y, (i32)rect.size.width, (i32)rect.size.height};
}

CcoWindowFramebufferSize ccoGetWindowFramebufferSize(CcoWindow window) {
    NSRect bounds = [[window->window contentView] bounds];
    CGFloat dpiScale = [window->window backingScaleFactor];

    CGFloat width = bounds.size.width * dpiScale;
    CGFloat height = bounds.size.height * dpiScale;

    return (CcoWindowFramebufferSize){width, height};
}

CcoBool ccoShouldWindowClose(CcoWindow window) { return window->shouldClose; }

VkSurfaceKHR ccoCreateWindowVulkanSurface(VkInstance instance, CcoWindow window) {
    NSView *view = window->window.contentView;
    CAMetalLayer *metalLayer = [CAMetalLayer layer];
    metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    metalLayer.frame = view.bounds;

    view.wantsLayer = YES;
    view.layer = metalLayer;

    VkMetalSurfaceCreateInfoEXT createInfo = {
        .sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT,
        .pNext = NULL,
        .flags = 0,
        .pLayer = metalLayer
    };

    VkSurfaceKHR surface;
    vkCreateMetalSurfaceEXT(instance, &createInfo, NULL, &surface);
    return surface;
}