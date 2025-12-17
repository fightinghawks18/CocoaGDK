//
// Created by fightinghawks18 on 12/15/2025.
//

#include "opengl/gl/opengl_context.h"

#include <windows.h>
#include <gl/GL.h>
#include <gl/wglext.h>

struct CcoOpenGLContext_T {
    HGLRC ctx;
    HDC hdc;
};

CcoResult ccoCreateOpenGLContext(void *windowHandle, void *displayHandle, CcoOpenGLContext *outOpenGLContext) {
    CcoOpenGLContext openGLContext = malloc(sizeof(CcoOpenGLContext_T));
    if (!openGLContext)
        return CCO_FAIL_OUT_OF_MEMORY;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0, 0, 0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0, 0, 0, 0
    };

    HDC hdc = GetDC(windowHandle);
    const i32 pxFormat = ChoosePixelFormat(hdc, &pfd);
    if (!pxFormat) {
        CCO_LOG("Windows failed to select a pixel format for HDC!");
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }

    if (!SetPixelFormat(hdc, pxFormat, &pfd)) {
        CCO_LOG("Windows failed to set pixel format for HDC!");
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }

    HGLRC dumCtx = wglCreateContext(hdc);
    if (!dumCtx) {
        CCO_LOG("Windows failed to create an OpenGL context!");
        free(openGLContext);
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }
    wglMakeCurrent(hdc, dumCtx);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wglCreateContextAttribsARB) {
        CCO_LOG("Windows failed to load the wglCreateContextAttribsARB function!");
        free(openGLContext);
        wglDeleteContext(dumCtx);
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    HGLRC ctx = wglCreateContextAttribsARB(hdc, 0, attribs);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dumCtx);

    openGLContext->ctx = ctx;
    openGLContext->hdc = hdc;

    *outOpenGLContext = openGLContext;
    return CCO_SUCCESS;
}

void ccoDestroyOpenGLContext(CcoOpenGLContext openGLContext) {
    if (openGLContext->ctx) {
        wglDeleteContext(openGLContext->ctx);
        openGLContext->ctx = NULL;
    }
    openGLContext->hdc = NULL;
    free(openGLContext);
}

void ccoFlushOpenGLContextBuffer(CcoOpenGLContext openGLContext) {
    wglSwapLayerBuffers(openGLContext->hdc, WGL_SWAP_MAIN_PLANE);
}

void ccoMakeCurrentOpenGLContext(CcoOpenGLContext openGLContext) {
    wglMakeCurrent(openGLContext->hdc, openGLContext->ctx);
}

void ccoRemoveCurrentOpenGLContext() {
    wglMakeCurrent(NULL, NULL);
}