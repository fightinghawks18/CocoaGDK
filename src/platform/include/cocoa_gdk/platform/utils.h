//
// Created by fightinghawks18 on 12/19/25.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "cocoa_gdk/core/core_types.h"
#include "cocoa_gdk/core/results.h"
#include "cocoa_gdk/core/log.h"
#include "export.h"

/// @brief Pauses execution for an amount of time
/// @param ms Sleep time in milliseconds
CCO_PLATFORM_API void cco_sleep(u32 ms);

#ifdef __cplusplus
}
#endif