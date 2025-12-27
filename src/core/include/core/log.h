//
// Created by fightinghawks18 on 12/26/2025.
//

#pragma once

#include <stddef.h>
#include <stdio.h>

#include "export.h"

CCO_CORE_API void cco_set_log_file_name(const char *name);
CCO_CORE_API void cco_log_to_file(const char *fmt, ...);

#ifdef DEBUG
#define CCO_LOG(str, ...)                                                                                              \
    do {                                                                                                               \
        cco_log_to_file(str, ##__VA_ARGS__);                                                                           \
        printf(str "\n", ##__VA_ARGS__);                                                                               \
    } while (0)
#else
#define CCO_LOG(str, ...)
#endif