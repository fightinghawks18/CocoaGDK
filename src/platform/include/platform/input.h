//
// Created by fightinghawks18 on 12/18/25.
//

#pragma once

#include "core/core_types.h"
#include "windowing.h"

typedef enum {
    CCO_INPUT_KEY_NONE = 0,
    CCO_INPUT_KEY_W,
    CCO_INPUT_KEY_A,
    CCO_INPUT_KEY_S,
    CCO_INPUT_KEY_D,
    CCO_INPUT_KEY_SPACE,
    CCO_INPUT_KEY_CTRL,
    CCO_INPUT_KEY_ESC,
} CcoInputKey;

typedef enum {
    CCO_INPUT_MOUSE_KEY_M1,    ///< @brief Mouse1 (Left Click)
    CCO_INPUT_MOUSE_KEY_M2,    ///< @brief Mouse2 (Right Click)
    CCO_INPUT_MOUSE_KEY_M3,    ///< @brief Mouse3
    CCO_INPUT_MOUSE_KEY_M4,    ///< @brief Mouse4
    CCO_INPUT_MOUSE_KEY_M5,    ///< @brief Mouse5
} CcoInputMouseKey;

typedef struct {
    f32 x;
    f32 y;
    f32 wheel;
} CcoMouseDelta;

typedef struct {
    i32 x;
    i32 y;
} CcoMousePosition;

typedef struct CcoWindowInput_T CcoWindowInput_T;
typedef CcoWindowInput_T *CcoWindowInput;

CcoResult ccoCreateWindowInput(CcoWindow *window, CcoWindowInput *outInput);
void ccoDestroyWindowInput(CcoWindow *window, CcoWindowInput input);

void ccoUpdateWindowInput(CcoWindowInput input);

CcoMouseDelta ccoWindowInputGetMouseDelta(CcoWindowInput input);
CcoMousePosition ccoWindowInputGetMousePosition(CcoWindowInput input);
CcoBool ccoWindowInputKeyIsPressed(CcoWindowInput input, CcoInputKey key);
CcoBool ccoWindowInputKeyWasJustPressed(CcoWindowInput input, CcoInputKey key);
CcoBool ccoWindowInputKeyWasJustReleased(CcoWindowInput input, CcoInputKey key);
CcoBool ccoWindowInputMouseKeyIsPressed(CcoWindowInput input, CcoInputMouseKey mouseKey);
CcoBool ccoWindowInputMouseKeyWasJustPressed(CcoWindowInput input, CcoInputMouseKey mouseKey);
CcoBool ccoWindowInputMouseKeyWasJustReleased(CcoWindowInput input, CcoInputMouseKey mouseKey);