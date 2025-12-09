//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"
#include "../graphics/types.h"

typedef struct CcoGLShader CcoGLShader;

typedef struct {
    CcoShaderType shaderType;
    const char *shaderPath;
} CcoGLShaderDesc;

CcoResult ccoCreateGLShader(const CcoGLShaderDesc &shaderDesc, CcoGLShader **shader);
void ccoDestroyGLShader(CcoGLShader *shader);

u32 ccoGetGLShaderId(const CcoGLShader *shader);