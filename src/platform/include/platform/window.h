//
// Created by fightinghawks18 on 12/19/25.
//

#pragma once

#include "core/core_types.h"
#include "export.h"

typedef struct cco_window_t cco_window_t;
typedef cco_window_t *cco_window;

typedef struct {
    i32 x, y, width, height;
} cco_window_frame;

typedef struct {
    i32 width, height;
} cco_window_content_size;

/// @brief Starts the windowing system
/// @return CCO_SUCCESS if initialization succeeded, or an error result if it failed
CCO_PLATFORM_API cco_result cco_windowing_init(void);

/// @brief Cleans up the windowing system
CCO_PLATFORM_API void cco_windowing_quit(void);

/// @brief Creates a new window
/// @param x The x position this window should start at
/// @param y The y position this window should start at
/// @param width The width this window should start with
/// @param height The height this window should start with
/// @param title The name this window should start with
/// @param out_window The created window, or CCO_NIL if creation failed
/// @return CCO_SUCCESS if creation succeeded, or an error result if it failed
CCO_PLATFORM_API cco_result cco_create_window(i32 x, i32 y, i32 width, i32 height, const char *title, cco_window *out_window);

/// @brief Destroys a window
/// @param window The window to destroy
CCO_PLATFORM_API void cco_destroy_window(cco_window window);

/// @brief Moves a window
/// @param window The window to move
/// @param x The new x position this window should use
/// @param y The new y position this window should use
CCO_PLATFORM_API void cco_window_move(cco_window window, i32 x, i32 y);

/// @brief Changes the size of a window
/// @param window The window to resize
/// @param width The new width this window should use
/// @param height The new height this window should use
CCO_PLATFORM_API void cco_window_resize(cco_window window, i32 width, i32 height);

/// @brief Changes the name of a window
/// @param window The window to change the title for
/// @param title The name that this window should use
CCO_PLATFORM_API void cco_window_rename(cco_window window, const char *title);

/// @brief Finds and processes any events related to this window
/// @param window The window to process events for
CCO_PLATFORM_API void cco_window_pump_events(cco_window window);

/// @brief Gets the dimensions of the entire window
/// @param window The window to retrieve the frame dimensions from
/// @return The window's dimensions
CCO_PLATFORM_API cco_window_frame cco_window_get_frame(cco_window window);

/// @brief Gets the size of the area that content is displayed on a window
/// @param window The window to retrieve the content area size from
/// @return The window's content area size
CCO_PLATFORM_API cco_window_content_size cco_window_get_content_size(cco_window window);

/// @brief Gets the platform-specific handle to this window
/// @param window The window to retrieve the handle from
/// @return The window's handle
CCO_PLATFORM_API void *cco_window_get_handle(cco_window window);

/// @brief Checks if the window has been marked for closure
/// @param window The window to check if it wants to close
/// @return CCO_YES if this window has been marked for closure, or CCO_NO if it hasn't
CCO_PLATFORM_API cco_bool cco_window_will_close(cco_window window);

/// @brief Checks if the window is currently being interacted with by the user
/// @param window The window to check for focus
/// @return CCO_YES if the user is interacting with it, or CCO_NO if they aren't
CCO_PLATFORM_API cco_bool cco_window_is_focus(cco_window window);