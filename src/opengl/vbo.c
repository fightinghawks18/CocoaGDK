//
// Created by fightinghawks18 on 12/8/25.
//

#include "vbo.h"
#include <glad/glad.h>
#include <stdlib.h>

struct CcoGLVertexBufferObject {
    u32 glId;
};

CcoGLVertexBufferObject *ccoCreateGLVertexBufferObject() {
    CcoGLVertexBufferObject *vbo = malloc(sizeof(CcoGLVertexBufferObject));
    glCreateBuffers(1, &vbo->glId);
    return vbo;
}

void ccoDestroyGLVertexBufferObject(CcoGLVertexBufferObject *vbo) {
    if (vbo->glId != 0) {
        glDeleteBuffers(1, &vbo->glId);
        vbo->glId = 0;
    }
    free(vbo);
}

void ccoMapGLVertexBufferObject(const CcoGLVertexBufferObject *vbo, const CcoBufferMapper &mapper) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo->glId);
    if (mapper.offset > 0) {
        glBufferSubData(GL_ARRAY_BUFFER, mapper.offset, mapper.size, mapper.data);
    } else {
        glBufferData(GL_ARRAY_BUFFER, mapper.size, mapper.data, GL_DYNAMIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

u32 ccoGetGLVertexBufferObjectId(const CcoGLVertexBufferObject *vbo) { return vbo->glId; }
