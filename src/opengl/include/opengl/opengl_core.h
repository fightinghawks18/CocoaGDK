//
// Created by fightinghawks18 on 12/16/25.
//

#pragma once


#include "core/core_types.h"
#include "core/log.h"
#include "graphics/graphics_types.h"
#include "export.h"

typedef enum {
    CCO_OPENGL_NO_BUFFER_BIT = 0,
    CCO_OPENGL_COLOR_BUFFER_BIT = 1 << 0,
    CCO_OPENGL_DEPTH_BUFFER_BIT = 1 << 1
} cco_opengl_buffer_flag_bits;
typedef u32 CcoOpenGLBufferFlags;

typedef enum {
    CCO_OPENGL_PRIMITIVE_TRIANGLES,
    CCO_OPENGL_PRIMITIVE_TRIANGLE_LIST,
    CCO_OPENGL_PRIMITIVE_LINES
} cco_opengl_primitive_type;

typedef enum {
    CCO_OPENGL_INDEX_TYPE_U32
} cco_opengl_index_type;

typedef struct {
    f32 r, g, b, a;
} cco_clear_color;

CCO_OPENGL_API cco_result cco_initialize_opengl(void);
CCO_OPENGL_API void cco_opengl_set_viewport(cco_viewport viewport);
CCO_OPENGL_API void cco_opengl_clear_buffers(CcoOpenGLBufferFlags buffer_flags);
CCO_OPENGL_API void cco_opengl_set_clear_color(cco_clear_color clear_color);
CCO_OPENGL_API void cco_opengl_draw_elements(cco_opengl_primitive_type primitive_type, u32 num_indices, cco_opengl_index_type index_type);