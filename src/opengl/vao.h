#pragma once

#include "../core.h"

typedef struct CcoGLVertexArrayObject_T CcoGLVertexArrayObject_T;
typedef CcoGLVertexArrayObject_T* CcoGLVertexArrayObject;

CcoGLVertexArrayObject ccoCreateGLVertexArrayObject();
void ccoDestroyGLVertexArrayObject(CcoGLVertexArrayObject vao);

u32 ccoGetGLVertexArrayObjectId(CcoGLVertexArrayObject vao);