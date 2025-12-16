//
// Created by fightinghawks18 on 12/15/2025.
//

#include "opengl/opengl_context.h"

#include <windows.h>

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

    openGLContext->ctx = wglCreateContext(hdc);
    if (!openGLContext->ctx) {
        CCO_LOG("Windows failed to create an OpenGL context!");
        free(openGLContext);
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }
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