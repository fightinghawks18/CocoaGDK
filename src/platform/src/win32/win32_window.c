//
// Created by fightinghawks18 on 12/19/25.
//

#include "platform/window.h"
#include <windows.h>
#include "platform/win32/win32_input.h"

struct CcoWindow_T {
    HWND hWnd;
    CcoBool willClose;
    CcoBool focused;
};

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
    case WM_INPUT: {
        ccoInputHandleRawInput(hWnd, lParam);
        return 0;
    }
    case WM_CLOSE: {
        window->willClose = CCO_YES;
        return 0;
    }
    default: break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


