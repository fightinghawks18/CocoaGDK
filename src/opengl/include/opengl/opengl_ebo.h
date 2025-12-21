//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "exports.h"
#include "core/core_types.h"
#include "graphics/graphics_types.h"

typedef struct cco_opengl_ebo_t cco_opengl_ebo_t;
typedef cco_opengl_ebo_t *cco_opengl_ebo;

CCO_OPENGL_API cco_result cco_create_open_gl_ebo(cco_opengl_ebo *out_ebo);
CCO_OPENGL_API void cco_destroy_open_gl_ebo(cco_opengl_ebo ebo);

CCO_OPENGL_API void cco_use_open_gl_ebo(cco_opengl_ebo ebo);
CCO_OPENGL_API void cco_map_to_open_gl_ebo(cco_opengl_ebo ebo, const cco_buffer_mapping *mapping);

CCO_OPENGL_API u32 cco_get_open_gl_ebo_id(cco_opengl_ebo ebo);