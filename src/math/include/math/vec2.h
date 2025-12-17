//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "core/core_types.h"
#include <math.h>

typedef struct {
    f32 x, y;
} Vec2;
typedef Vec2 CcoVec2;

inline Vec2 ccoVec2(const f32 x, const f32 y) {
    Vec2 vec2;
    vec2.x = x;
    vec2.y = y;
    return vec2;
}

inline Vec2 ccoVec2Up() {
    return ccoVec2(0, 1);
}

inline Vec2 ccoVec2Right() {
    return ccoVec2(1, 0);
}

inline f32 ccoGetVec2Magnitude(const Vec2 vec2) {
    return sqrtf(powf(vec2.x, 2) + powf(vec2.y, 2));
}

inline Vec2 ccoNormalizeVec2(const Vec2 vec2) {
    const f32 mag = ccoGetVec2Magnitude(vec2);
    return ccoVec2(vec2.x/mag, vec2.y/mag);
}

inline f32 ccoDotVec2(const Vec2 vec2_a, const Vec2 vec2_b) {
    return vec2_a.x * vec2_b.x +
        vec2_a.y * vec2_b.y;
}

inline Vec2 ccoAddVec2_Vec2(const Vec2 vec2_a, const Vec2 vec2_b) {
    return ccoVec2(
        vec2_a.x + vec2_b.x,
        vec2_a.y + vec2_b.y);
}

inline Vec2 ccoSubtractVec2_Vec2(const Vec2 vec2_a, const Vec2 vec2_b) {
    return ccoVec2(
        vec2_a.x - vec2_b.x,
        vec2_a.y - vec2_b.y);
}

inline Vec2 ccoDivideVec2_Vec2(const Vec2 vec2_a, const Vec2 vec2_b) {
    return ccoVec2(
        vec2_a.x / vec2_b.x,
        vec2_a.y / vec2_b.y);
}

inline Vec2 ccoMultiplyVec2_Scalar(const Vec2 vec2_a, const f32 scalar) {
    return ccoVec2(
        vec2_a.x * scalar,
        vec2_a.y * scalar);
}