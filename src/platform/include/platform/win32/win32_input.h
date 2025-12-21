//
// Created by fightinghawks18 on 12/20/25.
//

#pragma once

#ifdef _WIN32

#include "platform/exports.h"

#include <windows.h>
CCO_PLATFORM_API void cco_input_handle_raw_input(HWND hWnd, LPARAM lparam);

#endif // _WIN32
