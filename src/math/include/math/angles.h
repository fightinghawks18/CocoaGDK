//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "core/core_types.h"
#include <math.h>

// PI constant is not provided in modern C standards
#define PI acosl(-1.0)

typedef f64 CcoRadians;
typedef f32 CcoDegrees;

static CcoRadians cco_deg_to_rad(const CcoDegrees degrees) { return degrees * (PI / 180.0); }
static CcoDegrees cco_rad_to_deg(const CcoRadians radians) { return (f32)(radians * (180.0 / PI)); }