//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"
#include "graphics/graphics_types.h"

typedef struct CcoOpenGLVbo_T CcoOpenGLVbo_T;
typedef CcoOpenGLVbo_T *CcoOpenGLVbo;

CcoResult ccoCreateOpenGLVbo(CcoOpenGLVbo *outVbo);
void ccoDestroyOpenGLVbo(CcoOpenGLVbo vbo);

void ccoUseOpenGLVbo(CcoOpenGLVbo vbo);
void ccoMapToOpenGLVbo(CcoOpenGLVbo vbo, const CcoBufferMapping *mapping);

u32 ccoGetOpenGLVboID(CcoOpenGLVbo vbo);