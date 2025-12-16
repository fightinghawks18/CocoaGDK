//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_vao.h"
#include "opengl/opengl_types.h"
#include <glad/glad.h>
#include <stdlib.h>

struct CcoOpenGLVao_T {
    u32 glID;
};

CcoResult ccoCreateOpenGLVao(CcoOpenGLVao *outVao) {
    CcoOpenGLVao vao = malloc(sizeof(CcoOpenGLVao_T));
    if (!vao)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenVertexArrays(1, &vao->glID);
    *outVao = vao;
    return CCO_SUCCESS;
}

void ccoDestroyOpenGLVao(CcoOpenGLVao vao) {
    if (vao->glID != CCO_NULL_GLID) {
        glDeleteVertexArrays(1, &vao->glID);
        vao->glID = CCO_NULL_GLID;
    }
    free(vao);
}

void ccoUseOpenGLVao(CcoOpenGLVao vao) { glBindVertexArray(vao->glID); }

void ccoSetOpenGLVaoLayout(CcoOpenGLVao vao, CcoOpenGLVbo vbo, CcoOpenGLEbo ebo, const CcoVertexLayout *layout) {
    ccoUseOpenGLVao(vao);
    ccoUseOpenGLVbo(vbo);
    for (u32 i = 0; i < layout->attributeCount; i++) {
        const CcoVertexAttribute attribute = layout->attributes[i];
        glVertexAttribPointer(attribute.location, (i32)attribute.numComponents, GL_FLOAT, GL_FALSE,
                              (i32)attribute.stride, (void *)(uintptr_t)attribute.offset);
        glEnableVertexAttribArray(attribute.location);
    }

    ccoUseOpenGLEbo(ebo);

    ccoRemoveCurrentOpenGLVao();
}

void ccoRemoveCurrentOpenGLVao() { glBindVertexArray(0); }

u32 ccoGetOpenGLVaoID(CcoOpenGLVao vao) { return vao->glID; }
