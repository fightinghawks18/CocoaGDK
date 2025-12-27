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
static cco_bool cco_load_opengl_functions() {
    // Buffers
    glGenBuffers = (PFNGLGENBUFFERSPROC)cco_get_opengl_proc_addr("glGenBuffers");
    if (!glGenBuffers) return CCO_NO;

    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)cco_get_opengl_proc_addr("glDeleteBuffers");
    if (!glDeleteBuffers) return CCO_NO;

    glBindBuffer = (PFNGLBINDBUFFERPROC)cco_get_opengl_proc_addr("glBindBuffer");
    if (!glBindBuffer) return CCO_NO;

    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)cco_get_opengl_proc_addr("glBufferSubData");
    if (!glBufferSubData) return CCO_NO;

    glBufferData = (PFNGLBUFFERDATAPROC)cco_get_opengl_proc_addr("glBufferData");
    if (!glBufferData) return CCO_NO;

    glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)cco_get_opengl_proc_addr("glBindBufferBase");
    if (!glBindBufferBase) return CCO_NO;

    glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)cco_get_opengl_proc_addr("glGetUniformBlockIndex");
    if (!glGetUniformBlockIndex) return CCO_NO;

    glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)cco_get_opengl_proc_addr("glUniformBlockBinding");
    if (!glUniformBlockBinding) return CCO_NO;

    // Vertex Array Object
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)cco_get_opengl_proc_addr("glGenVertexArrays");
    if (!glGenVertexArrays) return CCO_NO;

    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)cco_get_opengl_proc_addr("glDeleteVertexArrays");
    if (!glDeleteVertexArrays) return CCO_NO;

    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)cco_get_opengl_proc_addr("glBindVertexArray");
    if (!glBindVertexArray) return CCO_NO;

    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)cco_get_opengl_proc_addr("glVertexAttribPointer");
    if (!glVertexAttribPointer) return CCO_NO;

    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)cco_get_opengl_proc_addr("glEnableVertexAttribArray");
    if (!glEnableVertexAttribArray) return CCO_NO;

    // Main
    glEnable = (PFNGLENABLEPROC)cco_get_opengl_proc_addr("glEnable");
    if (!glEnable) return CCO_NO;

    glDisable = (PFNGLDISABLEPROC)cco_get_opengl_proc_addr("glDisable");
    if (!glDisable) return CCO_NO;

    glDepthFunc = (PFNGLDEPTHFUNCPROC)cco_get_opengl_proc_addr("glDepthFunc");
    if (!glDepthFunc) return CCO_NO;

    glViewport = (PFNGLVIEWPORTPROC)cco_get_opengl_proc_addr("glViewport");
    if (!glViewport) return CCO_NO;

    glDepthRange = (PFNGLDEPTHRANGEPROC)cco_get_opengl_proc_addr("glDepthRange");
    if (!glDepthRange) return CCO_NO;

    glClear = (PFNGLCLEARPROC)cco_get_opengl_proc_addr("glClear");
    if (!glClear) return CCO_NO;

    glClearColor = (PFNGLCLEARCOLORPROC)cco_get_opengl_proc_addr("glClearColor");
    if (!glClearColor) return CCO_NO;

    glDrawElements = (PFNGLDRAWELEMENTSPROC)cco_get_opengl_proc_addr("glDrawElements");
    if (!glDrawElements) return CCO_NO;

    // Program
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)cco_get_opengl_proc_addr("glCreateProgram");
    if (!glCreateProgram) return CCO_NO;

    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)cco_get_opengl_proc_addr("glDeleteProgram");
    if (!glDeleteProgram) return CCO_NO;

    glLinkProgram = (PFNGLLINKPROGRAMPROC)cco_get_opengl_proc_addr("glLinkProgram");
    if (!glLinkProgram) return CCO_NO;

    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)cco_get_opengl_proc_addr("glGetProgramiv");
    if (!glGetProgramiv) return CCO_NO;

    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)cco_get_opengl_proc_addr("glGetProgramInfoLog");
    if (!glGetProgramInfoLog) return CCO_NO;

    glUseProgram = (PFNGLUSEPROGRAMPROC)cco_get_opengl_proc_addr("glUseProgram");
    if (!glUseProgram) return CCO_NO;

    // Shader
    glCreateShader = (PFNGLCREATESHADERPROC)cco_get_opengl_proc_addr("glCreateShader");
    if (!glCreateShader) return CCO_NO;

    glShaderSource = (PFNGLSHADERSOURCEPROC)cco_get_opengl_proc_addr("glShaderSource");
    if (!glShaderSource) return CCO_NO;

    glAttachShader = (PFNGLATTACHSHADERPROC)cco_get_opengl_proc_addr("glAttachShader");
    if (!glAttachShader) return CCO_NO;

    glDetachShader = (PFNGLDETACHSHADERPROC)cco_get_opengl_proc_addr("glDetachShader");
    if (!glDetachShader) return CCO_NO;

    glCompileShader = (PFNGLCOMPILESHADERPROC)cco_get_opengl_proc_addr("glCompileShader");
    if (!glCompileShader) return CCO_NO;

    glGetShaderiv = (PFNGLGETSHADERIVPROC)cco_get_opengl_proc_addr("glGetShaderiv");
    if (!glGetShaderiv) return CCO_NO;

    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)cco_get_opengl_proc_addr("glGetShaderInfoLog");
    if (!glGetShaderInfoLog) return CCO_NO;

    glDeleteShader = (PFNGLDELETESHADERPROC)cco_get_opengl_proc_addr("glDeleteShader");
    if (!glDeleteShader) return CCO_NO;

    CCO_LOG("All OpenGL functions loaded successfully");
    return CCO_YES;
}