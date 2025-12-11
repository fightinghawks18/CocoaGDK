//
// Created by fightinghawks18 on 12/10/25.
//

#pragma once

#include "../core.h"
#include "../graphics/types.h"

typedef struct CcoGLUniformBuffer_T CcoGLUniformBuffer_T;
typedef CcoGLUniformBuffer_T* CcoGLUniformBuffer;

CcoGLUniformBuffer ccoCreateGLUniformBuffer();
void ccoDestroyGLUniformBuffer(CcoGLUniformBuffer uniform);

void ccoUseGLUniformBuffer(CcoGLUniformBuffer uniform);
void ccoSetGLUniformBufferUniform(CcoGLUniformBuffer uniform, CcoUniformType uniformType, i32 location, const void *value);
void ccoMapGLUniformBuffer(CcoGLUniformBuffer uniform, const CcoBufferMapper *mapper);

u32 ccoGetGLUniformBufferId(CcoGLUniformBuffer uniform);
