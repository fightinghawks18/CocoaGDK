//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_context.h"
#include <AppKit/AppKit.h>

struct CcoOpenGLContext_T {
    NSOpenGLContext *ctx;
};

CcoResult ccoCreateOpenGLContext(void *windowHandle, void *displayHandle, CcoOpenGLContext *outOpenGLContext) {
    CcoOpenGLContext openGLContext = malloc(sizeof(CcoOpenGLContext_T));
    if (!openGLContext)
        return CCO_FAIL_OUT_OF_MEMORY;
    NSOpenGLPixelFormatAttribute attrs[] = {NSOpenGLPFADoubleBuffer,  NSOpenGLPFADepthSize,          24,
                                            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core, 0};

    NSOpenGLPixelFormat *pxFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    NSOpenGLContext *glContext = [[NSOpenGLContext alloc] initWithFormat:pxFormat shareContext:nil];
    [pxFormat release];

    [glContext setView:[(__bridge NSWindow *)windowHandle contentView]];
    openGLContext->ctx = glContext;

    *outOpenGLContext = openGLContext;
    return CCO_SUCCESS;
}

void ccoDestroyOpenGLContext(CcoOpenGLContext openGLContext) {
    if (openGLContext->ctx != nil) {
        [openGLContext->ctx release];
        openGLContext->ctx = nil;
    }
    free(openGLContext);
}

void ccoFlushOpenGLContextBuffer(CcoOpenGLContext openGLContext) {
    [openGLContext->ctx flushBuffer];
}

void ccoMakeCurrentOpenGLContext(CcoOpenGLContext openGLContext) { [openGLContext->ctx makeCurrentContext]; }
void ccoRemoveCurrentOpenGLContext() { [NSOpenGLContext clearCurrentContext]; }