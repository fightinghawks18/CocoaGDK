//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once


#include "core/core_types.h"
#include "graphics/graphics_types.h"

typedef struct cco_opengl_vbo_t cco_opengl_vbo_t;
typedef cco_opengl_vbo_t *cco_opengl_vbo;

cco_result cco_create_opengl_vbo(cco_opengl_vbo *out_vbo);
void cco_destroy_opengl_vbo(cco_opengl_vbo vbo);

void cco_use_opengl_vbo(cco_opengl_vbo vbo);
void cco_map_to_opengl_vbo(cco_opengl_vbo vbo, const cco_buffer_mapping *mapping);

u32 cco_get_opengl_vbo_id(cco_opengl_vbo vbo);