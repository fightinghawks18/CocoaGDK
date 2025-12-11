//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"
#include "shader.h"

typedef struct CcoGLProgram_T CcoGLProgram_T;
typedef CcoGLProgram_T* CcoGLProgram;

typedef struct {
    CcoGLShader vertexShader;
    CcoGLShader pixelShader;
} CcoGLProgramDesc;

CcoResult ccoCreateGLProgram(const CcoGLProgramDesc *programDesc, CcoGLProgram *program);
void ccoDestroyGLProgram(CcoGLProgram program);

void ccoUseGLProgram(CcoGLProgram program);

u32 ccoGetGLProgramId(CcoGLProgram program);