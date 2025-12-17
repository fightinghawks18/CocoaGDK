//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "core/core_types.h"
#include <math.h>

typedef struct {
    f32 x, y, z;
} Vec3;
typedef Vec3 CcoVec3;

inline Vec3 ccoVec3(const f32 x, const f32 y, const f32 z) {
    Vec3 vec3;
    vec3.x = x;
    vec3.y = y;
    vec3.z = z;
    return vec3;
}

inline Vec3 ccoVec3Up() {
    return ccoVec3(0, 1, 0);
}

inline Vec3 ccoVec3Right() {
    return ccoVec3(1, 0, 0);
}

inline Vec3 ccoVec3Forward() {
    return ccoVec3(0, 0, 1);
}

inline f32 ccoVec3Magnitude(const Vec3 vec3) {
    return sqrtf(powf(vec3.x, 2) + powf(vec3.y, 2) + powf(vec3.z, 2));
}

inline Vec3 ccoVec3Normalized(const Vec3 vec3) {
    const f32 mag = ccoVec3Magnitude(vec3);
    return ccoVec3(vec3.x/mag, vec3.y/mag, vec3.z/mag);
}

inline f32 ccoVec3Dot(const Vec3 vec3_a, const Vec3 vec3_b) {
    return vec3_a.x * vec3_b.x +
        vec3_a.y * vec3_b.y +
        vec3_a.z * vec3_b.z;
}

inline Vec3 ccoVec3Cross(const Vec3 vec3_a, const Vec3 vec3_b) {
    const f32 x = vec3_a.y * vec3_b.z - vec3_a.z * vec3_b.y;
    const f32 y = vec3_a.z * vec3_b.x - vec3_a.x * vec3_b.z;
    const f32 z = vec3_a.x * vec3_b.y - vec3_a.y * vec3_b.x;
    return ccoVec3(x, y, z);
}

inline Vec3 ccoVec3Add(const Vec3 vec3_a, const Vec3 vec3_b) {
    return ccoVec3(
        vec3_a.x + vec3_b.x,
        vec3_a.y + vec3_b.y,
        vec3_a.z + vec3_b.z);
}

inline Vec3 ccoVec3Sub(const Vec3 vec3_a, const Vec3 vec3_b) {
    return ccoVec3(
        vec3_a.x - vec3_b.x,
        vec3_a.y - vec3_b.y,
        vec3_a.z - vec3_b.z);
}

inline Vec3 ccoVec3Div(const Vec3 vec3_a, const Vec3 vec3_b) {
    return ccoVec3(
        vec3_a.x / vec3_b.x,
        vec3_a.y / vec3_b.y,
        vec3_a.z / vec3_b.z);
}

inline Vec3 ccoVec3Scale(const Vec3 vec3_a, const f32 scalar) {
    return ccoVec3(
        vec3_a.x * scalar,
        vec3_a.y * scalar,
        vec3_a.z * scalar);
}