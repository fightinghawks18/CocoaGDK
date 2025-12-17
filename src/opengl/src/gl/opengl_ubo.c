//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/gl/opengl_ubo.h"
#include "../../include/opengl/gl/opengl_types.h"
#include <glad/glad.h>
#include <stdlib.h>

struct CcoOpenGLUbo_T {
    u32 glID;
};

CcoResult ccoCreateOpenGLUbo(CcoOpenGLUbo *outUbo) {
    CcoOpenGLUbo ubo = malloc(sizeof(CcoOpenGLUbo_T));
    if (!ubo)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenBuffers(1, &ubo->glID);
    *outUbo = ubo;
    return CCO_SUCCESS;
}

void ccoDestroyOpenGLUbo(CcoOpenGLUbo ubo) {
    if (ubo->glID != CCO_NULL_GLID) {
        glDeleteBuffers(1, &ubo->glID);
        ubo->glID = CCO_NULL_GLID;
    }
    free(ubo);
}

void ccoUseOpenGLUbo(const CcoOpenGLUboBinding *binding, CcoOpenGLUbo ubo) {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo->glID);
    if (!binding)
        return;

    switch (binding->type) {
    case CCO_OPENGL_UBO_BINDING_BINDING: {
        glBindBufferBase(GL_UNIFORM_BUFFER, binding->binding, ubo->glID);
        break;
    }
    case CCO_OPENGL_UBO_BINDING_BLOCK_NAME: {
        if (!binding->pip)
            break;
        const u32 blockIndex = glGetUniformBlockIndex(ccoGetOpenGLPipelineID(binding->pip), binding->name);

        u32 bindingPoint = 0;
        glUniformBlockBinding(ccoGetOpenGLPipelineID(binding->pip), blockIndex, bindingPoint);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo->glID);
        break;
    }
    }
}

void ccoMapToOpenGLUbo(CcoOpenGLUbo ubo, const CcoBufferMapping *mapping) {
    ccoUseOpenGLUbo(NULL, ubo);
    if (mapping->dataOffset > 0) {
        glBufferSubData(GL_UNIFORM_BUFFER, (long)mapping->dataOffset, (long)mapping->dataSize, mapping->data);
    } else {
        glBufferData(GL_UNIFORM_BUFFER, (long)mapping->dataSize, mapping->data, GL_DYNAMIC_DRAW);
    }
}

u32 ccoGetOpenGLUboID(CcoOpenGLUbo ubo) { return ubo->glID; }