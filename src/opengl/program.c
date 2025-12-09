//
// Created by fightinghawks18 on 12/8/25.
//

#include "program.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

struct CcoGLProgram_T {
    u32 glId;
};

CcoResult ccoCreateGLProgram(const CcoGLProgramDesc *programDesc, CcoGLProgram *program) {
    CcoGLProgram glProgram = malloc(sizeof(CcoGLProgram));
    glProgram->glId = glCreateProgram();

    glAttachShader(glProgram->glId, ccoGetGLShaderId(programDesc->vertexShader));
    glAttachShader(glProgram->glId, ccoGetGLShaderId(programDesc->pixelShader));
    glLinkProgram(glProgram->glId);

    i32 linkSuccess;
    glGetProgramiv(glProgram->glId, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        char infoLog[512];
        glGetProgramInfoLog(glProgram->glId, 512, NULL, infoLog);
        printf("Failed to link GL program! %s\n", infoLog);
        ccoDestroyGLProgram(glProgram);
        return CCO_FAIL_PIPELINE_CREATE_ERROR;
    }

    glDetachShader(glProgram->glId, ccoGetGLShaderId(programDesc->vertexShader));
    glDetachShader(glProgram->glId, ccoGetGLShaderId(programDesc->pixelShader));

    *program = glProgram;
    return CCO_SUCCESS;
}

void ccoDestroyGLProgram(CcoGLProgram program) {
    if (program->glId) {
        glDeleteProgram(program->glId);
        program->glId = 0;
    }
    free(program);
}

u32 ccoGetGLProgramId(CcoGLProgram program) { return program->glId; }
