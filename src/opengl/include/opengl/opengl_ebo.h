//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once


#include "core/core_types.h"
#include "graphics/graphics_types.h"

typedef struct cco_opengl_ebo_t cco_opengl_ebo_t;
typedef cco_opengl_ebo_t *cco_opengl_ebo;

cco_result cco_create_opengl_ebo(cco_opengl_ebo *out_ebo);
void cco_destroy_opengl_ebo(cco_opengl_ebo ebo);

void cco_use_opengl_ebo(cco_opengl_ebo ebo);
void cco_map_to_opengl_ebo(cco_opengl_ebo ebo, const cco_buffer_mapping *mapping);

u32 cco_get_opengl_ebo_id(cco_opengl_ebo ebo);