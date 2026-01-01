//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "cocoa_gdk/core/core_types.h"
#include "cocoa_gdk/core/results.h"
#include "cocoa_gdk/core/log.h"
#include "export.h"

// Buffers

typedef void (*PFNGLGENBUFFERSPROC)(usize n, u32 *buffers);
extern PFNGLGENBUFFERSPROC glGenBuffers;

typedef void (*PFNGLDELETEBUFFERSPROC)(usize n, u32 *buffers);
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_UNIFORM_BUFFER 0x8A11
typedef void (*PFNGLBINDBUFFERPROC)(i32 bufferType, u32 buffer);
extern PFNGLBINDBUFFERPROC glBindBuffer;

typedef void (*PFNGLBUFFERSUBDATAPROC)(i32 bufferType, i64 offset, i64 size, void *data);
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
typedef void (*PFNGLBUFFERDATAPROC)(i32 bufferType, i64 size, void *data, i32 drawType);
extern PFNGLBUFFERDATAPROC glBufferData;

typedef void (*PFNGLBINDBUFFERBASEPROC)(i32 bufferType, u32 binding, u32 buffer);
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;

// Vertex Array Object

typedef void (*PFNGLGENVERTEXARRAYSPROC)(usize n, u32 *arrays);
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;

typedef void (*PFNGLDELETEVERTEXARRAYSPROC)(usize n, u32 *arrays);
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

typedef void (*PFNGLBINDVERTEXARRAYPROC)(u32 array);
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

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
#define GL_UNSIGNED_BYTE 0x1401
#define GL_BYTE 0x1400
#define GL_UNSIGNED_SHORT 0x1403
#define GL_SHORT 0x1402
#define GL_UNSIGNED_INT 0x1405
#define GL_INT 0x1404
#define GL_HALF_FLOAT 0x140B
#define GL_FLOAT 0x1406
typedef void (*PFNGLDRAWELEMENTSPROC)(i32 prim, u32 numIndices, i32 indexType, void *indices);
extern PFNGLDRAWELEMENTSPROC glDrawElements;

// Uniforms

typedef void (*PFNGLUNIFORM1IPROC)(i32 location, i32 value);
extern PFNGLUNIFORM1IPROC glUniform1i;

typedef u32 (*PFNGLGETUNIFORMBLOCKINDEXPROC)(u32 program, const char *blockName);
extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;

typedef void (*PFNGLUNIFORMBLOCKBINDINGPROC)(u32 program, u32 blockIndex, u32 bindingPoint);
extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;

typedef i32 (*PFNGLGETUNIFORMLOCATIONPROC)(u32 program, const char *uniformName);
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;

// Images

typedef void (*PFNGLGENTEXTURESPROC)(usize n, u32 *textures);
extern PFNGLGENTEXTURESPROC glGenTextures;

#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
typedef void (*PFNGLACTIVETEXTUREPROC)(i32 textureUnit);
extern PFNGLACTIVETEXTUREPROC glActiveTexture;

#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_3D 0x806F
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE_RECTANGLE 0x84F5
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
typedef void (*PFNGLBINDTEXTUREPROC)(i32 target, u32 texture);
extern PFNGLBINDTEXTUREPROC glBindTexture;

#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_TEXTURE_LOD_BIAS 0x8501
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_REPEAT 0x2901
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_MIRRORED_REPEAT 0x8370
#define GL_MIRROR_CLAMP_TO_EDGE 0x8743
#define GL_COMPARE_REF_TO_TEXTURE 0x884E
#define GL_NONE 0
#define GL_LEQUAL 0x0203
#define GL_GEQUAL 0x0206
#define GL_LESS 0x0201
#define GL_GREATER 0x0204
#define GL_EQUAL 0x0202
#define GL_NOTEQUAL 0x0205
#define GL_ALWAYS 0x0207
#define GL_NEVER 0x0200
typedef void (*PFNGLTEXPARAMETERIPROC)(i32 target, i32 parameter, i32 value);
extern PFNGLTEXPARAMETERIPROC glTexParameteri;

#define GL_UNPACK_ALIGNMENT 0x0CF5
typedef void (*PFNGLPIXELSTOREIPROC)(i32 parameter, i32 value);
extern PFNGLPIXELSTOREIPROC glPixelStorei;

