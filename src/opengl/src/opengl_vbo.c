//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_vbo.h"
#include "opengl/opengl_types.h"
#include <glad/glad.h>
#include <stdlib.h>

struct cco_opengl_vbo_t {
    u32 gl_id;
};

cco_result cco_create_open_gl_vbo(cco_opengl_vbo *out_vbo) {
    cco_opengl_vbo vbo = malloc(sizeof(cco_opengl_vbo_t));
    if (!vbo)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenBuffers(1, &vbo->gl_id);
    *out_vbo = vbo;
    return CCO_SUCCESS;
}

void cco_destroy_open_gl_vbo(cco_opengl_vbo vbo) {
    if (vbo->gl_id != CCO_NULL_GLID) {
        glDeleteBuffers(1, &vbo->gl_id);
        vbo->gl_id = CCO_NULL_GLID;
    }
    free(vbo);
}

void cco_use_open_gl_vbo(cco_opengl_vbo vbo) { glBindBuffer(GL_ARRAY_BUFFER, vbo->gl_id); }

void cco_map_to_open_gl_vbo(cco_opengl_vbo vbo, const cco_buffer_mapping *mapping) {
    cco_use_open_gl_vbo(vbo);
    if (mapping->data_offset > 0) {
        glBufferSubData(GL_ARRAY_BUFFER, (long)mapping->data_offset, (long)mapping->data_size, mapping->data);
    } else {
        glBufferData(GL_ARRAY_BUFFER, (long)mapping->data_size, mapping->data, GL_STATIC_DRAW);
    }
}

u32 cco_get_open_gl_vbo_id(cco_opengl_vbo vbo) { return vbo->gl_id; }