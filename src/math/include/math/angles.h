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

static CcoRadians ccoDegreesToRadian(const CcoDegrees degrees) { return degrees * (PI / 180.0); }

static CcoDegrees ccoRadianToDegrees(const CcoRadians radians) { return radians * (180.0 / PI); }