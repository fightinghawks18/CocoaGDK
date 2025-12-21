//
// Created by fightinghawks18 on 12/19/25.
//

#pragma once

#include "core/core_types.h"

typedef struct CcoWindow_T CcoWindow_T;
typedef CcoWindow_T *CcoWindow;

typedef struct {
    i32 x, y, width, height;
} CcoWindowFrame;

typedef struct {
    i32 width, height;
} CcoWindowContentSize;

/// @brief Starts the windowing system
/// @return CCO_SUCCESS if initialization succeeded, or an error result if it failed
CcoResult ccoWindowingInit(void);

/// @brief Cleans up the windowing system
void ccoWindowingQuit(void);

/// @brief Creates a new window
/// @param x The x position this window should start at
/// @param y The y position this window should start at
/// @param width The width this window should start with
/// @param height The height this window should start with
/// @param title The name this window should start with
/// @param outWindow The created window, or CCO_NIL if creation failed
/// @return CCO_SUCCESS if creation succeeded, or an error result if it failed
CcoResult ccoCreateWindow(i32 x, i32 y, i32 width, i32 height, const char *title, CcoWindow *outWindow);

/// @brief Destroys a window
/// @param window The window to destroy
void ccoDestroyWindow(CcoWindow window);

/// @brief Moves a window
/// @param window The window to move
/// @param x The new x position this window should use
/// @param y The new y position this window should use
void ccoWindowMove(CcoWindow window, i32 x, i32 y);

/// @brief Changes the size of a window
/// @param window The window to resize
/// @param width The new width this window should use
/// @param height The new height this window should use
void ccoWindowResize(CcoWindow window, i32 width, i32 height);

/// @brief Changes the name of a window
/// @param window The window to change the title for
/// @param title The name that this window should use
void ccoWindowRename(CcoWindow window, const char *title);

/// @brief Finds and processes any events related to this window
/// @param window The window to process events for
void ccoWindowPumpEvents(CcoWindow window);

/// @brief Gets the dimensions of the entire window
/// @param window The window to retrieve the frame dimensions from
/// @return The window's dimensions
CcoWindowFrame ccoWindowGetFrame(CcoWindow window);

/// @brief Gets the size of the area that content is displayed on a window
/// @param window The window to retrieve the content area size from
/// @return The window's content area size
CcoWindowContentSize ccoWindowGetContentSize(CcoWindow window);

/// @brief Gets the platform-specific handle to this window
/// @param window The window to retrieve the handle from
/// @return The window's handle
void *ccoWindowGetHandle(CcoWindow window);

/// @brief Checks if the window has been marked for closure
/// @param window The window to check if it wants to close
/// @return CCO_YES if this window has been marked for closure, or CCO_NO if it hasn't
CcoBool ccoWindowWillClose(CcoWindow window);

/// @brief Checks if the window is currently being interacted with by the user
/// @param window The window to check for focus
/// @return CCO_YES if the user is interacting with it, or CCO_NO if they aren't
CcoBool ccoWindowIsFocus(CcoWindow window);