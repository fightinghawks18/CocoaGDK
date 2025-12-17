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

inline CcoRadians ccoDegToRad(const CcoDegrees degrees) { return degrees * (PI / 180.0); }
inline CcoDegrees ccoRadToDeg(const CcoRadians radians) { return (f32)(radians * (180.0 / PI)); }