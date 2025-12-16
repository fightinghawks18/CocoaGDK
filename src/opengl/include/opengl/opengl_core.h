//
// Created by fightinghawks18 on 12/16/25.
//

#pragma once

#include "core/core_types.h"
#include "graphics/graphics_types.h"

typedef enum {
    CCO_OPENGL_NO_BUFFER_BIT = 0,
    CCO_OPENGL_COLOR_BUFFER_BIT = 1 << 0,
    CCO_OPENGL_DEPTH_BUFFER_BIT = 1 << 1
} CcoOpenGLBufferFlagBits;
typedef u32 CcoOpenGLBufferFlags;

typedef enum {
    CCO_OPENGL_PRIMITIVE_TRIANGLES,
    CCO_OPENGL_PRIMITIVE_TRIANGLE_LIST
} CcoOpenGLPrimitiveType;

typedef enum {
    CCO_OPENGL_INDEX_TYPE_U32
} CcoOpenGLIndexType;

typedef struct {
    f32 r, g, b, a;
} CcoClearColor;

CcoResult ccoInitializeOpenGL();
void ccoSetOpenGLViewport(CcoViewport viewport);
void ccoClearOpenGLBuffers(CcoOpenGLBufferFlags bufferFlags);
void ccoSetOpenGLClearColor(CcoClearColor clearColor);
void ccoDrawOpenGLElements(CcoOpenGLPrimitiveType primitiveType, u32 numIndices, CcoOpenGLIndexType indexType);