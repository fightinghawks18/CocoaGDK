//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once


#include "core/core_types.h"
#include "core/results.h"
#include "core/log.h"
#include "graphics/graphics_types.h"
#include "export.h"

typedef struct cco_opengl_shader_t cco_opengl_shader_t;
typedef cco_opengl_shader_t* cco_opengl_shader;

typedef struct {
    cco_shader_type shader_type;
    const char *shader_path;
} cco_opengl_shader_desc;

CCO_OPENGL_API cco_result cco_create_opengl_shader(const cco_opengl_shader_desc *description, cco_opengl_shader *out_shader);
CCO_OPENGL_API void cco_destroy_opengl_shader(cco_opengl_shader shader);

CCO_OPENGL_API u32 cco_opengl_shader_get_id(cco_opengl_shader shader);