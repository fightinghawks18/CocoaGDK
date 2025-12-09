#pragma once

#include "../core.h"

typedef struct CcoGLVertexArrayObject CcoGLVertexArrayObject;

CcoGLVertexArrayObject* ccoCreateGLVertexArrayObject();
void ccoDestroyGLVertexArrayObject(CcoGLVertexArrayObject *vao);

u32 ccoGetGLVertexArrayObjectId(const CcoGLVertexArrayObject *vao);