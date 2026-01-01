//
// Created by fightinghawks18 on 12/30/2025.
//

#pragma once

#include "core/core_types.h"
#include "core/results.h"
#include "export.h"

#define CCO_MAX_GAMEPAD_COUNT 8
#define CCO_GAMEPAD_BUTTON_COUNT 15

typedef struct cco_gamepad_t cco_gamepad_t;
typedef cco_gamepad_t *cco_gamepad;

typedef enum {
    CCO_GAMEPAD_BUTTON_UNKNOWN,
    CCO_GAMEPAD_BUTTON_LEFT,  ///< @note Square on PlayStation, X on Xbox
    CCO_GAMEPAD_BUTTON_UP,    ///< @note Triangle on PlayStation, Y on Xbox
    CCO_GAMEPAD_BUTTON_RIGHT, ///< @note Circle on PlayStation, B on Xbox
    CCO_GAMEPAD_BUTTON_DOWN,  ///< @note X on PlayStation, A on Xbox
    CCO_GAMEPAD_BUTTON_LEFT_BUMPER,
    CCO_GAMEPAD_BUTTON_RIGHT_BUMPER,
    CCO_GAMEPAD_BUTTON_LEFT_STICK,
    CCO_GAMEPAD_BUTTON_RIGHT_STICK,
    CCO_GAMEPAD_BUTTON_LEFT_DPAD,
    CCO_GAMEPAD_BUTTON_RIGHT_DPAD,
    CCO_GAMEPAD_BUTTON_UP_DPAD,
    CCO_GAMEPAD_BUTTON_DOWN_DPAD,
    CCO_GAMEPAD_BUTTON_MENU, ///< @note Options button on PlayStation, Menu button on Xbox (≡)
    CCO_GAMEPAD_BUTTON_VIEW, ///< @note Share button on PlayStation, View button on Xbox (⧉)
} cco_gamepad_button;

typedef struct {
    f32 x;       ///< @brief The stick's position on the X axis
    f32 y;       ///< @brief The stick's position on the Y axis
    f32 x_delta; ///< @brief The stick's change in X position since last frame
    f32 y_delta; ///< @brief The stick's change in Y position since last frame
} cco_gamepad_stick_pos;

typedef enum { CCO_GAMEPAD_STICK_LEFT, CCO_GAMEPAD_STICK_RIGHT } cco_gamepad_stick;

typedef enum { CCO_GAMEPAD_TRIGGER_LEFT, CCO_GAMEPAD_TRIGGER_RIGHT } cco_gamepad_trigger;

typedef u8 cco_gamepad_id; ///< @brief The ID representing the order of which this gamepad was connected

/// @brief Checks if the gamepad is pressing the button specified currently
/// @param gamepad_id The gamepad id to check
/// @param button The button to check if pressed
/// @return CCO_YES if the button specified is currently pressed, or CCO_NO if it isn't
/// @warning This method may return CCO_NO if the gamepad doesn't exist
CCO_PLATFORM_API cco_bool cco_input_is_gamepad_button_pressed(cco_gamepad_id gamepad_id, cco_gamepad_button button);

/// @brief Checks if the gamepad just released the button specified
/// @param gamepad_id The gamepad id to check
/// @param button The button to check if it was just released
/// @return CCO_YES if the button specified was just released, or CCO_NO if it isn't
/// @warning This method may return CCO_NO if the gamepad doesn't exist
CCO_PLATFORM_API cco_bool cco_input_was_gamepad_button_just_released(cco_gamepad_id gamepad_id,
                                                                     cco_gamepad_button button);

/// @brief Checks if the gamepad just pressed the button specified
/// @param gamepad_id The gamepad id to check
/// @param button The button to check if it was just pressed
/// @return CCO_YES if the button specified was just pressed, or CCO_NO if it isn't
/// @warning This method may return CCO_NO if the gamepad doesn't exist
CCO_PLATFORM_API cco_bool cco_input_was_gamepad_button_just_pressed(cco_gamepad_id gamepad_id,
                                                                    cco_gamepad_button button);

/// @brief Retrieves the position of a specific stick from the gamepad
/// @param gamepad_id The gamepad id to retrieve stick values from
/// @param stick The stick to retrieve position from
/// @param out_data The stick position accessed from the gamepad
/// @return CCO_YES if gamepad stick position was accessed successfully, or CCO_NO if the gamepad doesn't exist
/// @note Both stick directions are clamped between -1..1
CCO_PLATFORM_API cco_bool cco_input_read_gamepad_stick_pos(cco_gamepad_id gamepad_id, cco_gamepad_stick stick,
                                                           cco_gamepad_stick_pos *out_data);

/// @brief Retrieves the amount of pressure put onto a specific trigger from the gamepad
/// @param gamepad_id The gamepad id to retrieve trigger pressure from
/// @param trigger The trigger to retrieve pressure from
/// @param out_pressure The amount of pressure put onto a trigger accessed from the gamepad
/// @return CCO_YES if gamepad trigger pressure was accessed successfully, or CCO_NO if the gamepad doesn't exist
/// @note 0.0 means no pressure, 1.0 means fully pushed down
CCO_PLATFORM_API cco_bool cco_input_read_gamepad_trigger_pressure(cco_gamepad_id gamepad_id,
                                                                  cco_gamepad_trigger trigger, f32 *out_pressure);

/// @brief Checks if the amount of pressure put onto a specific trigger from the gamepad passes the pressure threshold
/// @param gamepad_id The gamepad id to compare trigger pressure from
/// @param trigger The trigger to compare pressure from
/// @param threshold The max amount of pressure that can be put onto the trigger without being considered "pressed"
/// @note 0.0 means no pressure, 1.0 means fully pushed down
/// @warning This method may return CCO_NO if the gamepad doesn't exist
CCO_PLATFORM_API cco_bool cco_input_is_gamepad_trigger_pressed(cco_gamepad_id gamepad_id, cco_gamepad_trigger trigger,
                                                               f32 threshold);

/// @brief Disables input listening for this gamepad
/// @param gamepad_id The gamepad id to disable input listening for
/// @warning This method may fail if the gamepad doesn't exist
CCO_PLATFORM_API void cco_input_disable_gamepad(cco_gamepad_id gamepad_id);

/// @brief Enables input listening for this gamepad
/// @param gamepad_id The gamepad id to enable input listening for
/// @warning This method may fail if the gamepad doesn't exist
CCO_PLATFORM_API void cco_input_enable_gamepad(cco_gamepad_id gamepad_id);

/// @brief Checks if there is an active gamepad with the id
/// @param gamepad_id The gamepad id to check for
/// @return CCO_YES if a gamepad is connected, or CCO_NO if there isn't
CCO_PLATFORM_API cco_bool cco_input_has_gamepad(cco_gamepad_id gamepad_id);

/// @brief Returns the number of currently connected gamepads
/// @return The number of connected gamepads
CCO_PLATFORM_API u8 cco_input_get_connected_gamepad_count(void);