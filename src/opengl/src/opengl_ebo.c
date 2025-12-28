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

void cco_use_opengl_ebo(cco_opengl_ebo ebo) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->gl_id); }

void cco_map_to_opengl_ebo(cco_opengl_ebo ebo, const cco_buffer_mapping *mapping) {
    cco_use_opengl_ebo(ebo);
    if (mapping->data) {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (long)mapping->data_offset, (long)mapping->data_size, mapping->data);
    } else {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mapping->data_size, mapping->data, GL_STATIC_DRAW);
    }
}

u32 cco_get_opengl_ebo_id(cco_opengl_ebo ebo) { return ebo->gl_id; }