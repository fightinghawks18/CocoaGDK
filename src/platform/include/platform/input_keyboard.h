//
// Created by fightinghawks18 on 12/30/2025.
//

#pragma once

#define KEY_COUNT 106

#include "export.h"
#include "core/core_types.h"

typedef enum {
    CCO_INPUT_KEY_UNKNOWN = 0,

    CCO_INPUT_KEY_ESC,
    CCO_INPUT_KEY_F1,
    CCO_INPUT_KEY_F2,
    CCO_INPUT_KEY_F3,
    CCO_INPUT_KEY_F4,
    CCO_INPUT_KEY_F5,
    CCO_INPUT_KEY_F6,
    CCO_INPUT_KEY_F7,
    CCO_INPUT_KEY_F8,
    CCO_INPUT_KEY_F9,
    CCO_INPUT_KEY_F10,
    CCO_INPUT_KEY_F11,
    CCO_INPUT_KEY_F12,
    CCO_INPUT_KEY_SCROLL_LOCK,
    CCO_INPUT_KEY_MEDIA_MUTE,
    CCO_INPUT_KEY_MEDIA_VOLUME_UP,
    CCO_INPUT_KEY_MEDIA_VOLUME_DOWN,
    CCO_INPUT_KEY_MEDIA_PLAY_PAUSE,
    CCO_INPUT_KEY_MEDIA_STOP,
    CCO_INPUT_KEY_MEDIA_PREVIOUS_TRACK,
    CCO_INPUT_KEY_MEDIA_NEXT_TRACK,

    CCO_INPUT_KEY_GRAVE, ///< @brief The ` key, produces a tilde ( ~ ) when holding shift
    CCO_INPUT_KEY_1,     ///< @note Produces an exclamation-mark ( ! ) when holding shift
    CCO_INPUT_KEY_2,     ///< @note Produces an at-symbol ( @ ) when holding shift
    CCO_INPUT_KEY_3,     ///< @note Produces a number-sign ( # ) when holding shift
    CCO_INPUT_KEY_4,     ///< @note Produces a dollar-sign ( $ ) when holding shift
    CCO_INPUT_KEY_5,     ///< @note Produces a percent ( % ) when holding shift
    CCO_INPUT_KEY_6,     ///< @note Produces a caret ( ^ ) when holding shift
    CCO_INPUT_KEY_7,     ///< @note Produces an and-symbol ( & ) when holding shift
    CCO_INPUT_KEY_8,     ///< @note Produces an asterisk ( * ) when holding shift
    CCO_INPUT_KEY_9,     ///< @note Produces a left parenthesis [ ( ] when holding shift
    CCO_INPUT_KEY_0,     ///< @note Produces a right parenthesis [ ) ] when holding shift
    CCO_INPUT_KEY_MINUS, ///< @brief The - key, produces an underscore ( _ ) when holding shift
    CCO_INPUT_KEY_EQUAL, ///< @brief The = key, produces a plus ( + ) when holding shift
    CCO_INPUT_KEY_BACKSPACE,
    CCO_INPUT_KEY_INSERT,  ///< @note Key may be symbolized with an Insert or Ins, separate from Numpad's Insert
    CCO_INPUT_KEY_HOME,    ///< @note This is separate from the Numpad's Home
    CCO_INPUT_KEY_PAGE_UP, ///< @note This is separate from the Numpad's Page Up

    CCO_INPUT_KEY_TAB,
    CCO_INPUT_KEY_Q,
    CCO_INPUT_KEY_W,
    CCO_INPUT_KEY_E,
    CCO_INPUT_KEY_R,
    CCO_INPUT_KEY_T,
    CCO_INPUT_KEY_Y,
    CCO_INPUT_KEY_U,
    CCO_INPUT_KEY_I,
    CCO_INPUT_KEY_O,
    CCO_INPUT_KEY_P,
    CCO_INPUT_KEY_LEFT_BRACKET,  ///< @brief The [ key, produces a left brace ( { ) when holding shift
    CCO_INPUT_KEY_RIGHT_BRACKET, ///< @brief The ] key, produces a right brace ( } ) when holding shift
    CCO_INPUT_KEY_LEFT_SLASH,    ///< @brief The \ key, produces a pipe ( | ) when holding shift
    CCO_INPUT_KEY_DELETE,        ///< @note Key may be symbolized with a Delete or Del, separate from Numpad's Delete
    CCO_INPUT_KEY_END,
    CCO_INPUT_KEY_PAGE_DOWN, ///< @note This is separate from the Numpad's Page Down

    CCO_INPUT_KEY_CAPS_LOCK,
    CCO_INPUT_KEY_A,
    CCO_INPUT_KEY_S,
    CCO_INPUT_KEY_D,
    CCO_INPUT_KEY_F,
    CCO_INPUT_KEY_G,
    CCO_INPUT_KEY_H,
    CCO_INPUT_KEY_J,
    CCO_INPUT_KEY_K,
    CCO_INPUT_KEY_L,
    CCO_INPUT_KEY_SEMICOLON,  ///< @brief The ; key, produces a colon ( : ) when holding shift
    CCO_INPUT_KEY_APOSTROPHE, ///< @brief The ' key, produces a double-quote ( " ) when holding shift
    CCO_INPUT_KEY_ENTER,

    CCO_INPUT_KEY_LEFT_SHIFT,
    CCO_INPUT_KEY_Z,
    CCO_INPUT_KEY_X,
    CCO_INPUT_KEY_C,
    CCO_INPUT_KEY_V,
    CCO_INPUT_KEY_B,
    CCO_INPUT_KEY_N,
    CCO_INPUT_KEY_M,
    CCO_INPUT_KEY_COMMA,       ///< @brief The , key, produces a left-angle ( < ) when holding shift
    CCO_INPUT_KEY_PERIOD,      ///< @brief The . key, produces a right-angle ( > ) when holding shift
    CCO_INPUT_KEY_RIGHT_SLASH, ///< @brief The / key, produces a question-mark ( ? ) when holding shift
    CCO_INPUT_KEY_RIGHT_SHIFT,

    CCO_INPUT_KEY_LEFT_CTRL,
    CCO_INPUT_KEY_SUPER, ///< @brief The left super key (Windows key on Windows, command key on Mac, or the super key
                         ///< for linux)
    CCO_INPUT_KEY_LEFT_ALT,
    CCO_INPUT_KEY_SPACE,
    CCO_INPUT_KEY_RIGHT_ALT,
    CCO_INPUT_KEY_MENU, ///< @note Keyboard symbols vary, may have a page or three lines
    CCO_INPUT_KEY_RIGHT_CTRL,

    CCO_INPUT_KEY_UP_ARROW,
    CCO_INPUT_KEY_LEFT_ARROW,
    CCO_INPUT_KEY_DOWN_ARROW,
    CCO_INPUT_KEY_RIGHT_ARROW,

    CCO_INPUT_KEY_NUMPAD_NUMLOCK,

    CCO_INPUT_KEY_NUMPAD_0, ///< @note Produces Insert when NumLock is off
    CCO_INPUT_KEY_NUMPAD_1, ///< @note Produces End when NumLock is off
    CCO_INPUT_KEY_NUMPAD_2, ///< @note Produces Down-Arrow when NumLock is off
    CCO_INPUT_KEY_NUMPAD_3, ///< @note Produces Page Down when NumLock is off
    CCO_INPUT_KEY_NUMPAD_4, ///< @note Produces Left-Arrow when NumLock is off
    CCO_INPUT_KEY_NUMPAD_5,
    CCO_INPUT_KEY_NUMPAD_6, ///< @note Produces Right-Arrow when NumLock is off
    CCO_INPUT_KEY_NUMPAD_7, ///< @note Produces Home when NumLock is off
    CCO_INPUT_KEY_NUMPAD_8, ///< @note Produces Up-Arrow when NumLock is off
    CCO_INPUT_KEY_NUMPAD_9, ///< @note Produces Page Up when NumLock is off

    CCO_INPUT_KEY_NUMPAD_PLUS,
    CCO_INPUT_KEY_NUMPAD_ENTER,
    CCO_INPUT_KEY_NUMPAD_MINUS,
    CCO_INPUT_KEY_NUMPAD_ASTERISK,
    CCO_INPUT_KEY_NUMPAD_RIGHT_SLASH,
} cco_input_key;

