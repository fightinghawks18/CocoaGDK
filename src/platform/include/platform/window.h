//
// Created by fightinghawks18 on 12/19/25.
//

#pragma once

#include "core/core_types.h"

typedef struct CcoWindow_T CcoWindow_T;
typedef CcoWindow_T *CcoWindow;

typedef enum {
    CCO_WINDOW_EVENT_QUIT,
    CCO_WINDOW_EVENT_INPUT
} CcoWindowEventType;

typedef struct {
    CcoWindowEventType event;
    CcoWindow window;
} CcoWindowEvent;

typedef struct {
    i32 x, y, width, height;
} CcoWindowFrame;

typedef struct {
    i32 width, height;
} CcoWindowContentSize;

CcoResult ccoWindowingInit();
void ccoWindowingQuit();

CcoResult ccoCreateWindow(i32 x, i32 y, i32 width, i32 height, const char *title, CcoWindow *outWindow);
void ccoDestroyWindow(CcoWindow window);

void ccoWindowMove(CcoWindow window, i32 x, i32 y);
void ccoWindowResize(CcoWindow window, i32 width, i32 height);
void ccoWindowRename(CcoWindow window, const char *title);
void ccoWindowPumpEvents(CcoWindow window);

CcoWindowFrame ccoWindowGetFrame(CcoWindow window);
CcoWindowContentSize ccoWindowGetContentSize(CcoWindow window);
void *ccoWindowGetHandle(CcoWindow window);
CcoBool ccoWindowWillClose(CcoWindow window);
CcoBool ccoWindowIsFocus(CcoWindow window);