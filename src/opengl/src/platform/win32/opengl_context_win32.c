//
// Created by fightinghawks18 on 12/15/2025.
//

#include "opengl/opengl_context.h"

#include <windows.h>
#include <gl/GL.h>
#include "opengl/GL/wglext.h"

struct cco_opengl_context_t {
    HGLRC ctx;
    HDC hdc;
};

cco_result cco_create_opengl_context(void *window_handle, void *display_handle, cco_opengl_context *out_opengl_context) {
    cco_opengl_context opengl_context = malloc(sizeof(cco_opengl_context_t));
    if (!opengl_context)
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

    HDC hdc = GetDC(window_handle);
    const i32 px_format = ChoosePixelFormat(hdc, &pfd);
    if (!px_format) {
        CCO_LOG("Windows failed to select a pixel format for HDC!");
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }

    if (!SetPixelFormat(hdc, px_format, &pfd)) {
        CCO_LOG("Windows failed to set pixel format for HDC!");
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }

    HGLRC dum_ctx = wglCreateContext(hdc);
    if (!dum_ctx) {
        CCO_LOG("Windows failed to create an OpenGL context!");
        free(opengl_context);
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }
    wglMakeCurrent(hdc, dum_ctx);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wglCreateContextAttribsARB) {
        CCO_LOG("Windows failed to load the wglCreateContextAttribsARB function!");
        free(opengl_context);
        wglDeleteContext(dum_ctx);
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
    wglDeleteContext(dum_ctx);

    opengl_context->ctx = ctx;
    opengl_context->hdc = hdc;

    *out_opengl_context = opengl_context;
    return CCO_SUCCESS;
}

void cco_destroy_opengl_context(cco_opengl_context opengl_context) {
    if (opengl_context->ctx) {
        wglDeleteContext(opengl_context->ctx);
        opengl_context->ctx = NULL;
    }
    opengl_context->hdc = NULL;
    free(opengl_context);
}

void cco_flush_opengl_context_buffer(cco_opengl_context opengl_context) {
    wglSwapLayerBuffers(opengl_context->hdc, WGL_SWAP_MAIN_PLANE);
}

void cco_make_current_opengl_context(cco_opengl_context opengl_context) {
    wglMakeCurrent(opengl_context->hdc, opengl_context->ctx);
}

void cco_remove_current_opengl_context(void) {
    wglMakeCurrent(NULL, NULL);
}