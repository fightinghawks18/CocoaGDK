//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"
#include "../graphics/types.h"

typedef struct CcoGLElementBufferObject CcoGLElementBufferObject;

CcoGLElementBufferObject *ccoCreateGLElementBufferObject();
void ccoDestroyGLElementBufferObject(CcoGLElementBufferObject *ebo);

void ccoMapGLElementBufferObject(const CcoGLElementBufferObject *ebo, const CcoBufferMapper& mapper);

u32 ccoGetGLElementBufferObjectId(const CcoGLElementBufferObject *ebo);