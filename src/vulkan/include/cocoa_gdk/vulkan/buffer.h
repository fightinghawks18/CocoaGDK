//
// Created by fightinghawks18 on 12/24/2025.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "cocoa_gdk/core/core_types.h"
#include "cocoa_gdk/core/results.h"
#include "cocoa_gdk/core/log.h"
#include "cocoa_gdk/graphics/graphics_types.h"
#include "instance.h"
#include "export.h"

typedef struct cco_vulkan_buffer_t cco_vulkan_buffer_t;
typedef cco_vulkan_buffer_t *cco_vulkan_buffer;

typedef enum {
    CCO_VULKAN_BUFFER_USAGE_VERTEX_BIT,
    CCO_VULKAN_BUFFER_USAGE_INDEX_BIT,
    CCO_VULKAN_BUFFER_USAGE_UNIFORM_BIT,
    CCO_VULKAN_BUFFER_USAGE_STORAGE_BIT
} cco_vulkan_buffer_usage_bits;
typedef u32 cco_vulkan_buffer_usage_flags;

typedef struct {
    usize size;
    cco_vulkan_buffer_usage_flags usage;
} cco_vulkan_buffer_desc;

CCO_VULKAN_API cco_result cco_create_vulkan_buffer(cco_vulkan_instance instance, const cco_vulkan_buffer_desc *desc, cco_vulkan_buffer *out_buffer);
CCO_VULKAN_API void cco_destroy_vulkan_buffer(cco_vulkan_buffer buffer);

CCO_VULKAN_API void cco_vulkan_buffer_map_data(cco_vulkan_buffer buffer, const cco_buffer_mapping *data);

#ifdef __cplusplus
}
#endif