//
// Created by fightinghawks18 on 12/8/25.
//

#include "ebo.h"
#include <glad/glad.h>
#include <stdlib.h>

struct EvGLElementBufferObject {
    u32 glId;
};

EvGLElementBufferObject *evCreateGLElementBufferObject() {
    EvGLElementBufferObject *ebo = malloc(sizeof(EvGLElementBufferObject));
    glCreateBuffers(1, &ebo->glId);
    return ebo;
}

void evDestroyGLElementBufferObject(EvGLElementBufferObject *ebo) {
    if (ebo->glId != 0) {
        glDeleteBuffers(1, &ebo->glId);
        ebo->glId = 0;
    }
    free(ebo);
}

void evMapGLElementBufferObject(const EvGLElementBufferObject *ebo, const EvBufferMapper &mapper) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->glId);
    if (mapper.offset > 0) {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, mapper.offset, mapper.size, mapper.data);
    } else {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mapper.size, mapper.data, GL_STATIC_DRAW);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

u32 evGetGLElementBufferObjectId(const EvGLElementBufferObject *ebo) { return ebo->glId; }