#define GL_R8 0x8229
#define GL_RG8 0x822B
#define GL_RGB8 0x8051
#define GL_RGBA8 0x8058
#define GL_R16 0x822A
#define GL_RG16 0x822C
#define GL_RGB16 0x8054
#define GL_RGBA16 0x805B
#define GL_RED 0x1903
#define GL_RG 0x8227
#define GL_RGB 0x1907
#define GL_BGR 0x80E0
#define GL_RGBA 0x1908
#define GL_BGRA 0x80E1
#define GL_STENCIL_INDEX 0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_DEPTH_STENCIL 0x84F9
typedef void (*PFNGLTEXIMAGE2DPROC)(i32 target, i32 mipMapLevel, i32 internalFormat, usize width, usize height,
                                    i32 borderWidth, i32 format, i32 pixelType, const void *pixelData);
extern PFNGLTEXIMAGE2DPROC glTexImage2D;

typedef void (*PFNGLGENERATEMIPMAPPROC)(i32 target);
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

typedef void (*PFNGLDELETETEXTURESPROC)(usize n, u32 *images);
extern PFNGLDELETETEXTURESPROC glDeleteTextures;

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
    if (!glGenBuffers)
        return CCO_NO;

    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)cco_get_opengl_proc_addr("glDeleteBuffers");
    if (!glDeleteBuffers)
        return CCO_NO;

    glBindBuffer = (PFNGLBINDBUFFERPROC)cco_get_opengl_proc_addr("glBindBuffer");
    if (!glBindBuffer)
        return CCO_NO;

    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)cco_get_opengl_proc_addr("glBufferSubData");
    if (!glBufferSubData)
        return CCO_NO;

    glBufferData = (PFNGLBUFFERDATAPROC)cco_get_opengl_proc_addr("glBufferData");
    if (!glBufferData)
        return CCO_NO;

    glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)cco_get_opengl_proc_addr("glBindBufferBase");
    if (!glBindBufferBase)
        return CCO_NO;

    // Vertex Array Object
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)cco_get_opengl_proc_addr("glGenVertexArrays");
    if (!glGenVertexArrays)
        return CCO_NO;

    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)cco_get_opengl_proc_addr("glDeleteVertexArrays");
    if (!glDeleteVertexArrays)
        return CCO_NO;

    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)cco_get_opengl_proc_addr("glBindVertexArray");
    if (!glBindVertexArray)
        return CCO_NO;

    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)cco_get_opengl_proc_addr("glVertexAttribPointer");
    if (!glVertexAttribPointer)
        return CCO_NO;

    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)cco_get_opengl_proc_addr("glEnableVertexAttribArray");
    if (!glEnableVertexAttribArray)
        return CCO_NO;

    // Main
    glEnable = (PFNGLENABLEPROC)cco_get_opengl_proc_addr("glEnable");
    if (!glEnable)
        return CCO_NO;

    glDisable = (PFNGLDISABLEPROC)cco_get_opengl_proc_addr("glDisable");
    if (!glDisable)
        return CCO_NO;

    glDepthFunc = (PFNGLDEPTHFUNCPROC)cco_get_opengl_proc_addr("glDepthFunc");
    if (!glDepthFunc)
        return CCO_NO;

    glViewport = (PFNGLVIEWPORTPROC)cco_get_opengl_proc_addr("glViewport");
    if (!glViewport)
        return CCO_NO;

    glDepthRange = (PFNGLDEPTHRANGEPROC)cco_get_opengl_proc_addr("glDepthRange");
    if (!glDepthRange)
        return CCO_NO;

    glClear = (PFNGLCLEARPROC)cco_get_opengl_proc_addr("glClear");
    if (!glClear)
        return CCO_NO;

    glClearColor = (PFNGLCLEARCOLORPROC)cco_get_opengl_proc_addr("glClearColor");
    if (!glClearColor)
        return CCO_NO;

    glDrawElements = (PFNGLDRAWELEMENTSPROC)cco_get_opengl_proc_addr("glDrawElements");
    if (!glDrawElements)
        return CCO_NO;

    // Uniforms

    glUniform1i = (PFNGLUNIFORM1IPROC)cco_get_opengl_proc_addr("glUniform1i");
    if (!glUniform1i)
        return CCO_NO;

    glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)cco_get_opengl_proc_addr("glGetUniformBlockIndex");
    if (!glGetUniformBlockIndex)
        return CCO_NO;

    glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)cco_get_opengl_proc_addr("glUniformBlockBinding");
    if (!glUniformBlockBinding)
        return CCO_NO;

    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)cco_get_opengl_proc_addr("glGetUniformLocation");
    if (!glGetUniformLocation)
        return CCO_NO;

    // Images
    glGenTextures = (PFNGLGENTEXTURESPROC)cco_get_opengl_proc_addr("glGenTextures");
    if (!glGenTextures)
        return CCO_NO;

    glActiveTexture = (PFNGLACTIVETEXTUREPROC)cco_get_opengl_proc_addr("glActiveTexture");
    if (!glActiveTexture)
        return CCO_NO;

    glBindTexture = (PFNGLBINDTEXTUREPROC)cco_get_opengl_proc_addr("glBindTexture");
    if (!glBindTexture)
        return CCO_NO;

    glTexParameteri = (PFNGLTEXPARAMETERIPROC)cco_get_opengl_proc_addr("glTexParameteri");
    if (!glTexParameteri)
        return CCO_NO;

    glPixelStorei = (PFNGLPIXELSTOREIPROC)cco_get_opengl_proc_addr("glPixelStorei");
    if (!glPixelStorei)
        return CCO_NO;

    glTexImage2D = (PFNGLTEXIMAGE2DPROC)cco_get_opengl_proc_addr("glTexImage2D");
    if (!glTexImage2D)
        return CCO_NO;

    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)cco_get_opengl_proc_addr("glGenerateMipmap");
    if (!glGenerateMipmap)
        return CCO_NO;

    glDeleteTextures = (PFNGLDELETETEXTURESPROC)cco_get_opengl_proc_addr("glDeleteTextures");
    if (!glDeleteTextures)
        return CCO_NO;

    // Program
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)cco_get_opengl_proc_addr("glCreateProgram");
    if (!glCreateProgram)
        return CCO_NO;

    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)cco_get_opengl_proc_addr("glDeleteProgram");
    if (!glDeleteProgram)
        return CCO_NO;

    glLinkProgram = (PFNGLLINKPROGRAMPROC)cco_get_opengl_proc_addr("glLinkProgram");
    if (!glLinkProgram)
        return CCO_NO;

    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)cco_get_opengl_proc_addr("glGetProgramiv");
    if (!glGetProgramiv)
        return CCO_NO;

    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)cco_get_opengl_proc_addr("glGetProgramInfoLog");
    if (!glGetProgramInfoLog)
        return CCO_NO;

    glUseProgram = (PFNGLUSEPROGRAMPROC)cco_get_opengl_proc_addr("glUseProgram");
    if (!glUseProgram)
        return CCO_NO;

    // Shader
    glCreateShader = (PFNGLCREATESHADERPROC)cco_get_opengl_proc_addr("glCreateShader");
    if (!glCreateShader)
        return CCO_NO;

    glShaderSource = (PFNGLSHADERSOURCEPROC)cco_get_opengl_proc_addr("glShaderSource");
    if (!glShaderSource)
        return CCO_NO;

    glAttachShader = (PFNGLATTACHSHADERPROC)cco_get_opengl_proc_addr("glAttachShader");
    if (!glAttachShader)
        return CCO_NO;

    glDetachShader = (PFNGLDETACHSHADERPROC)cco_get_opengl_proc_addr("glDetachShader");
    if (!glDetachShader)
        return CCO_NO;

    glCompileShader = (PFNGLCOMPILESHADERPROC)cco_get_opengl_proc_addr("glCompileShader");
    if (!glCompileShader)
        return CCO_NO;

    glGetShaderiv = (PFNGLGETSHADERIVPROC)cco_get_opengl_proc_addr("glGetShaderiv");
    if (!glGetShaderiv)
        return CCO_NO;

    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)cco_get_opengl_proc_addr("glGetShaderInfoLog");
    if (!glGetShaderInfoLog)
        return CCO_NO;

    glDeleteShader = (PFNGLDELETESHADERPROC)cco_get_opengl_proc_addr("glDeleteShader");
    if (!glDeleteShader)
        return CCO_NO;

    CCO_LOG("All OpenGL functions loaded successfully");
    return CCO_YES;
}

#ifdef __cplusplus
}
#endif