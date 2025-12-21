//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_pipeline.h"
#include "opengl/opengl_loader.h"
#include "opengl/opengl_shader.h"

#include <stdlib.h>

struct cco_opengl_pipeline_t {
    u32 gl_id;
};

cco_result cco_create_open_gl_pipeline(const cco_open_gl_pipeline_desc *description, cco_opengl_pipeline *out_pipeline) {
    cco_opengl_pipeline pipeline = malloc(sizeof(cco_opengl_pipeline));
    pipeline->gl_id = glCreateProgram();

    glAttachShader(pipeline->gl_id, cco_get_open_gl_shader_id(description->vertex_shader));
    glAttachShader(pipeline->gl_id, cco_get_open_gl_shader_id(description->pixel_shader));
    glLinkProgram(pipeline->gl_id);

    i32 link_success;
    glGetProgramiv(pipeline->gl_id, GL_LINK_STATUS, &link_success);
    if (!link_success) {
        char info_log[512];
        glGetProgramInfoLog(pipeline->gl_id, 512, NULL, info_log);
        CCO_LOG("Failed to link OpenGL pipeline! %s", info_log);
        cco_destroy_open_gl_pipeline(pipeline);
        return CCO_FAIL_PIPELINE_CREATE_ERROR;
    }

    glDetachShader(pipeline->gl_id, cco_get_open_gl_shader_id(description->vertex_shader));
    glDetachShader(pipeline->gl_id, cco_get_open_gl_shader_id(description->pixel_shader));

    *out_pipeline = pipeline;
    return CCO_SUCCESS;
}

void cco_destroy_open_gl_pipeline(cco_opengl_pipeline pipeline) {
    if (pipeline->gl_id) {
        glDeleteProgram(pipeline->gl_id);
        pipeline->gl_id = 0;
    }
    free(pipeline);
}

void cco_use_open_gl_pipeline(cco_opengl_pipeline pipeline) { glUseProgram(pipeline->gl_id); }

u32 cco_get_open_gl_pipeline_id(cco_opengl_pipeline pipeline) { return pipeline->gl_id; }