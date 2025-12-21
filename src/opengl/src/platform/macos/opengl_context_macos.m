//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_context.h"
#include <AppKit/AppKit.h>

struct cco_opengl_context_t {
    NSOpenGLContext *ctx;
};

cco_result cco_create_open_gl_context(void *window_handle, void *display_handle, cco_opengl_context *out_open_gl_context) {
    cco_opengl_context open_gl_context = malloc(sizeof(cco_opengl_context_t));
    if (!open_gl_context)
        return CCO_FAIL_OUT_OF_MEMORY;
    NSOpenGLPixelFormatAttribute attrs[] = {NSOpenGLPFADoubleBuffer,  NSOpenGLPFADepthSize,          24,
                                            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core, 0};

    NSWindow *window = (__bridge NSWindow *)window_handle;
    NSRect frame = [window frame];

    NSOpenGLPixelFormat *px_format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    NSOpenGLView *glView = [[NSOpenGLView alloc] initWithFrame:frame pixelFormat:px_format];
    [window setContentView:glView];
    [px_format release];

    NSOpenGLContext *glContext = [glView openGLContext];
    open_gl_context->ctx = glContext;
    [glView setWantsBestResolutionOpenGLSurface:YES];
    [glView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

    *out_open_gl_context = open_gl_context;
    return CCO_SUCCESS;
}

void cco_destroy_open_gl_context(cco_opengl_context open_gl_context) {
    if (open_gl_context->ctx != nil) {
        [open_gl_context->ctx release];
        open_gl_context->ctx = nil;
    }
    free(open_gl_context);
}

void cco_flush_open_gl_context_buffer(cco_opengl_context open_gl_context) {
    [open_gl_context->ctx flushBuffer];
}

void cco_make_current_open_gl_context(cco_opengl_context open_gl_context) { [open_gl_context->ctx makeCurrentContext]; }
void cco_remove_current_open_gl_context(void) { [NSOpenGLContext clearCurrentContext]; }