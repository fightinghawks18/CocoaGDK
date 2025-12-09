#include "vao.h"
#include <glad/glad.h>
#include <stdlib.h>

struct CcoGLVertexArrayObject {
    u32 glId;
};

CcoGLVertexArrayObject *ccoCreateGLVertexArrayObject() {
    CcoGLVertexArrayObject *vao = malloc(sizeof(CcoGLVertexArrayObject));
    glCreateVertexArrays(1, &vao->glId);
    return vao;
}

void ccoDestroyGLVertexArrayObject(CcoGLVertexArrayObject *vao) {
    if (vao->glId != 0) {
        glDeleteVertexArrays(1, &vao->glId);
        vao->glId = 0;
    }
    free(vao);
}

u32 ccoGetGLVertexArrayObjectId(const CcoGLVertexArrayObject *vao) { return vao->glId; }
