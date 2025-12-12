//
// Created by fightinghawks18 on 12/10/25.
//

#include "ubo.h"

#include <stdlib.h>
#include <glad/glad.h>

#include "../math/matrix4x4.h"

struct CcoGLUniformBufferObject_T {
    u32 glId;
};

CcoGLUniformBufferObject ccoCreateGLUniformBuffer(i32 location) {
    CcoGLUniformBufferObject uniform = malloc(sizeof(CcoGLUniformBufferObject));
    glGenBuffers(1, &uniform->glId);

    ccoUseGLUniformBuffer(uniform);
    glBindBufferBase(GL_UNIFORM_BUFFER, location, uniform->glId);
    ccoUseGLUniformBuffer(NULL);

    return uniform;
}

void ccoDestroyGLUniformBuffer(CcoGLUniformBufferObject uniform) {
    if (uniform->glId != 0) {
        glDeleteBuffers(1, &uniform->glId);
        uniform->glId = 0;
    }
    free(uniform);
}

void ccoUseGLUniformBuffer(CcoGLUniformBufferObject uniform) {
    glBindBuffer(GL_UNIFORM_BUFFER, uniform == NULL ? 0 : uniform->glId);
}

void ccoMapGLUniformBuffer(CcoGLUniformBufferObject uniform, const CcoBufferMapper *mapper) {
    glBindBuffer(GL_UNIFORM_BUFFER, uniform->glId);
    if (mapper->offset > 0) {
        glBufferSubData(GL_UNIFORM_BUFFER, (long)mapper->offset, (long)mapper->size, mapper->data);
    } else {
        glBufferData(GL_UNIFORM_BUFFER, (long)mapper->size, mapper->data, GL_DYNAMIC_DRAW);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

u32 ccoGetGLUniformBufferId(CcoGLUniformBufferObject uniform) {
    return uniform->glId;
}
