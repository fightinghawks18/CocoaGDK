//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once


#include "core/core_types.h"
#include "core/results.h"
#include "core/log.h"
#include "opengl_shader.h"
#include "export.h"

typedef struct cco_opengl_pipeline_t cco_opengl_pipeline_t;
typedef cco_opengl_pipeline_t* cco_opengl_pipeline;

typedef struct {
    cco_opengl_shader vertex_shader;
    cco_opengl_shader pixel_shader;
} cco_opengl_pipeline_desc;

CCO_OPENGL_API cco_result cco_create_opengl_pipeline(const cco_opengl_pipeline_desc *description, cco_opengl_pipeline *out_pipeline);
CCO_OPENGL_API void cco_destroy_opengl_pipeline(cco_opengl_pipeline pipeline);

CCO_OPENGL_API void cco_opengl_pipeline_use(cco_opengl_pipeline pipeline);
CCO_OPENGL_API void cco_opengl_pipeline_set_uniform_image_slot(cco_opengl_pipeline pipeline, const char *uniform_name, i32 slot);

CCO_OPENGL_API u32 cco_opengl_pipeline_get_id(cco_opengl_pipeline pipeline);