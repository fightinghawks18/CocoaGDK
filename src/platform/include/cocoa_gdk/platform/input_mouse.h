//
// Created by fightinghawks18 on 12/30/2025.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define MOUSE_BUTTON_COUNT 5

#include "export.h"
#include "cocoa_gdk/core/core_types.h"

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

/// @brief Gets the change in mouse state between last frame and this frame
/// @return cco_mouse_delta
/// @see cco_mouse_delta
CCO_PLATFORM_API cco_mouse_delta cco_input_get_mouse_delta(void);

/// @brief Gets the current position of the mouse
/// @return cco_mouse_point
/// @see cco_mouse_point
CCO_PLATFORM_API cco_mouse_point cco_input_get_mouse_point(void);

/// @brief Checks if a specific mouse button is being pressed
/// @return CCO_YES if the button is being pressed, or CCO_NO if it isn't
CCO_PLATFORM_API cco_bool cco_input_mouse_button_is_pressed(cco_input_mouse_button button);

/// @brief Checks if a specific mouse button was pressed this frame
/// @return CCO_YES if the button was pressed this frame, or CCO_NO if it wasn't
CCO_PLATFORM_API cco_bool cco_input_mouse_button_was_just_pressed(cco_input_mouse_button button);

/// @brief Checks if a specific mouse button was released this frame
/// @return CCO_YES if the button was released this frame, or CCO_NO if it wasn't
CCO_PLATFORM_API cco_bool cco_input_mouse_button_was_just_released(cco_input_mouse_button button);

#ifdef __cplusplus
}
#endif