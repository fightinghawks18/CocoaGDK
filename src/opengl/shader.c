//
// Created by fightinghawks18 on 12/8/25.
//

#include "shader.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>

EvResult read_file(const char *filepath, char **source) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        switch (errno) {
        case ENOENT:
            return EV_FAIL_FILE_NON_EXISTENT;
        case EACCES:
        default:
            return EV_FAIL_CANT_READ_FILE;
        }
    }

    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return EV_FAIL_OUT_OF_MEMORY;
    }

    const size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0';

    fclose(file);
    *source = buffer;
    return EV_SUCCESS;
}

struct EvGLShader {
    u32 glId;
};

EvResult evCreateGLShader(const EvGLShaderDesc &shaderDesc, EvGLShader **shader) {
    EvGLShader *glShader = malloc(sizeof(EvGLShader));

    u32 shaderType = 0;
    switch (shaderDesc.shaderType) {
    case EV_SHADER_TYPE_VERTEX:
        shaderType = GL_VERTEX_SHADER;
        break;
    case EV_SHADER_TYPE_PIXEL:
        shaderType = GL_FRAGMENT_SHADER;
        break;
    }

    char *shaderContents = NULL;
    const EvResult readFileResult = read_file(shaderDesc.shaderPath, &shaderContents);
    if (readFileResult != EV_SUCCESS) {
        printf("Failed to load shader contents for GL!\n");
        return readFileResult;
    }

    glShader->glId = glCreateShader(shaderType);
    glShaderSource(glShader->glId, 1, &shaderContents, NULL);
    glCompileShader(glShader->glId);

    free(shaderContents);

    i32 compileSuccess;
    glGetShaderiv(glShader->glId, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        char infoLog[512];
        glGetShaderInfoLog(glShader->glId, 512, NULL, infoLog);
        printf("Failed to compile GL shader from path %s! %s\n", shaderDesc.shaderPath, infoLog);
        evDestroyGLShader(glShader);
        return EV_FAIL_COMPILE_ERROR;
    }

    *shader = glShader;
    return EV_SUCCESS;
}

void evDestroyGLShader(EvGLShader *shader) {
    if (shader->glId != 0) {
        glDeleteShader(shader->glId);
        shader->glId = 0;
    }
    free(shader);
}

u32 evGetGLShaderId(const EvGLShader *shader) { return shader->glId; }
