//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"

typedef struct CcoOpenGLContext_T CcoOpenGLContext_T;
typedef CcoOpenGLContext_T *CcoOpenGLContext;

CcoResult ccoCreateOpenGLContext(void *windowHandle, void *displayHandle, CcoOpenGLContext *outOpenGLContext);
void ccoDestroyOpenGLContext(CcoOpenGLContext openGLContext);
void ccoFlushOpenGLContextBuffer(CcoOpenGLContext openGLContext);
void ccoMakeCurrentOpenGLContext(CcoOpenGLContext openGLContext);
void ccoRemoveCurrentOpenGLContext();