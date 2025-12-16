//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../../../math/include/math/matrix4x4.h"
#include "core/core_types.h"

typedef struct {
    CcoMatrix4X4 model;
    CcoMatrix4X4 view;
    CcoMatrix4X4 projection;
} CcoModelViewProjection;

/// @brief Describes how data will be mapped into a buffer
typedef struct {
    usize dataSize;
    usize dataOffset;
    void *data;
} CcoBufferMapping;

/// @brief Describes how a vertex attribute will be laid out in memory
typedef struct {
    u32 location;
    u32 numComponents;
    usize stride;
    usize offset;
} CcoVertexAttribute;

/// @brief Describes how vertex data will be laid out in memory
typedef struct {
    CcoVertexAttribute *attributes;
    u32 attributeCount;
} CcoVertexLayout;

/// @brief Describes how the rendered output is transformed onto the screen
typedef struct {
    i32 x, y;
    i32 w, h;
    f32 minDepth; ///< @brief The smallest depth value allowed (clamps depths smaller than this)
    f32 maxDepth; ///< @brief The largest depth value allowed (clamps depths larger than this)
} CcoViewport;

/// @brief A definition of a single point in a geometric object
typedef struct {
    f32 pos[3];
    f32 col[4];
} CcoVertex;

typedef struct {
    usize offset;
    usize size;
    void *data;
} CcoBufferMapper;

/// @brief Describes the regions that pixels/fragments are accepted in,
/// otherwise it'll discard anything outside the region
/// @note If you want to transform how the rendered output will look, see CcoViewport
/// @see CcoViewport
typedef struct {
    i32 x, y;
    i32 w, h;
} CcoScissor;

typedef struct {
    i32 w, h;
} CcoExtent;

/// @brief The type of operation that a shader uses
typedef enum CcoShaderType {
    CCO_SHADER_TYPE_VERTEX, ///< @brief Shader operates on per-vertex
    CCO_SHADER_TYPE_PIXEL ///< @brief Shader operates on per-pixel/per-fragment
} CcoShaderType;

/// @brief Thw type of value
typedef enum CcoUniformType {
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
} CcoUniformType;