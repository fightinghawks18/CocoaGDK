//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "core/core_types.h"
#include <math.h>

typedef struct {
    f32 x, y, z;
} vec3;
typedef vec3 CcoVec3;

static vec3 cco_vec3(const f32 x, const f32 y, const f32 z) {
    vec3 vec3;
    vec3.x = x;
    vec3.y = y;
    vec3.z = z;
    return vec3;
}

static vec3 cco_vec3_up(void) {
    return cco_vec3(0, 1, 0);
}

static vec3 cco_vec3_right(void) {
    return cco_vec3(1, 0, 0);
}

static vec3 cco_vec3_forward(void) {
    return cco_vec3(0, 0, 1);
}

static f32 cco_vec3_magnitude(const vec3 vec3) {
    return sqrtf(powf(vec3.x, 2) + powf(vec3.y, 2) + powf(vec3.z, 2));
}

static vec3 cco_vec3_normalized(const vec3 vec3) {
    const f32 mag = cco_vec3_magnitude(vec3);
    return cco_vec3(vec3.x/mag, vec3.y/mag, vec3.z/mag);
}

static f32 cco_vec3_dot(const vec3 vec3_a, const vec3 vec3_b) {
    return vec3_a.x * vec3_b.x +
        vec3_a.y * vec3_b.y +
        vec3_a.z * vec3_b.z;
}

static vec3 cco_vec3_cross(const vec3 vec3_a, const vec3 vec3_b) {
    const f32 x = vec3_a.y * vec3_b.z - vec3_a.z * vec3_b.y;
    const f32 y = vec3_a.z * vec3_b.x - vec3_a.x * vec3_b.z;
    const f32 z = vec3_a.x * vec3_b.y - vec3_a.y * vec3_b.x;
    return cco_vec3(x, y, z);
}

static vec3 cco_vec3_add(const vec3 vec3_a, const vec3 vec3_b) {
    return cco_vec3(
        vec3_a.x + vec3_b.x,
        vec3_a.y + vec3_b.y,
        vec3_a.z + vec3_b.z);
}

static vec3 cco_vec3_sub(const vec3 vec3_a, const vec3 vec3_b) {
    return cco_vec3(
        vec3_a.x - vec3_b.x,
        vec3_a.y - vec3_b.y,
        vec3_a.z - vec3_b.z);
}

static vec3 cco_vec3_div(const vec3 vec3_a, const vec3 vec3_b) {
    return cco_vec3(
        vec3_a.x / vec3_b.x,
        vec3_a.y / vec3_b.y,
        vec3_a.z / vec3_b.z);
}

static vec3 cco_vec3_scale(const vec3 vec3_a, const f32 scalar) {
    return cco_vec3(
        vec3_a.x * scalar,
        vec3_a.y * scalar,
        vec3_a.z * scalar);
}