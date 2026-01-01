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
    CCO_FAIL_INPUT_KEYBOARD_INIT_ERROR,           ///< @brief Failed to initialize the keyboard
    CCO_FAIL_INPUT_MOUSE_INIT_ERROR,              ///< @brief Failed to initialize the mouse
    CCO_FAIL_INPUT_GAMEPAD_INIT_ERROR,            ///< @brief Failed to initialize the gamepad
    CCO_FAIL_AUDIO_INIT_ERROR,                    ///< @brief Failed to initialize the audio system
    CCO_FAIL_AUDIO_STREAM_CREATE_ERROR,           ///< @brief Failed to create an audio stream
    CCO_FAIL_AUDIO_CREATE_PCM_ERROR,              ///< @brief Failed to create PCM data from an audio file
    CCO_FAIL_SOCKET_INIT_ERROR,                   ///< @brief Failed to initialize the socket system
    CCO_FAIL_SOCKET_UNKNOWN_ERROR,                ///< @brief Socket operation had an unhandled error reason or is unknown
    CCO_FAIL_SOCKET_CREATE_ERROR,                 ///< @brief Failed to create a socket
    CCO_FAIL_SOCKET_ADDR_IN_USE_ERROR,     ///< @brief Attempting to claim an address that is already claimed by another
                                           ///< socket
    CCO_FAIL_SOCKET_TOO_MANY_ERROR,        ///< @brief There are way too many sockets active
    CCO_FAIL_SOCKET_AF_NO_SUPPORTED_ERROR, ///< @brief The address family attempting to claim is not supported
    CCO_FAIL_SOCKET_NO_PERMS_ERROR,        ///< @brief Trying to bind to a port that requires admin permission
    CCO_FAIL_SOCKET_ADDR_NOT_AVAIL_ERROR,  ///< @brief Socket address doesn't exist on this computer
    CCO_FAIL_SOCKET_BAD_ARGS_ERROR,        ///< @brief Arguments provided to a socket operation were invalid
    CCO_FAIL_SOCKET_BAD_ADDR_ERROR,        ///< @brief Address provided to socket was invalid
    CCO_FAIL_SOCKET_BAD_SOCK_ERROR,        ///< @brief Socket provided was invalid
    CCO_FAIL_SOCKET_CONNECT_TIMEOUT_ERROR, ///< @brief Timed out trying to connect to a host
    CCO_FAIL_SOCKET_CONNECT_REJECTED_ERROR, ///< @brief Socket join was rejected by the host
    CCO_FAIL_SOCKET_CONNECT_MIA_HOST_ERROR, ///< @brief Socket join was rejected due to an unreachable host
    CCO_FAIL_SOCKET_KICKED_ERROR,           ///< @brief Socket was kicked forcefully from a host
    CCO_FAIL_SOCKET_BROKEN_PIPE_ERROR,      ///< @brief Socket attempted to send data to a socket that was closed
    CCO_FAIL_SOCKET_UNCONNECTED_ERROR,      ///< @brief Socket attempted to perform operation that requires them to be
                                            ///< connected
    CCO_FAIL_SOCKET_CONNECTED_ERROR,        ///< @brief Socket attempted to perform operation that requires them to be
                                            ///< unconnected
} cco_result;