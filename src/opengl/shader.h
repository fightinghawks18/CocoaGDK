//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"
#include "../graphics/types.h"

typedef struct EvGLShader EvGLShader;

typedef struct {
    EvShaderType shaderType;
    const char *shaderPath;
} EvGLShaderDesc;

EvResult evCreateGLShader(const EvGLShaderDesc &shaderDesc, EvGLShader **shader);
void evDestroyGLShader(EvGLShader *shader);

u32 evGetGLShaderId(const EvGLShader *shader);