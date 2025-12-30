//
// Created by fightinghawks18 on 12/29/2025.
//

#pragma once

typedef enum {
    CCO_SUCCESS = 0,                ///< @brief Successfully completed operation
    CCO_GRAPHICS_REBUILD_SWAPCHAIN, ///< @brief The graphics swapchain must be rebuilt, don't count this as an error!
    CCO_FAIL_FILE_NON_EXISTENT,     ///< @brief Attempted to read a file with an invalid path
    CCO_FAIL_CANT_READ_FILE,        ///< @brief Failed to read a file due to insufficient permissions
    CCO_FAIL_OUT_OF_MEMORY,         ///< @brief Memory allocation failed due to insufficient memory
    CCO_FAIL_GRAPHICS_INIT_ERROR,   ///< @brief The graphics system failed to initialize
    CCO_FAIL_GRAPHICS_NO_ADAPTERS_ERROR,          ///< @brief The graphics system failed to find any adapters available
    CCO_FAIL_GRAPHICS_NO_SUITABLE_ADAPTERS_ERROR, ///< @brief The graphics system failed to find a suitable adapter
    CCO_FAIL_GRAPHICS_CREATE_ERROR,               ///< @brief Failed to create a graphics resource
    CCO_FAIL_GRAPHICS_FRAME_START_ERROR,          ///< @brief Failed to begin start a graphics frame
    CCO_FAIL_GRAPHICS_FRAME_END_ERROR,            ///< @brief Failed to begin end a graphics frame
    CCO_FAIL_GRAPHICS_FRAME_SUBMIT_ERROR,         ///< @brief Failed to begin submit a graphics frame
    CCO_FAIL_GRAPHICS_FRAME_PRESENT_ERROR,        ///< @brief Failed to begin present a graphics frame
    CCO_FAIL_COMPILE_ERROR,                       ///< @brief Failed to compile a file
    CCO_FAIL_PIPELINE_CREATE_ERROR,               ///< @brief Failed to create a pipeline
    CCO_FAIL_PLATFORM_INIT_ERROR,                 ///< @brief Failed to initialize platform
    CCO_FAIL_WINDOWING_INIT_ERROR,                ///< @brief Failed to initialize the windowing system
    CCO_FAIL_WINDOWING_CREATE_ERROR,              ///< @brief Failed to create a new window
    CCO_FAIL_INPUT_INIT_ERROR,                    ///< @brief Failed to initialize the input system
    CCO_FAIL_AUDIO_INIT_ERROR,                    ///< @brief Failed to initialize the audio system
    CCO_FAIL_AUDIO_STREAM_CREATE_ERROR,           ///< @brief Failed to create an audio stream
    CCO_FAIL_AUDIO_CREATE_PCM_ERROR,              ///< @brief Failed to create PCM data from an audio file
} cco_result;