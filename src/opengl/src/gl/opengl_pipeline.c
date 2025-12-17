//
// Created by fightinghawks18 on 12/14/25.
//

#include "../../include/opengl/gl/opengl_pipeline.h"

#include <glad/glad.h>
#include <stdlib.h>

struct CcoOpenGLPipeline_T {
    u32 glID;
};

CcoResult ccoCreateOpenGLPipeline(const CcoOpenGLPipelineDesc *description, CcoOpenGLPipeline *outPipeline) {
    CcoOpenGLPipeline pipeline = malloc(sizeof(CcoOpenGLPipeline));
    pipeline->glID = glCreateProgram();

    glAttachShader(pipeline->glID, ccoGetOpenGLShaderID(description->vertexShader));
    glAttachShader(pipeline->glID, ccoGetOpenGLShaderID(description->pixelShader));
    glLinkProgram(pipeline->glID);

    i32 linkSuccess;
    glGetProgramiv(pipeline->glID, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        char infoLog[512];
        glGetProgramInfoLog(pipeline->glID, 512, NULL, infoLog);
        CCO_LOG("Failed to link OpenGL pipeline! %s", infoLog);
        ccoDestroyOpenGLPipeline(pipeline);
        return CCO_FAIL_PIPELINE_CREATE_ERROR;
    }

    glDetachShader(pipeline->glID, ccoGetOpenGLShaderID(description->vertexShader));
    glDetachShader(pipeline->glID, ccoGetOpenGLShaderID(description->pixelShader));

    *outPipeline = pipeline;
    return CCO_SUCCESS;
}

void ccoDestroyOpenGLPipeline(CcoOpenGLPipeline pipeline) {
    if (pipeline->glID) {
        glDeleteProgram(pipeline->glID);
        pipeline->glID = 0;
    }
    free(pipeline);
}

void ccoUseOpenGLPipeline(CcoOpenGLPipeline pipeline) { glUseProgram(pipeline->glID); }

u32 ccoGetOpenGLPipelineID(CcoOpenGLPipeline pipeline) { return pipeline->glID; }