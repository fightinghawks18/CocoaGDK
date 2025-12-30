//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once


#include "core/core_types.h"
#include "core/results.h"
#include "core/log.h"
#include "graphics/graphics_types.h"
#include "export.h"

typedef struct cco_opengl_vbo_t cco_opengl_vbo_t;
typedef cco_opengl_vbo_t *cco_opengl_vbo;

CCO_OPENGL_API cco_result cco_create_opengl_vbo(cco_opengl_vbo *out_vbo);
CCO_OPENGL_API void cco_destroy_opengl_vbo(cco_opengl_vbo vbo);

CCO_OPENGL_API void cco_opengl_vbo_allocate(cco_opengl_vbo vbo, usize vbo_size);

CCO_OPENGL_API void cco_opengl_vbo_use(cco_opengl_vbo vbo);
CCO_OPENGL_API void cco_opengl_vbo_upload(cco_opengl_vbo vbo, const cco_buffer_mapping *mapping);

CCO_OPENGL_API u32 cco_opengl_vbo_get_id(cco_opengl_vbo vbo);