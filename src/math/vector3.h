//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "../core.h"

typedef struct {
    f32 x, y, z;
} CcoVector3;


inline CcoVector3 ccoCreateVector3(f32 x = 0.0f, f32 y = 0.0f, f32 z = 0.0f) {
    return {x, y, z};
}

inline CcoVector3 ccoCreateVector3Up() {
    return {0, 1, 0};
}

inline CcoVector3 ccoCreateVector3Right() {
    return {1, 0, 0};
}

inline CcoVector3 ccoCreateVector3Forward() {
    return {0, 0, 1};
}

inline CcoVector3 ccoAddVector3_Vector3(CcoVector3 vector3_a, CcoVector3 vector3_b) {
    return {
        vector3_a.x + vector3_b.x,
        vector3_a.y + vector3_b.y,
        vector3_a.z + vector3_b.z};
}

inline CcoVector3 ccoSubtractVector3_Vector3(CcoVector3 vector3_a, CcoVector3 vector3_b) {
    return {
        vector3_a.x - vector3_b.x,
        vector3_a.y - vector3_b.y,
        vector3_a.z - vector3_b.z};
}

inline CcoVector3 ccoDivideVector3_Vector3(CcoVector3 vector3_a, CcoVector3 vector3_b) {
    return {
        vector3_a.x / vector3_b.x,
        vector3_a.y / vector3_b.y,
        vector3_a.z / vector3_b.z};
}

inline CcoVector3 ccoMultiplyVector3_Scalar(CcoVector3 vector3_a, f32 scalar) {
    return {
        vector3_a.x * scalar,
        vector3_a.y * scalar,
        vector3_a.z * scalar};
}