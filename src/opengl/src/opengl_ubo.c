//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_ubo.h"
#include <stdlib.h>
#include <glad/glad.h>
#include "opengl/opengl_types.h"

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

void ccoUseOpenGLUbo(CcoOpenGLUbo ubo) { glBindBuffer(GL_UNIFORM_BUFFER, ubo->glID); }

void ccoMapToOpenGLUbo(CcoOpenGLUbo ubo, const CcoBufferMapping *mapping) {
    ccoUseOpenGLUbo(ubo);
    if (mapping->dataOffset > 0) {
        glBufferSubData(GL_UNIFORM_BUFFER, (long)mapping->dataOffset, (long)mapping->dataSize, mapping->data);
    } else {
        glBufferData(GL_UNIFORM_BUFFER, mapping->dataSize, mapping->data, GL_DYNAMIC_DRAW);
    }
}

u32 ccoGetOpenGLUboID(CcoOpenGLUbo ubo) { return ubo->glID; }