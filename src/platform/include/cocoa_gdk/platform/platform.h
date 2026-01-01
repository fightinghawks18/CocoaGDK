//
// Created by fightinghawks18 on 12/29/2025.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "cocoa_gdk/core/core_types.h"
#include "cocoa_gdk/core/results.h"
#include "export.h"

typedef enum {
    CCO_PLATFORM_INIT_UNKNOWN,
    CCO_PLATFORM_INIT_WINDOWING_BIT,
    CCO_PLATFORM_INIT_INPUT_BIT,
    CCO_PLATFORM_INIT_AUDIO_BIT,
    CCO_PLATFORM_INIT_SOCKET_BIT,
} cco_platform_init_flag_bits;
typedef u32 cco_platform_init_flags;

CCO_PLATFORM_API cco_result cco_platform_init(cco_platform_init_flags init_flags);
CCO_PLATFORM_API void cco_platform_quit();

#ifdef __cplusplus
}
#endif