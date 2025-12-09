//
// Created by fightinghawks18 on 12/8/25.
//

#include "program.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

struct EvGLProgram {
    u32 glId;
};

EvResult evCreateGLProgram(const EvGLProgramDesc &programDesc, EvGLProgram **program) {
    EvGLProgram *glProgram = malloc(sizeof(EvGLProgram));
    glProgram->glId = glCreateProgram();

    glAttachShader(glProgram->glId, evGetGLShaderId(programDesc.vertexShader));
    glAttachShader(glProgram->glId, evGetGLShaderId(programDesc.pixelShader));
    glLinkProgram(glProgram->glId);

    i32 linkSuccess;
    glGetProgramiv(glProgram->glId, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        char infoLog[512];
        glGetProgramInfoLog(glProgram->glId, 512, NULL, infoLog);
        printf("Failed to link GL program! %s\n", infoLog);
        evDestroyGLProgram(glProgram);
        return EV_FAIL_PIPELINE_CREATE_ERROR;
    }

    glDetachShader(glProgram->glId, evGetGLShaderId(programDesc.vertexShader));
    glDetachShader(glProgram->glId, evGetGLShaderId(programDesc.pixelShader));

    *program = glProgram;
    return EV_SUCCESS;
}

void evDestroyGLProgram(EvGLProgram *program) {
    if (program->glId) {
        glDeleteProgram(program->glId);
        program->glId = 0;
    }
    free(program);
}

u32 evGetGLProgramId(const EvGLProgram *program) { return program->glId; }
