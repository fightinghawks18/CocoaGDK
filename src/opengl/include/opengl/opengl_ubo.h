//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"
#include "graphics/graphics_types.h"

typedef struct CcoOpenGLUbo_T CcoOpenGLUbo_T;
typedef CcoOpenGLUbo_T *CcoOpenGLUbo;

CcoResult ccoCreateOpenGLUbo(CcoOpenGLUbo *outUbo);
void ccoDestroyOpenGLUbo(CcoOpenGLUbo ubo);

void ccoUseOpenGLUbo(CcoOpenGLUbo ubo);
void ccoMapToOpenGLUbo(CcoOpenGLUbo ubo, const CcoBufferMapping *mapping);

u32 ccoGetOpenGLUboID(CcoOpenGLUbo ubo);