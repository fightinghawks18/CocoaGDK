//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once


#include "core/core_types.h"
#include "core/log.h"
#include "graphics/graphics_types.h"
#include "export.h"
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
} cco_opengl_ubo_binding;

CCO_OPENGL_API cco_result cco_create_opengl_ubo(cco_opengl_ubo *out_ubo);
CCO_OPENGL_API void cco_destroy_opengl_ubo(cco_opengl_ubo ubo);

CCO_OPENGL_API void cco_opengl_ubo_allocate(cco_opengl_ubo ubo, usize ubo_size);

CCO_OPENGL_API void cco_opengl_ubo_use(const cco_opengl_ubo_binding *binding, cco_opengl_ubo ubo);
CCO_OPENGL_API void cco_opengl_ubo_upload(cco_opengl_ubo ubo, const cco_buffer_mapping *mapping);

CCO_OPENGL_API u32 cco_opengl_ubo_get_id(cco_opengl_ubo ubo);