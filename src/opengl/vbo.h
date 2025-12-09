//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"
#include "../graphics/types.h"

typedef struct CcoGLVertexBufferObject CcoGLVertexBufferObject;

CcoGLVertexBufferObject *ccoCreateGLVertexBufferObject();
void ccoDestroyGLVertexBufferObject(CcoGLVertexBufferObject *vbo);

void ccoMapGLVertexBufferObject(const CcoGLVertexBufferObject *vbo, const CcoBufferMapper& mapper);

u32 ccoGetGLVertexBufferObjectId(const CcoGLVertexBufferObject *vbo);