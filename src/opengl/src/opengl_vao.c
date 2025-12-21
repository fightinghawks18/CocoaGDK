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

cco_result cco_create_open_gl_vao(cco_opengl_vao *out_vao) {
    cco_opengl_vao vao = malloc(sizeof(cco_opengl_vao_t));
    if (!vao)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenVertexArrays(1, &vao->gl_id);
    *out_vao = vao;
    return CCO_SUCCESS;
}

void cco_destroy_open_gl_vao(cco_opengl_vao vao) {
    if (vao->gl_id != CCO_NULL_GLID) {
        glDeleteVertexArrays(1, &vao->gl_id);
        vao->gl_id = CCO_NULL_GLID;
    }
    free(vao);
}

void cco_use_open_gl_vao(cco_opengl_vao vao) { glBindVertexArray(vao->gl_id); }

void cco_set_open_gl_vao_layout(cco_opengl_vao vao, cco_opengl_vbo vbo, cco_opengl_ebo ebo, const cco_vertex_layout *layout) {
    cco_use_open_gl_vao(vao);
    cco_use_open_gl_vbo(vbo);
    for (u32 i = 0; i < layout->attribute_count; i++) {
        const cco_vertex_attribute attribute = layout->attributes[i];
        glVertexAttribPointer(attribute.location, (i32)attribute.num_components, GL_FLOAT, GL_FALSE,
                              (i32)attribute.stride, (void *)(uintptr_t)attribute.offset);
        glEnableVertexAttribArray(attribute.location);
    }

    cco_use_open_gl_ebo(ebo);

    cco_remove_current_open_gl_vao();
}

void cco_remove_current_open_gl_vao(void) { glBindVertexArray(0); }

u32 cco_get_open_gl_vao_id(cco_opengl_vao vao) { return vao->gl_id; }
