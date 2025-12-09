#include "vao.h"
#include <glad/glad.h>
#include <stdlib.h>

struct CcoGLVertexArrayObject_T {
    u32 glId;
};

CcoGLVertexArrayObject ccoCreateGLVertexArrayObject(const CcoGLVertexArrayObjectDesc *vaoDesc) {
    CcoGLVertexArrayObject vao = malloc(sizeof(CcoGLVertexArrayObject));
    glGenVertexArrays(1, &vao->glId);

    glBindVertexArray(vao->glId);
    for (u32 i = 0; i < vaoDesc->layoutCount; i++) {
        const CcoGLVertexLayoutDesc layout = vaoDesc->layouts[i];
        glVertexAttribPointer(layout.location, layout.size, layout.type, GL_FALSE, layout.stride, (void*)layout.offset);
        glEnableVertexAttribArray(i);
    }
    glBindVertexArray(0);

    return vao;
}

void ccoDestroyGLVertexArrayObject(CcoGLVertexArrayObject vao) {
    if (vao->glId != 0) {
        glDeleteVertexArrays(1, &vao->glId);
        vao->glId = 0;
    }
    free(vao);
}

u32 ccoGetGLVertexArrayObjectId(CcoGLVertexArrayObject vao) { return vao->glId; }
