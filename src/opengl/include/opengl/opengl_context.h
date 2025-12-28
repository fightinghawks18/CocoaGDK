//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"
#include "core/log.h"
#include "export.h"

typedef struct cco_opengl_context_t cco_opengl_context_t;
typedef cco_opengl_context_t *cco_opengl_context;

CCO_OPENGL_API cco_result cco_create_opengl_context(void *window_handle, void *display_handle, cco_opengl_context *out_opengl_context);
CCO_OPENGL_API void cco_destroy_opengl_context(cco_opengl_context opengl_context);
CCO_OPENGL_API void cco_opengl_context_flush(cco_opengl_context opengl_context);
CCO_OPENGL_API void cco_opengl_context_make_current(cco_opengl_context opengl_context);