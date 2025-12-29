//
// Created by fightinghawks18 on 12/29/2025.
//

#pragma once

#include "export.h"
#include "core/core_types.h"
#include "graphics/image_loader.h"

typedef struct cco_opengl_image_t cco_opengl_image_t;
typedef cco_opengl_image_t *cco_opengl_image;

CCO_OPENGL_API cco_result cco_create_opengl_image(cco_opengl_image *out_image);
CCO_OPENGL_API void cco_destroy_opengl_image(cco_opengl_image image);

CCO_OPENGL_API void cco_opengl_image_use(i32 texture_type, cco_opengl_image image);
CCO_OPENGL_API void cco_opengl_image_bind(u32 texture_slot, cco_opengl_image image);

CCO_OPENGL_API void cco_opengl_image_upload(i32 texture_type, cco_opengl_image image, const cco_image *image_data);

CCO_OPENGL_API void cco_opengl_image_set_wrapping(cco_opengl_image image, i32 wrap_s, i32 wrap_t);
CCO_OPENGL_API void cco_opengl_image_set_filtering(cco_opengl_image image, i32 min_filter, i32 mag_filter);