#include "vao.h"
#include <glad/glad.h>
#include <stdlib.h>

struct EvGLVertexArrayObject {
    u32 glId;
};

EvGLVertexArrayObject *evCreateGLVertexArrayObject() {
    EvGLVertexArrayObject *vao = malloc(sizeof(EvGLVertexArrayObject));
    glCreateVertexArrays(1, &vao->glId);
    return vao;
}

void evDestroyGLVertexArrayObject(EvGLVertexArrayObject *vao) {
    if (vao->glId != 0) {
        glDeleteVertexArrays(1, &vao->glId);
        vao->glId = 0;
    }
    free(vao);
}

u32 evGetGLVertexArrayObjectId(const EvGLVertexArrayObject *vao) { return vao->glId; }
