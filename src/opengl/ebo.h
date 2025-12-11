//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"
#include "../graphics/types.h"

typedef struct CcoGLElementBufferObject_T CcoGLElementBufferObject_T;
typedef CcoGLElementBufferObject_T* CcoGLElementBufferObject;

CcoGLElementBufferObject ccoCreateGLElementBufferObject();
void ccoDestroyGLElementBufferObject(CcoGLElementBufferObject ebo);

void ccoUseGLElementBufferObject(CcoGLElementBufferObject ebo);
void ccoMapGLElementBufferObject(CcoGLElementBufferObject ebo, const CcoBufferMapper *mapper);

u32 ccoGetGLElementBufferObjectId(CcoGLElementBufferObject ebo);