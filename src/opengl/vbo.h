//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"
#include "../graphics/types.h"

typedef struct CcoGLVertexBufferObject_T CcoGLVertexBufferObject_T;
typedef CcoGLVertexBufferObject_T* CcoGLVertexBufferObject;

CcoGLVertexBufferObject ccoCreateGLVertexBufferObject();
void ccoDestroyGLVertexBufferObject(CcoGLVertexBufferObject vbo);

void ccoMapGLVertexBufferObject(CcoGLVertexBufferObject vbo, const CcoBufferMapper *mapper);

u32 ccoGetGLVertexBufferObjectId(CcoGLVertexBufferObject vbo);