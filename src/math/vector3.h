//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "../core.h"
#include <math.h>

typedef struct {
    f32 x, y, z;
} CcoVector3;

static CcoVector3 ccoCreateVector3(f32 x, f32 y, f32 z) {
    CcoVector3 vector3;
    vector3.x = x;
    vector3.y = y;
    vector3.z = z;
    return vector3;
}

static CcoVector3 ccoCreateVector3Up() {
    return ccoCreateVector3(0, 1, 0);
}

static CcoVector3 ccoCreateVector3Right() {
    return ccoCreateVector3(1, 0, 0);
}

static CcoVector3 ccoCreateVector3Forward() {
    return ccoCreateVector3(0, 0, 1);
}

static f32 ccoGetVector3Magnitude(const CcoVector3 vector3) {
    return sqrt(powf(vector3.x, 2) + powf(vector3.y, 2) + powf(vector3.z, 2));
}

static CcoVector3 ccoNormalizeVector3(const CcoVector3 vector3) {
    const f32 mag = ccoGetVector3Magnitude(vector3);
    return ccoCreateVector3(vector3.x/mag, vector3.y/mag, vector3.z/mag);
}

static f32 ccoDotVector3(const CcoVector3 vector3_a, const CcoVector3 vector3_b) {
    return vector3_a.x * vector3_b.x +
        vector3_a.y * vector3_b.y +
        vector3_a.z * vector3_b.z;
}

static CcoVector3 ccoCrossVector3(const CcoVector3 vector3_a, const CcoVector3 vector3_b) {
    const f32 x = vector3_a.y * vector3_b.z - vector3_a.z * vector3_b.y;
    const f32 y = vector3_a.z * vector3_b.x - vector3_a.x * vector3_b.z;
    const f32 z = vector3_a.x * vector3_b.y - vector3_a.y * vector3_b.x;
    return ccoCreateVector3(x, y, z);
}

static CcoVector3 ccoAddVector3_Vector3(const CcoVector3 vector3_a, const CcoVector3 vector3_b) {
    return ccoCreateVector3(
        vector3_a.x + vector3_b.x,
        vector3_a.y + vector3_b.y,
        vector3_a.z + vector3_b.z);
}

static CcoVector3 ccoSubtractVector3_Vector3(const CcoVector3 vector3_a, const CcoVector3 vector3_b) {
    return ccoCreateVector3(
        vector3_a.x - vector3_b.x,
        vector3_a.y - vector3_b.y,
        vector3_a.z - vector3_b.z);
}

static CcoVector3 ccoDivideVector3_Vector3(const CcoVector3 vector3_a, const CcoVector3 vector3_b) {
    return ccoCreateVector3(
        vector3_a.x / vector3_b.x,
        vector3_a.y / vector3_b.y,
        vector3_a.z / vector3_b.z);
}

static CcoVector3 ccoMultiplyVector3_Scalar(const CcoVector3 vector3_a, const f32 scalar) {
    return ccoCreateVector3(
        vector3_a.x * scalar,
        vector3_a.y * scalar,
        vector3_a.z * scalar);
}