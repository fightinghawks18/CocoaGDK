//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_ubo.h"
#include "opengl/opengl_types.h"
#include "opengl/opengl_loader.h"

#include <stdlib.h>

struct cco_opengl_ubo_t {
    u32 gl_id;
    usize size;
};

cco_result cco_create_opengl_ubo(cco_opengl_ubo *out_ubo) {
    cco_opengl_ubo ubo = malloc(sizeof(cco_opengl_ubo_t));
    if (!ubo)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenBuffers(1, &ubo->gl_id);
    ubo->size = 0;
    *out_ubo = ubo;
    return CCO_SUCCESS;
}

void cco_destroy_opengl_ubo(cco_opengl_ubo ubo) {
    if (ubo->gl_id != CCO_NULL_GLID) {
        glDeleteBuffers(1, &ubo->gl_id);
        ubo->gl_id = CCO_NULL_GLID;
    }
    free(ubo);
}

void cco_use_opengl_ubo(const cco_opengl_ubo_binding *binding, cco_opengl_ubo ubo) {
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
        const u32 block_index = glGetUniformBlockIndex(cco_get_opengl_pipeline_id(binding->pip), binding->name);

        u32 binding_point = 0;
        glUniformBlockBinding(cco_get_opengl_pipeline_id(binding->pip), block_index, binding_point);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, ubo->gl_id);
        break;
    }
    }
}

void cco_map_to_opengl_ubo(cco_opengl_ubo ubo, const cco_buffer_mapping *mapping) {
    cco_use_opengl_ubo(NULL, ubo);
    if (mapping->data) {
        glBufferSubData(GL_UNIFORM_BUFFER, (long)mapping->data_offset, (long)mapping->data_size, mapping->data);
    } else {
        ubo->size = mapping->data_size;
        glBufferData(GL_UNIFORM_BUFFER, (long)mapping->data_size, mapping->data, GL_DYNAMIC_DRAW);
    }
}

u32 cco_get_opengl_ubo_id(cco_opengl_ubo ubo) { return ubo->gl_id; }