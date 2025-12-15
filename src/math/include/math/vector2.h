//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "../core.h"
#include <math.h>

typedef struct {
    f32 x, y;
} CcoVector2;

static CcoVector2 ccoCreateVector2(f32 x, f32 y) {
    CcoVector2 vector2;
    vector2.x = x;
    vector2.y = y;
    return vector2;
}

static CcoVector2 ccoCreateVector2Up() {
    return ccoCreateVector2(0, 1);
}

static CcoVector2 ccoCreateVector2Right() {
    return ccoCreateVector2(1, 0);
}

static f32 ccoGetVector2Magnitude(const CcoVector2 vector2) {
    return sqrt(powf(vector2.x, 2) + powf(vector2.y, 2));
}

static CcoVector2 ccoNormalizeVector2(const CcoVector2 vector2) {
    const f32 mag = ccoGetVector2Magnitude(vector2);
    return ccoCreateVector2(vector2.x/mag, vector2.y/mag);
}

static f32 ccoDotVector2(const CcoVector2 vector2_a, const CcoVector2 vector2_b) {
    return vector2_a.x * vector2_b.x +
        vector2_a.y * vector2_b.y;
}

static CcoVector2 ccoAddVector2_Vector2(const CcoVector2 vector2_a, const CcoVector2 vector2_b) {
    return ccoCreateVector2(
        vector2_a.x + vector2_b.x,
        vector2_a.y + vector2_b.y);
}

static CcoVector2 ccoSubtractVector2_Vector2(const CcoVector2 vector2_a, const CcoVector2 vector2_b) {
    return ccoCreateVector2(
        vector2_a.x - vector2_b.x,
        vector2_a.y - vector2_b.y);
}

static CcoVector2 ccoDivideVector2_Vector2(const CcoVector2 vector2_a, const CcoVector2 vector2_b) {
    return ccoCreateVector2(
        vector2_a.x / vector2_b.x,
        vector2_a.y / vector2_b.y);
}

static CcoVector2 ccoMultiplyVector2_Scalar(const CcoVector2 vector2_a, const f32 scalar) {
    return ccoCreateVector2(
        vector2_a.x * scalar,
        vector2_a.y * scalar);
}