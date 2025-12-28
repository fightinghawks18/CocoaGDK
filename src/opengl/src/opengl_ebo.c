//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_ebo.h"
#include "opengl/opengl_loader.h"
#include "opengl/opengl_types.h"

#include <stdlib.h>

struct cco_opengl_ebo_t {
    u32 gl_id;
};

cco_result cco_create_opengl_ebo(cco_opengl_ebo *out_ebo) {
    cco_opengl_ebo ebo = malloc(sizeof(cco_opengl_ebo_t));
    if (!ebo)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenBuffers(1, &ebo->gl_id);
    *out_ebo = ebo;
    return CCO_SUCCESS;
}

void cco_destroy_opengl_ebo(cco_opengl_ebo ebo) {
    if (ebo->gl_id != CCO_NULL_GLID) {
        glDeleteBuffers(1, &ebo->gl_id);
        ebo->gl_id = CCO_NULL_GLID;
    }
    free(ebo);
}

void cco_opengl_ebo_allocate(cco_opengl_ebo ebo, const usize ebo_size) {
    cco_opengl_ebo_use(ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long)ebo_size, CCO_NIL, GL_STATIC_DRAW);
}

void cco_opengl_ebo_use(cco_opengl_ebo ebo) {
    if (ebo) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->gl_id);
        return;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CCO_NULL_GLID);
}

void cco_opengl_ebo_upload(cco_opengl_ebo ebo, const cco_buffer_mapping *mapping) {
    cco_opengl_ebo_use(ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (long)mapping->data_offset, (long)mapping->data_size, mapping->data);
}

u32 cco_opengl_ebo_get_id(cco_opengl_ebo ebo) { return ebo->gl_id; }