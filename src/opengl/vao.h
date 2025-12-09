#pragma once

#include "../core.h"

typedef struct EvGLVertexArrayObject EvGLVertexArrayObject;

EvGLVertexArrayObject* evCreateGLVertexArrayObject();
void evDestroyGLVertexArrayObject(EvGLVertexArrayObject *vao);

u32 evGetGLVertexArrayObjectId(const EvGLVertexArrayObject *vao);