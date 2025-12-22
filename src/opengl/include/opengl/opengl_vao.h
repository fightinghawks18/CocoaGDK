//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once


#include "core/core_types.h"
#include "opengl/opengl_vbo.h"
#include "graphics/graphics_types.h"
#include "opengl_ebo.h"

typedef struct cco_opengl_vao_t cco_opengl_vao_t;
typedef cco_opengl_vao_t *cco_opengl_vao;

cco_result cco_create_opengl_vao(cco_opengl_vao *out_vao);
void cco_destroy_opengl_vao(cco_opengl_vao vao);

void cco_use_opengl_vao(cco_opengl_vao vao);
void cco_set_opengl_vao_layout(cco_opengl_vao vao, cco_opengl_vbo vbo, cco_opengl_ebo ebo, const cco_vertex_layout *layout);
void cco_remove_current_opengl_vao(void);

u32 cco_get_opengl_vao_id(cco_opengl_vao vao);
