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

inline Vec2 ccoCreateVector2(const f32 x, const f32 y) {
    Vec2 vector2;
    vector2.x = x;
    vector2.y = y;
    return vector2;
}

inline Vec2 ccoCreateVector2Up() {
    return ccoCreateVector2(0, 1);
}

inline Vec2 ccoCreateVector2Right() {
    return ccoCreateVector2(1, 0);
}

inline f32 ccoGetVector2Magnitude(const Vec2 vector2) {
    return sqrtf(powf(vector2.x, 2) + powf(vector2.y, 2));
}

inline Vec2 ccoNormalizeVector2(const Vec2 vector2) {
    const f32 mag = ccoGetVector2Magnitude(vector2);
    return ccoCreateVector2(vector2.x/mag, vector2.y/mag);
}

inline f32 ccoDotVector2(const Vec2 vector2_a, const Vec2 vector2_b) {
    return vector2_a.x * vector2_b.x +
        vector2_a.y * vector2_b.y;
}

inline Vec2 ccoAddVector2_Vector2(const Vec2 vector2_a, const Vec2 vector2_b) {
    return ccoCreateVector2(
        vector2_a.x + vector2_b.x,
        vector2_a.y + vector2_b.y);
}

inline Vec2 ccoSubtractVector2_Vector2(const Vec2 vector2_a, const Vec2 vector2_b) {
    return ccoCreateVector2(
        vector2_a.x - vector2_b.x,
        vector2_a.y - vector2_b.y);
}

inline Vec2 ccoDivideVector2_Vector2(const Vec2 vector2_a, const Vec2 vector2_b) {
    return ccoCreateVector2(
        vector2_a.x / vector2_b.x,
        vector2_a.y / vector2_b.y);
}

inline Vec2 ccoMultiplyVector2_Scalar(const Vec2 vector2_a, const f32 scalar) {
    return ccoCreateVector2(
        vector2_a.x * scalar,
        vector2_a.y * scalar);
}