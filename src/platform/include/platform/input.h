//
// Created by fightinghawks18 on 12/20/25.
//

#pragma once

#include "core/core_types.h"
#include "window.h"

typedef enum {
    CCO_INPUT_KEY_W,
    CCO_INPUT_KEY_A,
    CCO_INPUT_KEY_S,
    CCO_INPUT_KEY_D,

    CCO_INPUT_KEY_SPACE,
    CCO_INPUT_KEY_CTRL,
    CCO_INPUT_KEY_ESC,
} CcoInputKey;

typedef enum {
    CCO_INPUT_MOUSE_BUTTON_LEFT,
    CCO_INPUT_MOUSE_BUTTON_RIGHT,
    CCO_INPUT_MOUSE_BUTTON_3,
    CCO_INPUT_MOUSE_BUTTON_4,
    CCO_INPUT_MOUSE_BUTTON_5,
} CcoInputMouseButton;

/// @brief Change in mouse position between last frame and this frame
typedef struct {
    f32 x, y;
} CcoMouseDelta;

/// @brief Mouse position
typedef struct {
    i32 x, y;
} CcoMousePoint;

/// @brief Starts the input listener and begins listening for input
/// @return CcoResult
CcoResult ccoInputInit(void);

/// @brief Closes the input listener
void ccoInputQuit(void);

/// @brief Accesses and caches input state
void ccoInputPoll(void);

/// @brief Clears input per-frame state
void ccoInputClearFrameState(void);

/// @brief Clears input hardware state
/// @note The hardware state is what is currently being processed before being cached after polling
void ccoInputClearHardwareState(void);

/// @brief Gives input focus to a new window
void ccoInputGiveWindowFocus(CcoWindow window);

/// @brief Enables input listening
void ccoInputEnable(void);

/// @brief Disables input listening
void ccoInputDisable(void);

/// @brief Gets the current window that is taking input
/// @return CcoWindow, or CCO_NIL if there is no active window
/// @see CcoWindow
CcoWindow ccoInputGetActiveWindow(void);

/// @brief Gets the change in position between last frame and this frame for the mouse
/// @return CcoMouseDelta
/// @see CcoMouseDelta
CcoMouseDelta ccoInputGetMouseDelta(void);

/// @brief Gets the current position of the mouse
/// @return CcoMousePoint
/// @see CcoMousePoint
CcoMousePoint ccoInputGetMousePoint(void);

/// @brief Checks if a specific keyboard key is being pressed
/// @return CCO_YES if the key is being pressed, or CCO_NO if it isn't
CcoBool ccoInputKeyIsPressed(CcoInputKey key);

/// @brief Checks if a specific keyboard key was pressed this frame
/// @return CCO_YES if the key was pressed this frame, or CCO_NO if it wasn't
CcoBool ccoInputKeyWasJustPressed(CcoInputKey key);

/// @brief Checks if a specific keyboard key was released this frame
/// @return CCO_YES if the button was released this frame, or CCO_NO if it wasn't
CcoBool ccoInputKeyWasJustReleased(CcoInputKey key);

/// @brief Checks if a specific mouse button is being pressed
/// @return CCO_YES if the button is being pressed, or CCO_NO if it isn't
CcoBool ccoInputMouseButtonIsPressed(CcoInputMouseButton button);

/// @brief Checks if a specific mouse button was pressed this frame
/// @return CCO_YES if the button was pressed this frame, or CCO_NO if it wasn't
CcoBool ccoInputMouseButtonWasJustPressed(CcoInputMouseButton button);

/// @brief Checks if a specific mouse button was released this frame
/// @return CCO_YES if the button was released this frame, or CCO_NO if it wasn't
CcoBool ccoInputMouseButtonWasJustReleased(CcoInputMouseButton button);