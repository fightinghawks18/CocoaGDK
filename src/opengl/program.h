//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"
#include "shader.h"

typedef struct EvGLProgram EvGLProgram;

typedef struct {
    EvGLShader *vertexShader;
    EvGLShader *pixelShader;
} EvGLProgramDesc;

EvResult evCreateGLProgram(const EvGLProgramDesc &programDesc, EvGLProgram **program);
void evDestroyGLProgram(EvGLProgram *program);

u32 evGetGLProgramId(const EvGLProgram *program);