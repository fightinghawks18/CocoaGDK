//
// Created by fightinghawks18 on 12/14/25.
//

#include "../../include/opengl/gl/opengl_vbo.h"
#include "../../include/opengl/gl/opengl_types.h"
#include <glad/glad.h>
#include <stdlib.h>

struct CcoOpenGLVbo_T {
    u32 glID;
};

CcoResult ccoCreateOpenGLVbo(CcoOpenGLVbo *outVbo) {
    CcoOpenGLVbo vbo = malloc(sizeof(CcoOpenGLVbo_T));
    if (!vbo)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenBuffers(1, &vbo->glID);
    *outVbo = vbo;
    return CCO_SUCCESS;
}

void ccoDestroyOpenGLVbo(CcoOpenGLVbo vbo) {
    if (vbo->glID != CCO_NULL_GLID) {
        glDeleteBuffers(1, &vbo->glID);
        vbo->glID = CCO_NULL_GLID;
    }
    free(vbo);
}

void ccoUseOpenGLVbo(CcoOpenGLVbo vbo) { glBindBuffer(GL_ARRAY_BUFFER, vbo->glID); }

void ccoMapToOpenGLVbo(CcoOpenGLVbo vbo, const CcoBufferMapping *mapping) {
    ccoUseOpenGLVbo(vbo);
    if (mapping->dataOffset > 0) {
        glBufferSubData(GL_ARRAY_BUFFER, (long)mapping->dataOffset, (long)mapping->dataSize, mapping->data);
    } else {
        glBufferData(GL_ARRAY_BUFFER, (long)mapping->dataSize, mapping->data, GL_STATIC_DRAW);
    }
}

u32 ccoGetOpenGLVboID(CcoOpenGLVbo vbo) { return vbo->glID; }