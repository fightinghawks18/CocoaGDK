//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"
#include "graphics/graphics_types.h"

#include "opengl_pipeline.h"

typedef struct CcoOpenGLUbo_T CcoOpenGLUbo_T;
typedef CcoOpenGLUbo_T *CcoOpenGLUbo;

typedef enum {
    CCO_OPENGL_UBO_BINDING_BINDING,
    CCO_OPENGL_UBO_BINDING_BLOCK_NAME
} CcoOpenGLUboBindingType;

typedef struct {
    CcoOpenGLUboBindingType type;
    CcoOpenGLPipeline pip;
    union {
        i32 binding;
        const char *name;
    };
} CcoOpenGLUboBinding;

CcoResult ccoCreateOpenGLUbo(CcoOpenGLUbo *outUbo);
void ccoDestroyOpenGLUbo(CcoOpenGLUbo ubo);

void ccoUseOpenGLUbo(const CcoOpenGLUboBinding *binding, CcoOpenGLUbo ubo);
void ccoMapToOpenGLUbo(CcoOpenGLUbo ubo, const CcoBufferMapping *mapping);

u32 ccoGetOpenGLUboID(CcoOpenGLUbo ubo);