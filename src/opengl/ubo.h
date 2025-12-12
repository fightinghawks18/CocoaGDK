//
// Created by fightinghawks18 on 12/10/25.
//

#pragma once

#include "../core.h"
#include "../graphics/types.h"

typedef struct CcoGLUniformBufferObject_T CcoGLUniformBufferObject_T;
typedef CcoGLUniformBufferObject_T* CcoGLUniformBufferObject;

CcoGLUniformBufferObject ccoCreateGLUniformBuffer(i32 location);
void ccoDestroyGLUniformBuffer(CcoGLUniformBufferObject uniform);

void ccoUseGLUniformBuffer(CcoGLUniformBufferObject uniform);
void ccoMapGLUniformBuffer(CcoGLUniformBufferObject uniform, const CcoBufferMapper *mapper);

u32 ccoGetGLUniformBufferId(CcoGLUniformBufferObject uniform);
