//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

typedef size_t usize;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef enum {
    EV_SUCCESS = 0, ///< @brief Successfully completed operation
    EV_FAIL_FILE_NON_EXISTENT, ///< @brief Attempted to read a file with an invalid path
    EV_FAIL_CANT_READ_FILE, ///< @brief Failed to read a file due to insufficient permissions
    EV_FAIL_OUT_OF_MEMORY, ///< @brief Memory allocation failed due to insufficient memory
    EV_FAIL_GRAPHICS_INIT_ERROR, ///< @brief The graphics system failed to initialize
    EV_FAIL_COMPILE_ERROR, ///< @brief Failed to compile a file
    EV_FAIL_PIPELINE_CREATE_ERROR, ///< @brief Failed to create a pipeline
} EvResult;