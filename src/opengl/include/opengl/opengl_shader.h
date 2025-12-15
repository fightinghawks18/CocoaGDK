//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"
#include "graphics/graphics_types.h"

typedef struct CcoOpenGLShader_T CcoOpenGLShader_T;
typedef CcoOpenGLShader_T* CcoOpenGLShader;

typedef struct {
    CcoShaderType shaderType;
    const char *shaderPath;
} CcoOpenGLShaderDesc;

CcoResult ccoCreateOpenGLShader(const CcoOpenGLShaderDesc *description, CcoOpenGLShader *outShader);
void ccoDestroyOpenGLShader(CcoOpenGLShader shader);

u32 ccoGetOpenGLShaderID(CcoOpenGLShader shader);