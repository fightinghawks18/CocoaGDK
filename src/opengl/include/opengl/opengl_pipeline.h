//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"
#include "opengl/opengl_shader.h"

typedef struct CcoOpenGLPipeline_T CcoOpenGLPipeline_T;
typedef CcoOpenGLPipeline_T* CcoOpenGLPipeline;

typedef struct {
    CcoOpenGLShader vertexShader;
    CcoOpenGLShader pixelShader;
} CcoOpenGLPipelineDesc;

CcoResult ccoCreateOpenGLPipeline(const CcoOpenGLPipelineDesc *description, CcoOpenGLPipeline *outPipeline);
void ccoDestroyOpenGLPipeline(CcoOpenGLPipeline pipeline);

void ccoUseOpenGLPipeline(CcoOpenGLPipeline pipeline);

u32 ccoGetOpenGLPipelineID(CcoOpenGLPipeline pipeline);