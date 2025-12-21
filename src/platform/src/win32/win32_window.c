//
// Created by fightinghawks18 on 12/19/25.
//

#include "platform/input.h"
#include "platform/win32/win32_input.h"
#include "platform/window.h"
#include <windows.h>

static const i32 BASE_DPI = 96;

struct CcoWindow_T {
    HWND hWnd;
    CcoBool will_close;
    CcoBool focused;
};

static i32 scale_from_dpi(const i32 logical_axis, const i32 dpi) { return MulDiv(logical_axis, dpi, BASE_DPI); }

LRESULT CALLBACK Wndproc(HWND hWnd, UINT u_msg, WPARAM wparam, LPARAM lparam) {
    CcoWindow window = CCO_NIL;
    if (u_msg == WM_NCCREATE) {
        CREATESTRUCT *create_struct = (CREATESTRUCT *)lparam;
        window = (CcoWindow)create_struct->lp_create_params;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
    } else {
        window = (CcoWindow)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    switch (u_msg) {
    case WM_DPICHANGED: {
        i32 new_dpi = HIWORD(wparam);
        const RECT *new_rect = (RECT *)lparam;

        const i32 width = new_rect->right - new_rect->left;
        const i32 height = new_rect->bottom - new_rect->top;

        SetWindowPos(hWnd, NULL, new_rect->left, new_rect->top, width, height, SWP_NOZORDER | SWP_NOACTIVATE);

        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    case WM_SETCURSOR: {
        if (LOWORD(lparam) == HTCLIENT) {
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return TRUE;
        }
        break;
    }
    case WM_INPUT: {
        cco_input_handle_raw_input(hWnd, lparam);
        return 0;
    }
    case WM_SETFOCUS: {
        cco_input_give_window_focus(window);
        break;
    }
    case WM_KILLFOCUS: {
        cco_input_give_window_focus(CCO_NIL);
        break;
    }
    case WM_CLOSE: {
        window->will_close = CCO_YES;
        return 0;
    }
    default:
        break;
    }

    return DefWindowProc(hWnd, u_msg, wparam, lparam);
}

cco_result cco_windowing_init(void) {
    if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
        CCO_LOG("Failed to set DPI-awareness for win32!");
        return CCO_FAIL_WINDOWING_INIT_ERROR;
    }

    WNDCLASSEX wc = {0};
    wc.cb_size = sizeof(WNDCLASSEX);
    wc.h_instance = GetModuleHandle(NULL);
    wc.lpfn_wnd_proc = &Wndproc;
    wc.lpsz_class_name = "CGDKWindow";

    if (!RegisterClassEx(&wc)) {
        CCO_LOG("Failed to register main win32 window class!");
        return CCO_FAIL_WINDOWING_INIT_ERROR;
    }
    return CCO_SUCCESS;
}

void cco_windowing_quit(void) {
    PostQuitMessage(0);
    if (!UnregisterClass("CGDKWindow", GetModuleHandle(NULL)))
        CCO_LOG("Failed to unregister main win32 window class!");
}

cco_result cco_create_window(i32 x, i32 y, i32 width, i32 height, const char *title, CcoWindow *out_window) {
    CcoWindow window = malloc(sizeof(CcoWindow_T));
    if (!window)
        return CCO_FAIL_OUT_OF_MEMORY;

    const u32 dpi = GetDpiForSystem();
    const i32 scaled_width = scale_from_dpi(width, (i32)dpi);
    const i32 scaled_height = scale_from_dpi(height, (i32)dpi);

    RECT rect = {0, 0, scaled_width, scaled_height};
    AdjustWindowRectExForDpi(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0, dpi);

    const i32 dpi_adjusted_width = rect.right - rect.left;
    const i32 dpi_adjusted_height = rect.bottom - rect.top;

    window->hWnd = NULL;
    window->will_close = CCO_NO;
    window->focused = CCO_YES;

    HWND hWnd = CreateWindowEx(0, "CGDKWindow", title, WS_OVERLAPPEDWINDOW, x, y, dpi_adjusted_width, dpi_adjusted_height,
                               NULL, NULL, GetModuleHandle(NULL), window);
    if (!hWnd) {
        CCO_LOG("Failed to create win32 window!");
        free(window);
        return CCO_FAIL_WINDOWING_CREATE_ERROR;
    }

    window->hWnd = hWnd;

    ShowWindow(hWnd, SW_SHOW);

    *out_window = window;
    return CCO_SUCCESS;
}

void cco_destroy_window(CcoWindow window) {
    if (IsWindow(window->hWnd)) {
        DestroyWindow(window->hWnd);
        window->hWnd = NULL;
    }
    free(window);
}

void cco_window_move(CcoWindow window, i32 x, i32 y) {
    SetWindowPos(window->hWnd, NULL, x, y, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void cco_window_resize(CcoWindow window, i32 width, i32 height) {
    const u32 dpi = GetDpiForWindow(window->hWnd);

    RECT rect = {0, 0, width, height};
    AdjustWindowRectExForDpi(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0, dpi);

    const i32 dpi_adjusted_width = rect.right - rect.left;
    const i32 dpi_adjusted_height = rect.bottom - rect.top;

    SetWindowPos(window->hWnd, NULL, 0, 0, dpi_adjusted_width, dpi_adjusted_height,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void cco_window_rename(CcoWindow window, const char *title) { SetWindowText(window->hWnd, title); }

void cco_window_pump_events(CcoWindow window) {
    MSG msg;
    while (PeekMessage(&msg, window->hWnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

cco_window_frame cco_window_get_frame(CcoWindow window) {
    RECT rect;
    GetWindowRect(window->hWnd, &rect);
    const i32 width = rect.right - rect.left;
    const i32 height = rect.bottom - rect.top;
    return (cco_window_frame){rect.left, rect.top, width, height};
}

cco_window_content_size cco_window_get_content_size(CcoWindow window) {
    RECT rect;
    GetClientRect(window->hWnd, &rect);
    const i32 width = rect.right - rect.left;
    const i32 height = rect.bottom - rect.top;
    return (cco_window_content_size){width, height};
}

void *cco_window_get_handle(CcoWindow window) { return window->hWnd; }
CcoBool cco_window_will_close(CcoWindow window) { return window->will_close; }
CcoBool cco_window_is_focus(CcoWindow window) { return window->focused; }