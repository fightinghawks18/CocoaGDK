//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include <math.h>
#include "../core.h"

// PI constant is not provided in modern C standards
#define PI acosl(-1.0)

typedef f64 CcoRadians;
typedef f32 CcoDegrees;

inline CcoRadians ccoDegreesToRadian(const CcoDegrees degrees) {
    return degrees * (PI/180.0);
}

inline CcoDegrees ccoRadianToDegrees(const CcoRadians radians) {
    return radians * (180.0/PI);
}