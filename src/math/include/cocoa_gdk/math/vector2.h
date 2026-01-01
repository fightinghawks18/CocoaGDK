//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "cocoa_gdk/core/core_types.h"
#include "cocoa_gdk/core/results.h"
#include "cocoa_gdk/core/log.h"
#include <math.h>

typedef struct {
    f32 x, y;
} vec2;
typedef vec2 ccovec2;

static vec2 cco_vec2(const f32 x, const f32 y) {
    vec2 vec2;
    vec2.x = x;
    vec2.y = y;
    return vec2;
}

static vec2 cco_vec2_up(void) {
    return cco_vec2(0, 1);
}

static vec2 cco_vec2_right(void) {
    return cco_vec2(1, 0);
}

static f32 cco_vec2_magnitude(const vec2 vec2) {
    return sqrtf(powf(vec2.x, 2) + powf(vec2.y, 2));
}

static vec2 cco_vec2_normalized(const vec2 vec2) {
    const f32 mag = cco_vec2_magnitude(vec2);
    return cco_vec2(vec2.x/mag, vec2.y/mag);
}

static f32 cco_vec2_dot(const vec2 vec2_a, const vec2 vec2_b) {
    return vec2_a.x * vec2_b.x +
        vec2_a.y * vec2_b.y;
}

static vec2 cco_vec2_add(const vec2 vec2_a, const vec2 vec2_b) {
    return cco_vec2(
        vec2_a.x + vec2_b.x,
        vec2_a.y + vec2_b.y);
}

static vec2 cco_vec2_sub(const vec2 vec2_a, const vec2 vec2_b) {
    return cco_vec2(
        vec2_a.x - vec2_b.x,
        vec2_a.y - vec2_b.y);
}

static vec2 cco_vec2_div(const vec2 vec2_a, const vec2 vec2_b) {
    return cco_vec2(
        vec2_a.x / vec2_b.x,
        vec2_a.y / vec2_b.y);
}

static vec2 cco_vec2_scale(const vec2 vec2_a, const f32 scalar) {
    return cco_vec2(
        vec2_a.x * scalar,
        vec2_a.y * scalar);
}

#ifdef __cplusplus
}
#endif