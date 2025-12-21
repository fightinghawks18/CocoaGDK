//
// Created by fightinghawks18 on 12/8/25.
//

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define CCO_NIL NULL
#define CCO_YES 1
#define CCO_NO 0

#ifdef DEBUG
#define CCO_LOG(str, ...) printf(str "\n", ##__VA_ARGS__);
#else
#define CCO_LOG(str, ...)
#endif

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

typedef u8 CcoBool;

typedef enum {
    CCO_SUCCESS = 0,                              ///< @brief Successfully completed operation
    CCO_FAIL_FILE_NON_EXISTENT,                   ///< @brief Attempted to read a file with an invalid path
    CCO_FAIL_CANT_READ_FILE,                      ///< @brief Failed to read a file due to insufficient permissions
    CCO_FAIL_OUT_OF_MEMORY,                       ///< @brief Memory allocation failed due to insufficient memory
    CCO_FAIL_GRAPHICS_INIT_ERROR,                 ///< @brief The graphics system failed to initialize
    CCO_FAIL_GRAPHICS_NO_ADAPTERS_ERROR,          ///< @brief The graphics system failed to find any adapters available
    CCO_FAIL_GRAPHICS_NO_SUITABLE_ADAPTERS_ERROR, ///< @brief The graphics system failed to find a suitable adapter
    CCO_FAIL_GRAPHICS_CREATE_ERROR,               ///< @brief Failed to create a graphics resource
    CCO_FAIL_COMPILE_ERROR,                       ///< @brief Failed to compile a file
    CCO_FAIL_PIPELINE_CREATE_ERROR,               ///< @brief Failed to create a pipeline
    CCO_FAIL_WINDOWING_INIT_ERROR,                ///< @brief Failed to initialize the windowing system
    CCO_FAIL_WINDOWING_CREATE_ERROR,              ///< @brief Failed to create a new window
    CCO_FAIL_INPUT_INIT_ERROR,                    ///< @brief Failed to initialize the input system
} CcoResult;