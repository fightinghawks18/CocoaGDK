//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_ubo.h"
#include "opengl/opengl_loader.h"
#include "opengl/opengl_types.h"

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

void cco_opengl_ubo_allocate(cco_opengl_ubo ubo, usize ubo_size) {
    ubo->size = ubo_size;
    cco_opengl_ubo_use(NULL, ubo);
    glBufferData(GL_UNIFORM_BUFFER, (long)ubo_size, CCO_NIL, GL_DYNAMIC_DRAW);
}

void cco_opengl_ubo_use(const cco_opengl_ubo_binding *binding, cco_opengl_ubo ubo) {
    if (!ubo) {
        glBindBuffer(GL_UNIFORM_BUFFER, CCO_NULL_GLID);
        return;
    }

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
        const u32 block_index = glGetUniformBlockIndex(cco_opengl_pipeline_get_id(binding->pip), binding->name);

        u32 binding_point = 0;
        glUniformBlockBinding(cco_opengl_pipeline_get_id(binding->pip), block_index, binding_point);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, ubo->gl_id);
        break;
    }
    }
}

void cco_opengl_ubo_upload(cco_opengl_ubo ubo, const cco_buffer_mapping *mapping) {
    if (ubo->size == 0)
        return;
    cco_opengl_ubo_use(NULL, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, (long)mapping->data_offset, (long)mapping->data_size, mapping->data);
}

u32 cco_opengl_ubo_get_id(cco_opengl_ubo ubo) { return ubo->gl_id; }