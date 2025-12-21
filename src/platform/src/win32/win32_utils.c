//
// Created by fightinghawks18 on 12/20/2025.
//

#include "platform/utils.h"
#include <windows.h>

_Thread_local HANDLE SLEEP_TIMER = NULL;

void cco_sleep(const u32 ms) {
    if (!SLEEP_TIMER) {
        SLEEP_TIMER = CreateWaitableTimer(NULL, TRUE, NULL);
    }
    LARGE_INTEGER ft;
    ft.QuadPart = -((LONGLONG)ms * 10000LL);
    SetWaitableTimer(SLEEP_TIMER, &ft, 0, NULL, NULL, FALSE);
    WaitForSingleObject(SLEEP_TIMER, INFINITE);
}