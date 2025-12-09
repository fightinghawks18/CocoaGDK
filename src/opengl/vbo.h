//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"
#include "../graphics/types.h"

typedef struct EvGLVertexBufferObject EvGLVertexBufferObject;

EvGLVertexBufferObject *evCreateGLVertexBufferObject();
void evDestroyGLVertexBufferObject(EvGLVertexBufferObject *vbo);

void evMapGLVertexBufferObject(const EvGLVertexBufferObject *vbo, const EvBufferMapper& mapper);

u32 evGetGLVertexBufferObjectId(const EvGLVertexBufferObject *vbo);