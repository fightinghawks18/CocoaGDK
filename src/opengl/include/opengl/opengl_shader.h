//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "exports.h"
#include "core/core_types.h"
#include "graphics/graphics_types.h"

typedef struct cco_opengl_shader_t cco_opengl_shader_t;
typedef cco_opengl_shader_t* cco_opengl_shader;

typedef struct {
    cco_shader_type shader_type;
    const char *shader_path;
} cco_open_gl_shader_desc;

CCO_OPENGL_API cco_result cco_create_open_gl_shader(const cco_open_gl_shader_desc *description, cco_opengl_shader *out_shader);
CCO_OPENGL_API void cco_destroy_open_gl_shader(cco_opengl_shader shader);

CCO_OPENGL_API u32 cco_get_open_gl_shader_id(cco_opengl_shader shader);