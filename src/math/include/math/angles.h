//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "core/core_types.h"
#include "core/results.h"
#include "core/log.h"
#include <math.h>

// PI constant is not provided in modern C standards
#define PI acosl(-1.0)

typedef f64 cco_rad;
typedef f32 cco_deg;

static cco_rad cco_deg_to_rad(const cco_deg degrees) { return degrees * (PI / 180.0); }
static cco_deg cco_rad_to_deg(const cco_rad radians) { return (f32)(radians * (180.0 / PI)); }