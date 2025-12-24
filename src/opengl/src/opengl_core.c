//
// Created by fightinghawks18 on 12/16/25.
//

#include "opengl/opengl_core.h"
#include "opengl/opengl_loader.h"

cco_result cco_initialize_opengl(void) {
    if (!cco_load_opengl_functions()) {
        CCO_LOG("Failed to load OpenGL methods!");
        return CCO_FAIL_GRAPHICS_INIT_ERROR;
    }
    return CCO_SUCCESS;
}

void cco_set_opengl_viewport(cco_viewport viewport) {
    glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
    glDepthRange(viewport.min_depth, viewport.max_depth);
}

void cco_clear_opengl_buffers(CcoOpenGLBufferFlags buffer_flags) {
    i32 buffer_bits = 0;
    if (buffer_flags & CCO_OPENGL_COLOR_BUFFER_BIT)
        buffer_bits |= GL_COLOR_BUFFER_BIT;
    if (buffer_flags & CCO_OPENGL_DEPTH_BUFFER_BIT)
        buffer_bits |= GL_DEPTH_BUFFER_BIT;
    glClear(buffer_bits);
}

void cco_set_opengl_clear_color(cco_clear_color clear_color) {
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
}

void cco_draw_opengl_elements(cco_opengl_primitive_type primitive_type, u32 num_indices, cco_opengl_index_type index_type) {
    u32 glPrimitive = 0;
    switch (primitive_type) {
    case CCO_OPENGL_PRIMITIVE_TRIANGLES: {
        glPrimitive = GL_TRIANGLES;
        break;
    }
    case CCO_OPENGL_PRIMITIVE_TRIANGLE_LIST: {
        glPrimitive = GL_TRIANGLE_STRIP;
        break;
    }
    case CCO_OPENGL_PRIMITIVE_LINES: {
        glPrimitive = GL_LINES;
        break;
    }
    }

    u32 glIndexType = 0;
    switch (index_type) {
    case CCO_OPENGL_INDEX_TYPE_U32: {
        glIndexType = GL_UNSIGNED_INT;
        break;
    }
    }
    glDrawElements(glPrimitive, num_indices, glIndexType, NULL);
}
