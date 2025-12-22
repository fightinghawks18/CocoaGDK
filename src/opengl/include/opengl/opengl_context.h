//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"

typedef struct cco_opengl_context_t cco_opengl_context_t;
typedef cco_opengl_context_t *cco_opengl_context;

cco_result cco_create_opengl_context(void *window_handle, void *display_handle, cco_opengl_context *out_opengl_context);
void cco_destroy_opengl_context(cco_opengl_context opengl_context);
void cco_flush_opengl_context_buffer(cco_opengl_context opengl_context);
void cco_make_current_opengl_context(cco_opengl_context opengl_context);
void cco_remove_current_opengl_context(void);