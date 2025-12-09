//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"

/// @brief Describes how the rendered output is transformed onto the screen
typedef struct {
    int x, y;
    int w, h;
    u32 minDepth; ///< @brief The smallest depth value allowed (clamps depths smaller than this)
    u32 maxDepth; ///< @brief The largest depth value allowed (clamps depths larger than this)
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
    int x, y;
    int w, h;
} CcoScissor;

/// @brief The type of operation that a shader uses
typedef enum CcoShaderType {
    CCO_SHADER_TYPE_VERTEX, ///< @brief Shader operates on per-vertex
    CCO_SHADER_TYPE_PIXEL ///< @brief Shader operates on per-pixel/per-fragment
} CcoShaderType;