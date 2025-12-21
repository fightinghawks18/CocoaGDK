//
// Created by fightinghawks18 on 12/19/25.
//

#include "platform/win32/win32_input.h"
#include "platform/window.h"
#include <windows.h>

static const i32 BASE_DPI = 96;

struct CcoWindow_T {
    HWND hWnd;
    CcoBool willClose;
    CcoBool focused;
};

static i32 scaleFromDPI(const i32 logicalAxis, const i32 dpi) { return MulDiv(logicalAxis, dpi, BASE_DPI); }

LRESULT CALLBACK Wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    CcoWindow window = CCO_NIL;
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT *createStruct = (CREATESTRUCT *)lParam;
        window = (CcoWindow)createStruct->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
    } else {
        window = (CcoWindow)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    switch (uMsg) {
    case WM_DPICHANGED: {
        i32 newDpi = HIWORD(wParam);
        const RECT *newRect = (RECT *)lParam;

        const i32 width = newRect->right - newRect->left;
        const i32 height = newRect->bottom - newRect->top;

        SetWindowPos(hWnd, NULL, newRect->left, newRect->top, width, height, SWP_NOZORDER | SWP_NOACTIVATE);

        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    case WM_SETCURSOR: {
        if (LOWORD(lParam) == HTCLIENT) {
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return TRUE;
        }
        break;
    }
    case WM_INPUT: {
        ccoInputHandleRawInput(hWnd, lParam);
        return 0;
    }
    case WM_CLOSE: {
        window->willClose = CCO_YES;
        return 0;
    }
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

CcoResult ccoWindowingInit(void) {
    if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
        CCO_LOG("Failed to set DPI-awareness for win32!");
        return CCO_FAIL_WINDOWING_INIT_ERROR;
    }

    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpfnWndProc = &Wndproc;
    wc.lpszClassName = "CGDKWindow";

    if (!RegisterClassEx(&wc)) {
        CCO_LOG("Failed to register main win32 window class!");
        return CCO_FAIL_WINDOWING_INIT_ERROR;
    }
    return CCO_SUCCESS;
}

void ccoWindowingQuit(void) {
    PostQuitMessage(0);
    if (!UnregisterClass("CGDKWindow", GetModuleHandle(NULL)))
        CCO_LOG("Failed to unregister main win32 window class!");
}

CcoResult ccoCreateWindow(i32 x, i32 y, i32 width, i32 height, const char *title, CcoWindow *outWindow) {
    CcoWindow window = malloc(sizeof(CcoWindow_T));
    if (!window)
        return CCO_FAIL_OUT_OF_MEMORY;

    const u32 dpi = GetDpiForSystem();
    const i32 scaledWidth = scaleFromDPI(width, (i32)dpi);
    const i32 scaledHeight = scaleFromDPI(height, (i32)dpi);

    RECT rect = {0, 0, scaledWidth, scaledHeight};
    AdjustWindowRectExForDpi(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0, dpi);

    const i32 dpiAdjustedWidth = rect.right - rect.left;
    const i32 dpiAdjustedHeight = rect.bottom - rect.top;

    window->hWnd = NULL;
    window->willClose = CCO_NO;
    window->focused = CCO_YES;

    HWND hWnd = CreateWindowEx(0, "CGDKWindow", title, WS_OVERLAPPEDWINDOW, x, y, dpiAdjustedWidth, dpiAdjustedHeight,
                               NULL, NULL, GetModuleHandle(NULL), window);
    if (!hWnd) {
        CCO_LOG("Failed to create win32 window!");
        free(window);
        return CCO_FAIL_WINDOWING_CREATE_ERROR;
    }

    window->hWnd = hWnd;

    ShowWindow(hWnd, SW_SHOW);

    *outWindow = window;
    return CCO_SUCCESS;
}

void ccoDestroyWindow(CcoWindow window) {
    if (IsWindow(window->hWnd)) {
        DestroyWindow(window->hWnd);
        window->hWnd = NULL;
    }
    free(window);
}

void ccoWindowMove(CcoWindow window, i32 x, i32 y) {
    SetWindowPos(window->hWnd, NULL, x, y, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void ccoWindowResize(CcoWindow window, i32 width, i32 height) {
    const u32 dpi = GetDpiForWindow(window->hWnd);

    RECT rect = {0, 0, width, height};
    AdjustWindowRectExForDpi(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0, dpi);

    const i32 dpiAdjustedWidth = rect.right - rect.left;
    const i32 dpiAdjustedHeight = rect.bottom - rect.top;

    SetWindowPos(window->hWnd, NULL, 0, 0, dpiAdjustedWidth, dpiAdjustedHeight,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void ccoWindowRename(CcoWindow window, const char *title) { SetWindowText(window->hWnd, title); }

void ccoWindowPumpEvents(CcoWindow window) {
    MSG msg;
    while (PeekMessage(&msg, window->hWnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

CcoWindowFrame ccoWindowGetFrame(CcoWindow window) {
    RECT rect;
    GetWindowRect(window->hWnd, &rect);
    const i32 width = rect.right - rect.left;
    const i32 height = rect.bottom - rect.top;
    return (CcoWindowFrame){rect.left, rect.top, width, height};
}

CcoWindowContentSize ccoWindowGetContentSize(CcoWindow window) {
    RECT rect;
    GetClientRect(window->hWnd, &rect);
    const i32 width = rect.right - rect.left;
    const i32 height = rect.bottom - rect.top;
    return (CcoWindowContentSize){width, height};
}

void *ccoWindowGetHandle(CcoWindow window) { return window->hWnd; }
CcoBool ccoWindowWillClose(CcoWindow window) { return window->willClose; }
CcoBool ccoWindowIsFocus(CcoWindow window) { return window->focused; }