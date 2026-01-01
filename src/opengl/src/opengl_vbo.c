//
// Created by fightinghawks18 on 12/14/25.
//

#include "cocoa_gdk/opengl/opengl_vbo.h"
#include "cocoa_gdk/opengl/opengl_loader.h"
#include "cocoa_gdk/opengl/opengl_types.h"

#include <stdlib.h>

struct cco_opengl_vbo_t {
    u32 gl_id;
    usize size;
};

cco_result cco_create_opengl_vbo(cco_opengl_vbo *out_vbo) {
    cco_opengl_vbo vbo = malloc(sizeof(cco_opengl_vbo_t));
    if (!vbo)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenBuffers(1, &vbo->gl_id);
    vbo->size = 0;
    *out_vbo = vbo;
    return CCO_SUCCESS;
}

void cco_destroy_opengl_vbo(cco_opengl_vbo vbo) {
    if (vbo->gl_id != CCO_NULL_GLID) {
        glDeleteBuffers(1, &vbo->gl_id);
        vbo->gl_id = CCO_NULL_GLID;
    }
    free(vbo);
}

void cco_opengl_vbo_allocate(cco_opengl_vbo vbo, const usize vbo_size) {
    cco_opengl_vbo_use(vbo);
    vbo->size = vbo_size;
    glBufferData(GL_ARRAY_BUFFER, (long)vbo_size, CCO_NIL, GL_STATIC_DRAW);
}

void cco_opengl_vbo_use(cco_opengl_vbo vbo) {
    if (vbo) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo->gl_id);
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, CCO_NULL_GLID);
}

void cco_opengl_vbo_upload(cco_opengl_vbo vbo, const cco_buffer_mapping *mapping) {
    if (vbo->size == 0)
        return;
    cco_opengl_vbo_use(vbo);
    glBufferSubData(GL_ARRAY_BUFFER, (long)mapping->data_offset, (long)mapping->data_size, mapping->data);
}

u32 cco_opengl_vbo_get_id(cco_opengl_vbo vbo) { return vbo->gl_id; }