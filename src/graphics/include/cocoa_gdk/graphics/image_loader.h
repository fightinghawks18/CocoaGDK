//
// Created by fightinghawks18 on 12/29/2025.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "export.h"
#include "cocoa_gdk/core/core_types.h"
#include "cocoa_gdk/core/results.h"

typedef enum {
    CCO_IMAGE_CHANNEL_UNKNOWN,
    CCO_IMAGE_CHANNEL_GRAYSCALE,
    CCO_IMAGE_CHANNEL_GRAYSCALE_ALPHA,
    CCO_IMAGE_CHANNEL_RGB,
    CCO_IMAGE_CHANNEL_RGB_ALPHA,
} cco_image_channel;

typedef struct {
    i32 width;
    i32 height;
    cco_image_channel channel;
    void *pixels;
} cco_image;

CCO_GRAPHICS_API cco_result cco_load_image_from_file(const char *path, cco_image *out_image);
CCO_GRAPHICS_API void cco_free_image(cco_image image);

#ifdef __cplusplus
}
#endif