static inline const char *cco_input_key_as_text(cco_input_key key) {
    static const char *cco_input_key_to_string[] = {
        [CCO_INPUT_KEY_UNKNOWN] = "UNKNOWN",

        [CCO_INPUT_KEY_ESC] = "ESC",
        [CCO_INPUT_KEY_F1] = "F1",
        [CCO_INPUT_KEY_F2] = "F2",
        [CCO_INPUT_KEY_F3] = "F3",
        [CCO_INPUT_KEY_F4] = "F4",
        [CCO_INPUT_KEY_F5] = "F5",
        [CCO_INPUT_KEY_F6] = "F6",
        [CCO_INPUT_KEY_F7] = "F7",
        [CCO_INPUT_KEY_F8] = "F8",
        [CCO_INPUT_KEY_F9] = "F9",
        [CCO_INPUT_KEY_F10] = "F10",
        [CCO_INPUT_KEY_F11] = "F11",
        [CCO_INPUT_KEY_F12] = "F12",
        [CCO_INPUT_KEY_SCROLL_LOCK] = "SCROLL_LOCK",
        [CCO_INPUT_KEY_MEDIA_MUTE] = "MEDIA_MUTE",
        [CCO_INPUT_KEY_MEDIA_VOLUME_UP] = "MEDIA_VOLUME_UP",
        [CCO_INPUT_KEY_MEDIA_VOLUME_DOWN] = "MEDIA_VOLUME_DOWN",
        [CCO_INPUT_KEY_MEDIA_PLAY_PAUSE] = "MEDIA_PLAY_PAUSE",
        [CCO_INPUT_KEY_MEDIA_STOP] = "MEDIA_STOP",
        [CCO_INPUT_KEY_MEDIA_PREVIOUS_TRACK] = "MEDIA_PREVIOUS_TRACK",
        [CCO_INPUT_KEY_MEDIA_NEXT_TRACK] = "MEDIA_NEXT_TRACK",

        [CCO_INPUT_KEY_GRAVE] = "GRAVE",
        [CCO_INPUT_KEY_1] = "1",
        [CCO_INPUT_KEY_2] = "2",
        [CCO_INPUT_KEY_3] = "3",
        [CCO_INPUT_KEY_4] = "4",
        [CCO_INPUT_KEY_5] = "5",
        [CCO_INPUT_KEY_6] = "6",
        [CCO_INPUT_KEY_7] = "7",
        [CCO_INPUT_KEY_8] = "8",
        [CCO_INPUT_KEY_9] = "9",
        [CCO_INPUT_KEY_0] = "0",
        [CCO_INPUT_KEY_MINUS] = "MINUS",
        [CCO_INPUT_KEY_EQUAL] = "EQUAL",
        [CCO_INPUT_KEY_BACKSPACE] = "BACKSPACE",
        [CCO_INPUT_KEY_INSERT] = "INSERT",
        [CCO_INPUT_KEY_HOME] = "HOME",
        [CCO_INPUT_KEY_PAGE_UP] = "PAGE_UP",

        [CCO_INPUT_KEY_TAB] = "TAB",
        [CCO_INPUT_KEY_Q] = "Q",
        [CCO_INPUT_KEY_W] = "W",
        [CCO_INPUT_KEY_E] = "E",
        [CCO_INPUT_KEY_R] = "R",
        [CCO_INPUT_KEY_T] = "T",
        [CCO_INPUT_KEY_Y] = "Y",
        [CCO_INPUT_KEY_U] = "U",
        [CCO_INPUT_KEY_I] = "I",
        [CCO_INPUT_KEY_O] = "O",
        [CCO_INPUT_KEY_P] = "P",
        [CCO_INPUT_KEY_LEFT_BRACKET] = "LEFT_BRACKET",
        [CCO_INPUT_KEY_RIGHT_BRACKET] = "RIGHT_BRACKET",
        [CCO_INPUT_KEY_LEFT_SLASH] = "BACKSLASH",
        [CCO_INPUT_KEY_DELETE] = "DELETE",
        [CCO_INPUT_KEY_END] = "END",
        [CCO_INPUT_KEY_PAGE_DOWN] = "PAGE_DOWN",

        [CCO_INPUT_KEY_CAPS_LOCK] = "CAPS_LOCK",
        [CCO_INPUT_KEY_A] = "A",
        [CCO_INPUT_KEY_S] = "S",
        [CCO_INPUT_KEY_D] = "D",
        [CCO_INPUT_KEY_F] = "F",
        [CCO_INPUT_KEY_G] = "G",
        [CCO_INPUT_KEY_H] = "H",
        [CCO_INPUT_KEY_J] = "J",
        [CCO_INPUT_KEY_K] = "K",
        [CCO_INPUT_KEY_L] = "L",
        [CCO_INPUT_KEY_SEMICOLON] = "SEMICOLON",
        [CCO_INPUT_KEY_APOSTROPHE] = "APOSTROPHE",
        [CCO_INPUT_KEY_ENTER] = "ENTER",

        [CCO_INPUT_KEY_LEFT_SHIFT] = "LEFT_SHIFT",
        [CCO_INPUT_KEY_Z] = "Z",
        [CCO_INPUT_KEY_X] = "X",
        [CCO_INPUT_KEY_C] = "C",
        [CCO_INPUT_KEY_V] = "V",
        [CCO_INPUT_KEY_B] = "B",
        [CCO_INPUT_KEY_N] = "N",
        [CCO_INPUT_KEY_M] = "M",
        [CCO_INPUT_KEY_COMMA] = "COMMA",
        [CCO_INPUT_KEY_PERIOD] = "PERIOD",
        [CCO_INPUT_KEY_RIGHT_SLASH] = "FORWARD_SLASH",
        [CCO_INPUT_KEY_RIGHT_SHIFT] = "RIGHT_SHIFT",

        [CCO_INPUT_KEY_LEFT_CTRL] = "LEFT_CTRL",
        [CCO_INPUT_KEY_SUPER] = "SUPER",
        [CCO_INPUT_KEY_LEFT_ALT] = "LEFT_ALT",
        [CCO_INPUT_KEY_SPACE] = "SPACE",
        [CCO_INPUT_KEY_RIGHT_ALT] = "RIGHT_ALT",
        [CCO_INPUT_KEY_MENU] = "MENU",
        [CCO_INPUT_KEY_RIGHT_CTRL] = "RIGHT_CTRL",

        [CCO_INPUT_KEY_UP_ARROW] = "UP_ARROW",
        [CCO_INPUT_KEY_LEFT_ARROW] = "LEFT_ARROW",
        [CCO_INPUT_KEY_DOWN_ARROW] = "DOWN_ARROW",
        [CCO_INPUT_KEY_RIGHT_ARROW] = "RIGHT_ARROW",

        [CCO_INPUT_KEY_NUMPAD_NUMLOCK] = "NUMPAD_NUMLOCK",
        [CCO_INPUT_KEY_NUMPAD_0] = "NUMPAD_0",
        [CCO_INPUT_KEY_NUMPAD_1] = "NUMPAD_1",
        [CCO_INPUT_KEY_NUMPAD_2] = "NUMPAD_2",
        [CCO_INPUT_KEY_NUMPAD_3] = "NUMPAD_3",
        [CCO_INPUT_KEY_NUMPAD_4] = "NUMPAD_4",
        [CCO_INPUT_KEY_NUMPAD_5] = "NUMPAD_5",
        [CCO_INPUT_KEY_NUMPAD_6] = "NUMPAD_6",
        [CCO_INPUT_KEY_NUMPAD_7] = "NUMPAD_7",
        [CCO_INPUT_KEY_NUMPAD_8] = "NUMPAD_8",
        [CCO_INPUT_KEY_NUMPAD_9] = "NUMPAD_9",
        [CCO_INPUT_KEY_NUMPAD_PLUS] = "NUMPAD_PLUS",
        [CCO_INPUT_KEY_NUMPAD_ENTER] = "NUMPAD_ENTER",
        [CCO_INPUT_KEY_NUMPAD_MINUS] = "NUMPAD_MINUS",
        [CCO_INPUT_KEY_NUMPAD_ASTERISK] = "NUMPAD_ASTERISK",
        [CCO_INPUT_KEY_NUMPAD_RIGHT_SLASH] = "NUMPAD_SLASH",
    };
    return cco_input_key_to_string[key];
}

/// @brief Checks if a specific keyboard key is being pressed
/// @return CCO_YES if the key is being pressed, or CCO_NO if it isn't
CCO_PLATFORM_API cco_bool cco_input_key_is_pressed(cco_input_key key);

/// @brief Checks if a specific keyboard key was pressed this frame
/// @return CCO_YES if the key was pressed this frame, or CCO_NO if it wasn't
CCO_PLATFORM_API cco_bool cco_input_key_was_just_pressed(cco_input_key key);

/// @brief Checks if a specific keyboard key was released this frame
/// @return CCO_YES if the button was released this frame, or CCO_NO if it wasn't
CCO_PLATFORM_API cco_bool cco_input_key_was_just_released(cco_input_key key);