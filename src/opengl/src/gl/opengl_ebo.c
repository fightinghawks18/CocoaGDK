//
// Created by fightinghawks18 on 12/14/25.
//

#include "../../include/opengl/gl/opengl_ebo.h"
#include "../../include/opengl/gl/opengl_types.h"
#include <glad/glad.h>
#include <stdlib.h>

struct CcoOpenGLEbo_T {
    u32 glID;
};

CcoResult ccoCreateOpenGLEbo(CcoOpenGLEbo *outEbo) {
    CcoOpenGLEbo ebo = malloc(sizeof(CcoOpenGLEbo_T));
    if (!ebo)
        return CCO_FAIL_OUT_OF_MEMORY;
    glGenBuffers(1, &ebo->glID);
    *outEbo = ebo;
    return CCO_SUCCESS;
}

void ccoDestroyOpenGLEbo(CcoOpenGLEbo ebo) {
    if (ebo->glID != CCO_NULL_GLID) {
        glDeleteBuffers(1, &ebo->glID);
        ebo->glID = CCO_NULL_GLID;
    }
    free(ebo);
}

void ccoUseOpenGLEbo(CcoOpenGLEbo ebo) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->glID); }

void ccoMapToOpenGLEbo(CcoOpenGLEbo ebo, const CcoBufferMapping *mapping) {
    ccoUseOpenGLEbo(ebo);
    if (mapping->dataOffset > 0) {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (long)mapping->dataOffset, (long)mapping->dataSize, mapping->data);
    } else {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mapping->dataSize, mapping->data, GL_STATIC_DRAW);
    }
}

u32 ccoGetOpenGLEboID(CcoOpenGLEbo ebo) { return ebo->glID; }