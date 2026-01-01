//
// Created by fightinghawks18 on 12/24/2025.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "cocoa_gdk/core/core_types.h"
#include "cocoa_gdk/core/results.h"
#include "cocoa_gdk/core/log.h"

#define CCO_CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#ifdef __cplusplus
}
#endif