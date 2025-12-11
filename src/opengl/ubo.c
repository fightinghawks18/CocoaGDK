//
// Created by fightinghawks18 on 12/10/25.
//

#include "ubo.h"

#include <stdlib.h>
#include <glad/glad.h>

#include "../math/matrix4x4.h"

struct CcoGLUniformBuffer_T {
    u32 glId;
};

CcoGLUniformBuffer ccoCreateGLUniformBuffer() {
    CcoGLUniformBuffer uniform = malloc(sizeof(CcoGLUniformBuffer));
    glGenBuffers(1, &uniform->glId);
    return uniform;
}

void ccoDestroyGLUniformBuffer(CcoGLUniformBuffer uniform) {
    if (uniform->glId != 0) {
        glDeleteBuffers(1, &uniform->glId);
        uniform->glId = 0;
    }
    free(uniform);
}

void ccoUseGLUniformBuffer(CcoGLUniformBuffer uniform) {
    glBindBuffer(GL_UNIFORM_BUFFER, uniform == NULL ? 0 : uniform->glId);
}

void ccoSetGLUniformBufferUniform(CcoGLUniformBuffer uniform, const CcoUniformType uniformType, const i32 location, const void *value) {
    ccoUseGLUniformBuffer(uniform);
    switch (uniformType) {
    case CCO_UNIFORM_TYPE_FLOAT1: {
        glUniform1f(location, *(f32*)value);
        break;
    }
    case CCO_UNIFORM_TYPE_FLOAT2: {
        const f32* values = (f32*)value;
        glUniform2f(location, values[0], values[1]);
        break;
    }
    case CCO_UNIFORM_TYPE_FLOAT3: {
        const f32* values = (f32*)value;
        glUniform3f(location, values[0], values[1], values[3]);
        break;
    }
    case CCO_UNIFORM_TYPE_FLOAT4: {
        const f32* values = (f32*)value;
        glUniform4f(location, values[0], values[1], values[3], values[4]);
        break;
    }
    case CCO_UNIFORM_TYPE_INT1: {
        glUniform1i(location, *(i32*)value);
        break;
    }
    case CCO_UNIFORM_TYPE_INT2: {
        const i32* values = (i32*)value;
        glUniform2i(location, values[0], values[1]);
        break;
    }
    case CCO_UNIFORM_TYPE_INT3: {
        const i32* values = (i32*)value;
        glUniform3i(location, values[0], values[1], values[2]);
        break;
    }
    case CCO_UNIFORM_TYPE_INT4: {
        const i32* values = (i32*)value;
        glUniform4i(location, values[0], values[1], values[2], values[3]);
        break;
    }
    case CCO_UNIFORM_TYPE_UINT1: {
        glUniform1ui(location, *(i32*)value);
        break;
    }
    case CCO_UNIFORM_TYPE_UINT2: {
        const i32* values = (i32*)value;
        glUniform2i(location, values[0], values[1]);
        break;
    }
    case CCO_UNIFORM_TYPE_UINT3: {
        const i32* values = (i32*)value;
        glUniform3i(location, values[0], values[1], values[2]);
        break;
    }
    case CCO_UNIFORM_TYPE_UINT4: {
        const i32* values = (i32*)value;
        glUniform4i(location, values[0], values[1], values[2], values[3]);
        break;
    }
    case CCO_UNIFORM_TYPE_MATRIX4X4: {
        f32 matrix4x4Ptr[16];
        ccoConvertMatrix4X4ToFloatArray(*(CcoMatrix4X4*)value, matrix4x4Ptr);
        glUniformMatrix4fv(location, 1, GL_TRUE, matrix4x4Ptr);
        break;
    }
    }
    ccoUseGLUniformBuffer(NULL);
}

void ccoMapGLUniformBuffer(CcoGLUniformBuffer uniform, const CcoBufferMapper *mapper) {
    glBindBuffer(GL_UNIFORM_BUFFER, uniform->glId);
    if (mapper->offset > 0) {
        glBufferSubData(GL_UNIFORM_BUFFER, (long)mapper->offset, (long)mapper->size, mapper->data);
    } else {
        glBufferData(GL_UNIFORM_BUFFER, (long)mapper->size, mapper->data, GL_DYNAMIC_DRAW);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

u32 ccoGetGLUniformBufferId(CcoGLUniformBuffer uniform) {
    return uniform->glId;
}
