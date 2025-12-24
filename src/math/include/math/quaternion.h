//
// Created by fightinghawks18 on 12/23/2025.
//

#pragma once

#include "core/core_types.h"
#include "vector3.h"

typedef struct {
    f32 x, y, z;
    f32 w;
} quat;
typedef quat ccoquat;

static quat cco_quat() {
    quat q;
    q.x = 0.0f;
    q.y = 0.0f;
    q.z = 0.0f;
    q.w = 1.0f;
    return q;
}

static quat cco_quat_from_axis_angle(const vec3 axis, f32 angle) {
    const f32 half_angle = angle / 2.0f;
    const f32 s = sinf(half_angle);
    return (quat){
        .x = axis.x * s,
        .y = axis.y * s,
        .z = axis.z * s,
        .w = cosf(half_angle)
    };
}

static quat cco_quat_from_euler(const vec3 euler) {
    const f32 cy = cosf(euler.y * 0.5f);
    const f32 sy = sinf(euler.y * 0.5f);
    const f32 cp = cosf(euler.x * 0.5f);
    const f32 sp = sinf(euler.x * 0.5f);
    const f32 cr = cosf(euler.z * 0.5f);
    const f32 sr = sinf(euler.z * 0.5f);

    return (quat){
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy
    };
}

static quat cco_quat_normalized(const quat q) {
    const f32 len = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    if (len <= 0.0001f) {
        return q;
    }
    return (quat){
        q.x / len,
        q.y / len,
        q.z / len,
        q.w / len
    };
}

static quat cco_quat_mul(const quat q0, const quat q1) {
    return (quat){
        q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y,
        q0.w * q1.y - q0.x * q1.z + q0.y * q1.w + q0.z * q1.x,
        q0.w * q1.z + q0.x * q1.y - q0.y * q1.x + q0.z * q1.w,
        q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z
    };
}