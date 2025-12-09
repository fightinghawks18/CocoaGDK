//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"
#include "../graphics/types.h"

typedef struct EvGLElementBufferObject EvGLElementBufferObject;

EvGLElementBufferObject *evCreateGLElementBufferObject();
void evDestroyGLElementBufferObject(EvGLElementBufferObject *ebo);

void evMapGLElementBufferObject(const EvGLElementBufferObject *ebo, const EvBufferMapper& mapper);

u32 evGetGLElementBufferObjectId(const EvGLElementBufferObject *ebo);