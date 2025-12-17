//
// Created by fightinghawks18 on 12/14/25.
//

#include "../../include/opengl/gl/opengl_shader.h"

#include "opengl/gl/opengl_types.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <errno.h>
#else
#include <sys/errno.h>
#endif

CcoResult readFile(const char *filepath, char **source) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        switch (errno) {
        case ENOENT:
            return CCO_FAIL_FILE_NON_EXISTENT;
        case EACCES:
        default:
            return CCO_FAIL_CANT_READ_FILE;
        }
    }

    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return CCO_FAIL_OUT_OF_MEMORY;
    }

    const size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0';

    fclose(file);
    *source = buffer;
    return CCO_SUCCESS;
}

struct CcoOpenGLShader_T {
    u32 glID;
};

CcoResult ccoCreateOpenGLShader(const CcoOpenGLShaderDesc *shaderDesc, CcoOpenGLShader *outShader) {
    CcoOpenGLShader shader = malloc(sizeof(CcoOpenGLShader));
    if (!shader)
        return CCO_FAIL_OUT_OF_MEMORY;

    u32 shaderType = 0;
    switch (shaderDesc->shaderType) {
    case CCO_SHADER_TYPE_VERTEX:
        shaderType = GL_VERTEX_SHADER;
        break;
    case CCO_SHADER_TYPE_PIXEL:
        shaderType = GL_FRAGMENT_SHADER;
        break;
    }

    char *shaderContents = NULL;
    const CcoResult readFileResult = readFile(shaderDesc->shaderPath, &shaderContents);
    if (readFileResult != CCO_SUCCESS) {
        CCO_LOG("Failed to load shader contents for OpenGL!");
        return readFileResult;
    }

    shader->glID = glCreateShader(shaderType);
    glShaderSource(shader->glID, 1, (const char * const *)&shaderContents, NULL);
    glCompileShader(shader->glID);

    free(shaderContents);

    i32 compileSuccess;
    glGetShaderiv(shader->glID, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        char infoLog[512];
        glGetShaderInfoLog(shader->glID, 512, NULL, infoLog);
        CCO_LOG("Failed to compile GL shader from path %s! %s", shaderDesc->shaderPath, infoLog);
        ccoDestroyOpenGLShader(shader);
        return CCO_FAIL_COMPILE_ERROR;
    }

    *outShader = shader;
    return CCO_SUCCESS;
}

void ccoDestroyOpenGLShader(CcoOpenGLShader shader) {
    if (shader->glID != CCO_NULL_GLID) {
        glDeleteShader(shader->glID);
        shader->glID = CCO_NULL_GLID;
    }
    free(shader);
}

u32 ccoGetOpenGLShaderID(CcoOpenGLShader shader) { return shader->glID; }
