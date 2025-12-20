//
// Created by fightinghawks18 on 12/19/25.
//

#include "platform/utils.h"

#include <time.h>

void ccoSleep(const u32 ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}