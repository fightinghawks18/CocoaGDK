//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_vao.h"
#include "opengl/opengl_loader.h"
#include "opengl/opengl_types.h"

#include <stdlib.h>

struct cco_opengl_vao_t {
    u32 gl_id;
};

cco_result cco_create_opengl_vao(cco_opengl_vao *out_vao) {
    cco_opengl_vao vao = malloc(sizeof(cco_opengl_vao_t));
    if (!vao)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenVertexArrays(1, &vao->gl_id);
    *out_vao = vao;
    return CCO_SUCCESS;
}

void cco_destroy_opengl_vao(cco_opengl_vao vao) {
    if (vao->gl_id != CCO_NULL_GLID) {
        glDeleteVertexArrays(1, &vao->gl_id);
        vao->gl_id = CCO_NULL_GLID;
    }
    free(vao);
}

void cco_opengl_vao_use(cco_opengl_vao vao) {
    if (vao) {
        glBindVertexArray(vao->gl_id);
        return;
    }
    glBindVertexArray(CCO_NULL_GLID);
}

void cco_opengl_vao_set_layout(cco_opengl_vao vao, cco_opengl_vbo vbo, cco_opengl_ebo ebo, const cco_vertex_layout *layout) {
    cco_opengl_vao_use(vao);
    cco_opengl_vbo_use(vbo);
    for (u32 i = 0; i < layout->attribute_count; i++) {
        const cco_vertex_attribute attribute = layout->attributes[i];
        glVertexAttribPointer(attribute.location, (i32)attribute.num_components, GL_FLOAT, GL_FALSE,
                              (i32)attribute.stride, (void *)(uintptr_t)attribute.offset);
        glEnableVertexAttribArray(attribute.location);
    }

    cco_opengl_ebo_use(ebo);
    cco_opengl_vao_use(CCO_NIL);
}

u32 cco_opengl_vao_get_id(cco_opengl_vao vao) { return vao->gl_id; }
