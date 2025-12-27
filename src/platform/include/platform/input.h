//
// Created by fightinghawks18 on 12/20/25.
//

#pragma once

#include "core/core_types.h"
#include "core/log.h"
#include "window.h"
#include "export.h"

#define KEY_COUNT 10
#define MOUSE_BUTTON_COUNT 5

typedef enum {
    CCO_INPUT_KEY_W,
    CCO_INPUT_KEY_A,
    CCO_INPUT_KEY_S,
    CCO_INPUT_KEY_D,

    CCO_INPUT_KEY_SPACE,
    CCO_INPUT_KEY_CTRL,
    CCO_INPUT_KEY_ESC,
} cco_input_key;

typedef enum {
    CCO_INPUT_MOUSE_BUTTON_LEFT,
    CCO_INPUT_MOUSE_BUTTON_RIGHT,
    CCO_INPUT_MOUSE_BUTTON_3,
    CCO_INPUT_MOUSE_BUTTON_4,
    CCO_INPUT_MOUSE_BUTTON_5,
} cco_input_mouse_button;

/// @brief Change in mouse state between last frame and the current frame
typedef struct {
    f32 x, y, wheel;
} cco_mouse_delta;

/// @brief Mouse position
typedef struct {
    i32 x, y;
} cco_mouse_point;

/// @brief Starts the input listener and begins listening for input
/// @return cco_result
CCO_PLATFORM_API cco_result cco_input_init(void);

/// @brief Closes the input listener
CCO_PLATFORM_API void cco_input_quit(void);

/// @brief Accesses and caches input state
CCO_PLATFORM_API void cco_input_poll(void);

/// @brief Clears input per-frame state
CCO_PLATFORM_API void cco_input_clear_frame_state(void);

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

/// @brief Gets the change in mouse state between last frame and this frame
/// @return cco_mouse_delta
/// @see cco_mouse_delta
CCO_PLATFORM_API cco_mouse_delta cco_input_get_mouse_delta(void);

/// @brief Gets the current position of the mouse
/// @return cco_mouse_point
/// @see cco_mouse_point
CCO_PLATFORM_API cco_mouse_point cco_input_get_mouse_point(void);

/// @brief Checks if a specific keyboard key is being pressed
/// @return CCO_YES if the key is being pressed, or CCO_NO if it isn't
CCO_PLATFORM_API cco_bool cco_input_key_is_pressed(cco_input_key key);

/// @brief Checks if a specific keyboard key was pressed this frame
/// @return CCO_YES if the key was pressed this frame, or CCO_NO if it wasn't
CCO_PLATFORM_API cco_bool cco_input_key_was_just_pressed(cco_input_key key);

/// @brief Checks if a specific keyboard key was released this frame
/// @return CCO_YES if the button was released this frame, or CCO_NO if it wasn't
CCO_PLATFORM_API cco_bool cco_input_key_was_just_released(cco_input_key key);

/// @brief Checks if a specific mouse button is being pressed
/// @return CCO_YES if the button is being pressed, or CCO_NO if it isn't
CCO_PLATFORM_API cco_bool cco_input_mouse_button_is_pressed(cco_input_mouse_button button);

/// @brief Checks if a specific mouse button was pressed this frame
/// @return CCO_YES if the button was pressed this frame, or CCO_NO if it wasn't
CCO_PLATFORM_API cco_bool cco_input_mouse_button_was_just_pressed(cco_input_mouse_button button);

/// @brief Checks if a specific mouse button was released this frame
/// @return CCO_YES if the button was released this frame, or CCO_NO if it wasn't
CCO_PLATFORM_API cco_bool cco_input_mouse_button_was_just_released(cco_input_mouse_button button);