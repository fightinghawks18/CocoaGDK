//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"
#include "graphics/graphics_types.h"

typedef struct CcoOpenGLEbo_T CcoOpenGLEbo_T;
typedef CcoOpenGLEbo_T *CcoOpenGLEbo;

CcoResult ccoCreateOpenGLEbo(CcoOpenGLEbo *outEbo);
void ccoDestroyOpenGLEbo(CcoOpenGLEbo ebo);

void ccoUseOpenGLEbo(CcoOpenGLEbo ebo);
void ccoMapToOpenGLEbo(CcoOpenGLEbo ebo, const CcoBufferMapping *mapping);

u32 ccoGetOpenGLEboID(CcoOpenGLEbo ebo);