//
// Created by fightinghawks18 on 12/15/2025.
//

#include "platform/windowing.h"

#include <windows.h>

static u32 DEFAULT_DPI_SCALE = 96;

typedef struct CcoWindow_T {
    HWND hWnd;
    bool shouldClose;
} CcoWindow_T;

LRESULT Wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    CcoWindow window = CCO_NULL_HANDLE;
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT *cs = (CREATESTRUCT *)lParam;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
        window = (CcoWindow)cs->lpCreateParams;
    } else {
        window = (CcoWindow)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (uMsg == WM_CLOSE) {
        window->shouldClose = true;
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

CcoResult ccoWindowingInit() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = &Wndproc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "CocoaWindow";
    if (!RegisterClass(&wc)) {
        CCO_LOG("Windows failed to create a new window class!");
        return CCO_FAIL_WINDOWING_INIT_ERROR;
    }

    return CCO_SUCCESS;
}

void ccoWindowingPoll() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
void ccoWindowingQuit() { PostQuitMessage(0); }

CcoResult ccoCreateWindow(const CcoWindowDesc *desc, CcoWindow *outWindow) {
    CcoWindow window = malloc(sizeof(CcoWindow_T));
    if (!window)
        return CCO_FAIL_OUT_OF_MEMORY;
    HWND hWnd = CreateWindowEx(0, "CocoaWindow", desc->title, WS_OVERLAPPEDWINDOW, desc->x, desc->y, desc->w, desc->h,
                               NULL, NULL, GetModuleHandle(NULL), window);
    if (!hWnd) {
        CCO_LOG("Windows failed to create window!");
        return CCO_FAIL_WINDOWING_CREATE_ERROR;
    }

    window->hWnd = hWnd;
    window->shouldClose = false;

    ShowWindow(window->hWnd, SW_SHOW);

    *outWindow = window;
    return CCO_SUCCESS;
}

void ccoSetWindowShouldClose(CcoWindow window, bool close) { window->shouldClose = close; }

void ccoCloseWindow(CcoWindow window) {
    if (IsWindow(window->hWnd)) {
        DestroyWindow(window->hWnd);
    }
    free(window);
}

CcoWindowNativeHandle ccoGetNativeWindowHandle(CcoWindow window) { return window->hWnd; }

CcoWindowDimensions ccoGetWindowDimensions(CcoWindow window) {
    RECT rect;
    GetWindowRect(window->hWnd, &rect);
    const i32 width = rect.right - rect.left;
    const i32 height = rect.bottom - rect.top;
    return (CcoWindowDimensions){rect.left, rect.top, width, height};
}

CcoWindowFramebufferSize ccoGetWindowFramebufferSize(CcoWindow window) {
    const u32 dpi = GetDpiForWindow(window->hWnd);
    const u32 dpiScale = dpi / DEFAULT_DPI_SCALE;

    RECT clientRect;
    GetClientRect(window->hWnd, &clientRect);

    const i32 logicalWidth = clientRect.right - clientRect.left;
    const i32 logicalHeight = clientRect.bottom - clientRect.top;
    const i32 width = (i32)(logicalWidth * dpiScale);
    const i32 height = (i32)(logicalHeight * dpiScale);

    return (CcoWindowFramebufferSize){width, height};
}

bool ccoShouldWindowClose(CcoWindow window) { return window->shouldClose; }