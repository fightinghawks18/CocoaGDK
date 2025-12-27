//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#include "core/core_types.h"
#include "core/log.h"
#include "export.h"

// Buffers

typedef void (*PFNGLGENBUFFERSPROC)(usize n, u32 *buffers);
extern PFNGLGENBUFFERSPROC glGenBuffers;

typedef void (*PFNGLDELETEBUFFERSPROC)(usize n, u32 *buffers);
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_UNIFORM_BUFFER 0x8A11
typedef void (*PFNGLBINDBUFFERPROC)(i32 bufferType, i32 buffer);
extern PFNGLBINDBUFFERPROC glBindBuffer;

typedef void (*PFNGLBUFFERSUBDATAPROC)(i32 bufferType, i64 offset, i64 size, void *data);
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
typedef void (*PFNGLBUFFERDATAPROC)(i32 bufferType, i64 size, void *data, i32 drawType);
extern PFNGLBUFFERDATAPROC glBufferData;

typedef void (*PFNGLBINDBUFFERBASEPROC)(i32 bufferType, i32 binding, u32 buffer);
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;

typedef u32 (*PFNGLGETUNIFORMBLOCKINDEXPROC)(u32 program, const char *blockName);
extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;

typedef void (*PFNGLUNIFORMBLOCKBINDINGPROC)(u32 program, u32 blockIndex, u32 bindingPoint);
extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;

// Vertex Array Object

typedef void (*PFNGLGENVERTEXARRAYSPROC)(usize n, u32 *arrays);
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;

typedef void (*PFNGLDELETEVERTEXARRAYSPROC)(usize n, u32 *arrays);
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

typedef void (*PFNGLBINDVERTEXARRAY)(u32 array);
extern PFNGLBINDVERTEXARRAY glBindVertexArray;

#define GL_FALSE 0
#define GL_TRUE 1
typedef void (*PFNGLVERTEXATTRIBPOINTERPROC)(u32 location, i32 numComponents, i32 componentType, i32 normalized,
                                             i32 stride, void *offset);
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

typedef void (*PFNGLENABLEVERTEXATTRIBARRAYPROC)(u32 location);
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

// Main

#define GL_DEPTH_TEST 0x0B71
typedef void (*PFNGLENABLEPROC)(i32 bits);
extern PFNGLENABLEPROC glEnable;

typedef void (*PFNGLDISABLEPROC)(i32 bits);
extern PFNGLDISABLEPROC glDisable;

#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
typedef void (*PFNGLDEPTHFUNCPROC)(i32 bits);
extern PFNGLDEPTHFUNCPROC glDepthFunc;

typedef void (*PFNGLVIEWPORTPROC)(i32 x, i32 y, i32 width, i32 height);
extern PFNGLVIEWPORTPROC glViewport;

typedef void (*PFNGLDEPTHRANGEPROC)(f32 min, f32 max);
extern PFNGLDEPTHRANGEPROC glDepthRange;

#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
typedef void (*PFNGLCLEARPROC)(i32 bits);
extern PFNGLCLEARPROC glClear;

typedef void (*PFNGLCLEARCOLORPROC)(f32 red, f32 green, f32 blue, f32 alpha);
extern PFNGLCLEARCOLORPROC glClearColor;

#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_LINES 0x0001
#define GL_FLOAT 0x1406
#define GL_UNSIGNED_INT 0x1405
typedef void (*PFNGLDRAWELEMENTSPROC)(i32 prim, i32 numIndices, i32 indexType, void *indices);
extern PFNGLDRAWELEMENTSPROC glDrawElements;

// Program

typedef u32 (*PFNGLCREATEPROGRAMPROC)(void);
extern PFNGLCREATEPROGRAMPROC glCreateProgram;

typedef void (*PFNGLDELETEPROGRAMPROC)(u32 program);
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;

typedef void (*PFNGLLINKPROGRAMPROC)(u32 program);
extern PFNGLLINKPROGRAMPROC glLinkProgram;

#define GL_LINK_STATUS 0x8B82
typedef void (*PFNGLGETPROGRAMIVPROC)(u32 program, i32 valueType, i32 *outValue);
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;

typedef void (*PFNGLGETPROGRAMINFOLOGPROC)(u32 program, usize logSize, void *d, char *infoLog);
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

typedef void (*PFNGLUSEPROGRAMPROC)(u32 program);
extern PFNGLUSEPROGRAMPROC glUseProgram;

// Shader

typedef void (*PFNGLATTACHSHADERPROC)(u32 program, u32 shader);
extern PFNGLATTACHSHADERPROC glAttachShader;

