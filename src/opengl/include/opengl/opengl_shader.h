//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once


#include "core/core_types.h"
#include "graphics/graphics_types.h"

typedef struct cco_opengl_shader_t cco_opengl_shader_t;
typedef cco_opengl_shader_t* cco_opengl_shader;

typedef struct {
    cco_shader_type shader_type;
    const char *shader_path;
} cco_opengl_shader_desc;

cco_result cco_create_opengl_shader(const cco_opengl_shader_desc *description, cco_opengl_shader *out_shader);
void cco_destroy_opengl_shader(cco_opengl_shader shader);

u32 cco_get_opengl_shader_id(cco_opengl_shader shader);