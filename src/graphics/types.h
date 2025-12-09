//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include "../core.h"

/// @brief Describes how the rendered output is transformed onto the screen
typedef struct {
    int x, y;
    int w, h;
    u32 minDepth = 0; ///< @brief The smallest depth value allowed (clamps depths smaller than this)
    u32 maxDepth = 1; ///< @brief The largest depth value allowed (clamps depths larger than this)
} EvViewport;

typedef struct {
    usize offset;
    usize size;
    void *data;
} EvBufferMapper;

/// @brief Describes the regions that pixels/fragments are accepted in,
/// otherwise it'll discard anything outside the region
/// @note If you want to transform how the rendered output will look, see EvViewport
/// @see EvViewport
typedef struct {
    int x, y;
    int w, h;
} EvScissor;

/// @brief The type of operation that a shader uses
typedef enum EvShaderType {
    EV_SHADER_TYPE_VERTEX, ///< @brief Shader operates on per-vertex
    EV_SHADER_TYPE_PIXEL ///< @brief Shader operates on per-pixel/per-fragment
} EvShaderType;