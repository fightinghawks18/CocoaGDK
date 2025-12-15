//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"
#include "graphics/graphics_types.h"
#include "opengl_ebo.h"
#include "opengl_vbo.h"

typedef struct CcoOpenGLVao_T CcoOpenGLVao_T;
typedef CcoOpenGLVao_T *CcoOpenGLVao;

CcoResult ccoCreateOpenGLVao(CcoOpenGLVao *outVao);
void ccoDestroyOpenGLVao(CcoOpenGLVao vao);

void ccoUseOpenGLVao(CcoOpenGLVao vao);
void ccoSetOpenGLVaoLayout(CcoOpenGLVao vao, CcoOpenGLVbo vbo, CcoOpenGLEbo ebo, const CcoVertexLayout *layout);
void ccoRemoveCurrentOpenGLVao();

u32 ccoGetOpenGLVaoID(CcoOpenGLVao vao);