typedef void (*PFNGLDETACHSHADERPROC)(u32 program, u32 shader);
extern PFNGLDETACHSHADERPROC glDetachShader;

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
typedef u32 (*PFNGLCREATESHADERPROC)(i32 shaderType);
extern PFNGLCREATESHADERPROC glCreateShader;

typedef void (*PFNGLSHADERSOURCEPROC)(u32 shader, usize count, const char *const *shaderContents, i32 *length);
extern PFNGLSHADERSOURCEPROC glShaderSource;

typedef void (*PFNGLCOMPILESHADERPROC)(u32 shader);
extern PFNGLCOMPILESHADERPROC glCompileShader;

#define GL_COMPILE_STATUS 0x8B81
typedef void (*PFNGLGETSHADERIVPROC)(u32 shader, i32 valueType, i32 *outValue);
extern PFNGLGETSHADERIVPROC glGetShaderiv;

typedef void (*PFNGLGETSHADERINFOLOGPROC)(u32 shader, usize logSize, void *d, char *infoLog);
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

typedef void (*PFNGLDELETESHADERPROC)(u32 shader);
extern PFNGLDELETESHADERPROC glDeleteShader;

CCO_OPENGL_API void *cco_get_opengl_proc_addr(const char *addr_name);
CCO_OPENGL_API static cco_bool cco_load_opengl_functions() {
    // Buffers
    glGenBuffers = (PFNGLGENBUFFERSPROC)cco_get_opengl_proc_addr("glGenBuffers");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)cco_get_opengl_proc_addr("glDeleteBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)cco_get_opengl_proc_addr("glBindBuffer");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)cco_get_opengl_proc_addr("glBufferSubData");
    glBufferData = (PFNGLBUFFERDATAPROC)cco_get_opengl_proc_addr("glBufferData");
    glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)cco_get_opengl_proc_addr("glBindBufferBase");
    glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)cco_get_opengl_proc_addr("glGetUniformBlockIndex");
    glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)cco_get_opengl_proc_addr("glUniformBlockBinding");

    // Vertex Array Object
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)cco_get_opengl_proc_addr("glGenVertexArrays");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)cco_get_opengl_proc_addr("glDeleteVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAY)cco_get_opengl_proc_addr("glBindVertexArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)cco_get_opengl_proc_addr("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)cco_get_opengl_proc_addr("glEnableVertexAttribArray");

    // Main
    glEnable = (PFNGLENABLEPROC)cco_get_opengl_proc_addr("glEnable");
    glDisable = (PFNGLDISABLEPROC)cco_get_opengl_proc_addr("glDisable");
    glDepthFunc = (PFNGLDEPTHFUNCPROC)cco_get_opengl_proc_addr("glDepthFunc");
    glViewport = (PFNGLVIEWPORTPROC)cco_get_opengl_proc_addr("glViewport");
    glDepthRange = (PFNGLDEPTHRANGEPROC)cco_get_opengl_proc_addr("glDepthRange");
    glClear = (PFNGLCLEARPROC)cco_get_opengl_proc_addr("glClear");
    glClearColor = (PFNGLCLEARCOLORPROC)cco_get_opengl_proc_addr("glClearColor");
    glDrawElements = (PFNGLDRAWELEMENTSPROC)cco_get_opengl_proc_addr("glDrawElements");

    // Program
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)cco_get_opengl_proc_addr("glCreateProgram");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)cco_get_opengl_proc_addr("glDeleteProgram");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)cco_get_opengl_proc_addr("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)cco_get_opengl_proc_addr("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)cco_get_opengl_proc_addr("glGetProgramInfoLog");
    glUseProgram = (PFNGLUSEPROGRAMPROC)cco_get_opengl_proc_addr("glUseProgram");

    // Shader
    glCreateShader = (PFNGLCREATESHADERPROC)cco_get_opengl_proc_addr("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)cco_get_opengl_proc_addr("glShaderSource");
    glAttachShader = (PFNGLATTACHSHADERPROC)cco_get_opengl_proc_addr("glAttachShader");
    glDetachShader = (PFNGLDETACHSHADERPROC)cco_get_opengl_proc_addr("glDetachShader");
    glCompileShader = (PFNGLCOMPILESHADERPROC)cco_get_opengl_proc_addr("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)cco_get_opengl_proc_addr("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)cco_get_opengl_proc_addr("glGetShaderInfoLog");
    glDeleteShader = (PFNGLDELETESHADERPROC)cco_get_opengl_proc_addr("glDeleteShader");

    return CCO_YES;
}