//
// Created by fightinghawks18 on 12/8/25.
//

#include "vbo.h"
#include <glad/glad.h>
#include <stdlib.h>

struct EvGLVertexBufferObject {
    u32 glId;
};

EvGLVertexBufferObject *evCreateGLVertexBufferObject() {
    EvGLVertexBufferObject *vbo = malloc(sizeof(EvGLVertexBufferObject));
    glCreateBuffers(1, &vbo->glId);
    return vbo;
}

void evDestroyGLVertexBufferObject(EvGLVertexBufferObject *vbo) {
    if (vbo->glId != 0) {
        glDeleteBuffers(1, &vbo->glId);
        vbo->glId = 0;
    }
    free(vbo);
}

void evMapGLVertexBufferObject(const EvGLVertexBufferObject *vbo, const EvBufferMapper &mapper) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo->glId);
    if (mapper.offset > 0) {
        glBufferSubData(GL_ARRAY_BUFFER, mapper.offset, mapper.size, mapper.data);
    } else {
        glBufferData(GL_ARRAY_BUFFER, mapper.size, mapper.data, GL_DYNAMIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

u32 evGetGLVertexBufferObjectId(const EvGLVertexBufferObject *vbo) { return vbo->glId; }
