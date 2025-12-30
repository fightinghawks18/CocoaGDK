//
// Created by fightinghawks18 on 12/19/25.
//

#pragma once

#include "core/core_types.h"
#include "core/results.h"
#include "core/log.h"
#include "export.h"

/// @brief Pauses execution for an amount of time
/// @param ms Sleep time in milliseconds
CCO_PLATFORM_API void cco_sleep(u32 ms);