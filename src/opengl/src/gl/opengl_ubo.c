//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/gl/opengl_ubo.h"
#include "../../include/opengl/gl/opengl_types.h"
#include <glad/glad.h>
#include <stdlib.h>

struct cco_opengl_ubo_t {
    u32 gl_id;
};

cco_result cco_create_open_gl_ubo(cco_opengl_ubo *out_ubo) {
    cco_opengl_ubo ubo = malloc(sizeof(cco_opengl_ubo_t));
    if (!ubo)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenBuffers(1, &ubo->gl_id);
    *out_ubo = ubo;
    return CCO_SUCCESS;
}

void cco_destroy_open_gl_ubo(cco_opengl_ubo ubo) {
    if (ubo->gl_id != CCO_NULL_GLID) {
        glDeleteBuffers(1, &ubo->gl_id);
        ubo->gl_id = CCO_NULL_GLID;
    }
    free(ubo);
}

void cco_use_open_gl_ubo(const CcoOpenGLUboBinding *binding, cco_opengl_ubo ubo) {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo->gl_id);
    if (!binding)
        return;

    switch (binding->type) {
    case CCO_OPENGL_UBO_BINDING_BINDING: {
        glBindBufferBase(GL_UNIFORM_BUFFER, binding->binding, ubo->gl_id);
        break;
    }
    case CCO_OPENGL_UBO_BINDING_BLOCK_NAME: {
        if (!binding->pip)
            break;
        const u32 block_index = glGetUniformBlockIndex(cco_get_open_gl_pipeline_id(binding->pip), binding->name);

        u32 binding_point = 0;
        glUniformBlockBinding(cco_get_open_gl_pipeline_id(binding->pip), block_index, binding_point);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, ubo->gl_id);
        break;
    }
    }
}

void cco_map_to_open_gl_ubo(cco_opengl_ubo ubo, const cco_buffer_mapping *mapping) {
    cco_use_open_gl_ubo(NULL, ubo);
    if (mapping->data_offset > 0) {
        glBufferSubData(GL_UNIFORM_BUFFER, (long)mapping->data_offset, (long)mapping->data_size, mapping->data);
    } else {
        glBufferData(GL_UNIFORM_BUFFER, (long)mapping->data_size, mapping->data, GL_DYNAMIC_DRAW);
    }
}

u32 cco_get_open_gl_ubo_id(cco_opengl_ubo ubo) { return ubo->gl_id; }