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
#include "cocoa_gdk/graphics/graphics_types.h"
#include "export.h"

typedef struct cco_opengl_ebo_t cco_opengl_ebo_t;
typedef cco_opengl_ebo_t *cco_opengl_ebo;

CCO_OPENGL_API cco_result cco_create_opengl_ebo(cco_opengl_ebo *out_ebo);
CCO_OPENGL_API void cco_destroy_opengl_ebo(cco_opengl_ebo ebo);

CCO_OPENGL_API void cco_opengl_ebo_allocate(cco_opengl_ebo ebo, usize ebo_size);

CCO_OPENGL_API void cco_opengl_ebo_use(cco_opengl_ebo ebo);
CCO_OPENGL_API void cco_opengl_ebo_upload(cco_opengl_ebo ebo, const cco_buffer_mapping *mapping);

CCO_OPENGL_API u32 cco_opengl_ebo_get_id(cco_opengl_ebo ebo);

#ifdef __cplusplus
}
#endif