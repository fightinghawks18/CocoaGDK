//
// Created by fightinghawks18 on 12/30/2025.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32

#include <windows.h>

void cco_win32_handle_raw_input(LPARAM lparam);

#endif // _WIN32

#ifdef __cplusplus
}
#endif