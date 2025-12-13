//
// Created by fightinghawks18 on 12/12/25.
//

#pragma once

#include "../core.h"

#define CCO_WINDOW_SIZE_FILL 0
#define CCO_WINDOW_POS_CENTER 0

typedef struct CcoWindow_T CcoWindow_T;
typedef struct CcoWindow_T *CcoWindow;
typedef void *CcoWindowNativeHandle;

typedef u32 CcoWindowFlags;
typedef enum {
    CCO_WINDOW_FLAG_NONE_BIT = 0,
    CCO_WINDOW_FLAG_RESIZE_BIT = 1 << 0, ///< @brief Allows the window to be resized
    CCO_WINDOW_FLAG_DECOR_BIT = 1 << 1,  ///< @brief Enables OS decoration onto this window
} CcoWindowFlagBits;

typedef enum {
    CCO_WINDOWING_EVENT_UPDATE,
    CCO_WINDOWING_EVENT_QUIT,
} CcoWindowingEventType;

typedef enum {
    CCO_INPUT_KEY_NONE = 0,
    CCO_INPUT_KEY_W,
    CCO_INPUT_KEY_A,
    CCO_INPUT_KEY_S,
    CCO_INPUT_KEY_D,
    CCO_INPUT_KEY_SPACE,
    CCO_INPUT_KEY_LCTRL, ///< @brief Left Control
    CCO_INPUT_KEY_RCTRL, ///< @brief Right Control
    CCO_INPUT_KEY_ESC,   ///< @brief Escape
    CCO_INPUT_KEY_M1,    ///< @brief Mouse1 (Left Click)
    CCO_INPUT_KEY_M2,    ///< @brief Mouse2 (Right Click)
    CCO_INPUT_KEY_M3,    ///< @brief Mouse3
    CCO_INPUT_KEY_M4,    ///< @brief Mouse4
    CCO_INPUT_KEY_M5,    ///< @brief Mouse5
} CcoInputKey;

typedef struct {
    i32 x, y, w, h;
    const char *title;
    CcoWindowFlags flags;
} CcoWindowDesc;

CcoResult ccoWindowingInit();
void ccoWindowingPoll();
void ccoWindowingQuit();

CcoResult ccoCreateWindow(const CcoWindowDesc *desc, CcoWindow *outWindow);
void ccoSetWindowShouldClose(CcoWindow window, bool close);
void ccoCloseWindow(CcoWindow window);
bool ccoShouldWindowClose(CcoWindow window);