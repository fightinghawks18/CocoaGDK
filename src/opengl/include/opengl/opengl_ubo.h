//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once


#include "core/core_types.h"
#include "graphics/graphics_types.h"

#include "opengl_pipeline.h"

typedef struct cco_opengl_ubo_t cco_opengl_ubo_t;
typedef cco_opengl_ubo_t *cco_opengl_ubo;

typedef enum {
    CCO_OPENGL_UBO_BINDING_BINDING,
    CCO_OPENGL_UBO_BINDING_BLOCK_NAME
} cco_opengl_ubo_binding_type;

typedef struct {
    cco_opengl_ubo_binding_type type;
    cco_opengl_pipeline pip;
    union {
        i32 binding;
        const char *name;
    };
} CcoOpenGLUboBinding;

cco_result cco_create_opengl_ubo(cco_opengl_ubo *out_ubo);
void cco_destroy_opengl_ubo(cco_opengl_ubo ubo);

void cco_use_opengl_ubo(const CcoOpenGLUboBinding *binding, cco_opengl_ubo ubo);
void cco_map_to_opengl_ubo(cco_opengl_ubo ubo, const cco_buffer_mapping *mapping);

u32 cco_get_opengl_ubo_id(cco_opengl_ubo ubo);