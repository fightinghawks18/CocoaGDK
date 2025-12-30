//
// Created by fightinghawks18 on 12/20/25.
//

#pragma once

#include "core/core_types.h"
#include "core/results.h"
#include "core/log.h"
#include "export.h"
#include "window.h"

typedef enum {
    CCO_INPUT_INIT_UNKNOWN_BIT,
    CCO_INPUT_INIT_KEYBOARD_BIT,
    CCO_INPUT_INIT_MOUSE_BIT
} cco_input_init_flag_bits;
typedef u32 cco_input_init_flags;

/// @brief Starts the input listener and begins listening for input
/// @return cco_result
CCO_PLATFORM_API cco_result cco_input_init(cco_input_init_flags init_flags);

/// @brief Closes the input listener
CCO_PLATFORM_API void cco_input_quit(void);

/// @brief Accesses and caches input state
CCO_PLATFORM_API void cco_input_poll(void);

/// @brief Clears input hardware state
/// @note The hardware state is what is currently being processed before being cached after polling
CCO_PLATFORM_API void cco_input_clear_hardware_state(void);

/// @brief Gives input focus to a new window
CCO_PLATFORM_API void cco_input_give_window_focus(cco_window window);

/// @brief Enables input listening
CCO_PLATFORM_API void cco_input_enable(void);

/// @brief Disables input listening
CCO_PLATFORM_API void cco_input_disable(void);

/// @brief Gets the current window that is taking input
/// @return CcoWindow, or CCO_NIL if there is no active window
/// @see CcoWindow
CCO_PLATFORM_API cco_window cco_input_get_active_window(void);