//
// Created by fightinghawks18 on 12/15/25.
//

#pragma once

#include "core/core_types.h"
#include <glad/glad.h>

void cco_check_open_gl_error(const char *location) {
    u32 gerr;
    while ((gerr = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error occurred at %s!\n", location);
        switch (gerr) {
        case GL_INVALID_ENUM:
            printf("GL_INVALID_ENUM\n");
            break;
        case GL_INVALID_VALUE:
            printf("GL_INVALID_VALUE\n");
            break;
        case GL_INVALID_OPERATION:
            printf("GL_INVALID_OPERATION\n");
            break;
        case GL_OUT_OF_MEMORY:
            printf("GL_OUT_OF_MEMORY\n");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            printf("GL_INVALID_FRAMEBUFFER_OPERATION\n");
            break;
        default:
            printf("%d\n", gerr);
            break;
        }
    }
}