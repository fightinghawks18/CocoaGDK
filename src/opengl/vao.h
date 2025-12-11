#pragma once

#include "../core.h"

typedef struct CcoGLVertexArrayObject_T CcoGLVertexArrayObject_T;
typedef CcoGLVertexArrayObject_T* CcoGLVertexArrayObject;

typedef struct {
    u32 location;
    u32 type;
    i32 size;
    i32 stride;
    u32 offset;
} CcoGLVertexLayoutDesc;

typedef struct {
    CcoGLVertexLayoutDesc *layouts;
    u32 layoutCount;
} CcoGLVertexArrayObjectDesc;

CcoGLVertexArrayObject ccoCreateGLVertexArrayObject(const CcoGLVertexArrayObjectDesc *vaoDesc);
void ccoDestroyGLVertexArrayObject(CcoGLVertexArrayObject vao);

void ccoUseGLVertexArrayObject(CcoGLVertexArrayObject vao);

u32 ccoGetGLVertexArrayObjectId(CcoGLVertexArrayObject vao);