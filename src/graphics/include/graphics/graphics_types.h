//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "core/core_types.h"
#include "core/log.h"
#include "math/matrix4.h"

typedef struct {
    mat4 model;
    mat4 view;
    mat4 projection;
} cco_model_view_projection;

/// @brief Describes how data will be mapped into a buffer
typedef struct {
    usize data_size;
    usize data_offset;
    void *data;
} cco_buffer_mapping;

/// @brief Describes how a vertex attribute will be laid out in memory
typedef struct {
    u32 location;
    u32 num_components;
    usize stride;
    usize offset;
} cco_vertex_attribute;

/// @brief Describes how vertex data will be laid out in memory
typedef struct {
    cco_vertex_attribute *attributes;
    u32 attribute_count;
} cco_vertex_layout;

/// @brief Describes how the rendered output is transformed onto the screen
typedef struct {
    i32 x, y;
    i32 w, h;
    f32 min_depth; ///< @brief The smallest depth value allowed (clamps depths smaller than this)
    f32 max_depth; ///< @brief The largest depth value allowed (clamps depths larger than this)
} cco_viewport;

/// @brief A definition of a single point in a geometric object
typedef struct {
    f32 pos[3];
    f32 col[4];
} cco_vertex;

/// @brief Describes the regions that pixels/fragments are accepted in,
/// otherwise it'll discard anything outside the region
/// @note If you want to transform how the rendered output will look, see cco_viewport
/// @see cco_viewport
typedef struct {
    i32 x, y;
    i32 w, h;
} cco_scissor;

typedef struct {
    i32 w, h;
} cco_extent;

/// @brief The type of operation that a shader uses
typedef enum cco_shader_type {
    CCO_SHADER_TYPE_VERTEX, ///< @brief Shader operates on per-vertex
    CCO_SHADER_TYPE_PIXEL ///< @brief Shader operates on per-pixel/per-fragment
} cco_shader_type;

/// @brief Thw type of value
typedef enum cco_uniform_type {
    CCO_UNIFORM_TYPE_FLOAT1,
    CCO_UNIFORM_TYPE_FLOAT2,
    CCO_UNIFORM_TYPE_FLOAT3,
    CCO_UNIFORM_TYPE_FLOAT4,
    CCO_UNIFORM_TYPE_INT1,
    CCO_UNIFORM_TYPE_INT2,
    CCO_UNIFORM_TYPE_INT3,
    CCO_UNIFORM_TYPE_INT4,
    CCO_UNIFORM_TYPE_UINT1,
    CCO_UNIFORM_TYPE_UINT2,
    CCO_UNIFORM_TYPE_UINT3,
    CCO_UNIFORM_TYPE_UINT4,
    CCO_UNIFORM_TYPE_MATRIX4X4,
} cco_uniform_type;