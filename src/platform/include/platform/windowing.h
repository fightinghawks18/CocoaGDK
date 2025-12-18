//
// Created by fightinghawks18 on 12/12/25.
//

#pragma once

#include "core/core_types.h"

#define CCO_WINDOW_POS_CENTER 0xFFFF

typedef struct CcoWindow_T CcoWindow_T;
typedef CcoWindow_T *CcoWindow;
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

typedef struct {
    i32 x, y, w, h;
    const char *title;
    CcoWindowFlags flags;
} CcoWindowDesc;

typedef struct {
    i32 x, y, w, h;
} CcoWindowDimensions;

typedef struct {
    f64 w, h;
} CcoWindowFramebufferSize;

CcoResult ccoWindowingInit();
void ccoWindowingPoll();
void ccoWindowingQuit();

CcoResult ccoCreateWindow(const CcoWindowDesc *desc, CcoWindow *outWindow);
void ccoSetWindowShouldClose(CcoWindow window, CcoBool close);
void ccoCloseWindow(CcoWindow window);
CcoWindowNativeHandle ccoGetNativeWindowHandle(CcoWindow window);
CcoWindowDimensions ccoGetWindowDimensions(CcoWindow window);

/// @brief Returns the dimensions of this window with DPI accounted for
/// @note DPI introduces a scaling factor to screens which may not return
/// a true scale for framebuffers
/// @returns Window framebuffer size
CcoWindowFramebufferSize ccoGetWindowFramebufferSize(CcoWindow window);
CcoBool ccoShouldWindowClose(CcoWindow window);