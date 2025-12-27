//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once


#include "core/core_types.h"
#include "core/log.h"
#include "opengl_vbo.h"
#include "graphics/graphics_types.h"
#include "opengl_ebo.h"
#include "export.h"

typedef struct cco_opengl_vao_t cco_opengl_vao_t;
typedef cco_opengl_vao_t *cco_opengl_vao;

CCO_OPENGL_API cco_result cco_create_opengl_vao(cco_opengl_vao *out_vao);
CCO_OPENGL_API void cco_destroy_opengl_vao(cco_opengl_vao vao);

CCO_OPENGL_API void cco_use_opengl_vao(cco_opengl_vao vao);
CCO_OPENGL_API void cco_set_opengl_vao_layout(cco_opengl_vao vao, cco_opengl_vbo vbo, cco_opengl_ebo ebo, const cco_vertex_layout *layout);
CCO_OPENGL_API void cco_remove_current_opengl_vao(void);

CCO_OPENGL_API u32 cco_get_opengl_vao_id(cco_opengl_vao vao);
