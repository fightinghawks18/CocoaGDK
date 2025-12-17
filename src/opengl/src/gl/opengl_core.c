//
// Created by fightinghawks18 on 12/16/25.
//

#include "../../include/opengl/gl/opengl_core.h"
#include "opengl/gl/opengl_loader.h"
#include <glad/glad.h>

CcoResult ccoInitializeOpenGL() {
    if (!gladLoadGLLoader(ccoGetGLProcAddr)) {
        CCO_LOG("Failed to load glad!");
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }
    return CCO_SUCCESS;
}

void ccoSetOpenGLViewport(CcoViewport viewport) {
    glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
    glDepthRange(viewport.minDepth, viewport.maxDepth);
}

void ccoClearOpenGLBuffers(CcoOpenGLBufferFlags bufferFlags) {
    i32 bufferBits = 0;
    if (bufferFlags & CCO_OPENGL_COLOR_BUFFER_BIT)
        bufferBits |= GL_COLOR_BUFFER_BIT;
    if (bufferFlags & CCO_OPENGL_DEPTH_BUFFER_BIT)
        bufferBits |= GL_DEPTH_BUFFER_BIT;
    glClear(bufferBits);
}

void ccoSetOpenGLClearColor(CcoClearColor clearColor) {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void ccoDrawOpenGLElements(CcoOpenGLPrimitiveType primitiveType, u32 numIndices, CcoOpenGLIndexType indexType) {
    u32 glPrimitive = 0;
    switch (primitiveType) {
    case CCO_OPENGL_PRIMITIVE_TRIANGLES: {
        glPrimitive = GL_TRIANGLES;
        break;
    }
    case CCO_OPENGL_PRIMITIVE_TRIANGLE_LIST: {
        glPrimitive = GL_TRIANGLE_STRIP;
        break;
    }
    }

    u32 glIndexType = 0;
    switch (indexType) {
    case CCO_OPENGL_INDEX_TYPE_U32: {
        glIndexType = GL_UNSIGNED_INT;
        break;
    }
    }
    glDrawElements(glPrimitive, numIndices, glIndexType, NULL);
}
