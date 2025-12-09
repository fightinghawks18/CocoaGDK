#pragma once

#include "../tools/types.h"

typedef struct EvGLVertexArrayObject EvGLVertexArrayObject;

EvGLVertexArrayObject* evCreateGLVertexArrayObject();
void evDestroyGLVertexArrayObject(EvGLVertexArrayObject *vao);

u32 evGetGLVertexArrayObjectId(EvGLVertexArrayObject *vao);