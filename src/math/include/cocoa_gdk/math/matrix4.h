//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "angles.h"
#include "cocoa_gdk/core/core_types.h"
#include "cocoa_gdk/core/results.h"
#include "cocoa_gdk/core/log.h"
#include "quaternion.h"
#include "vector3.h"

typedef struct {
    f32 m[4][4];
} mat4;
typedef mat4 ccomat4;

/**
 * @brief Creates an identity matrix with 4 rows and 4 columns
 * @return mat4
 */
static mat4 cco_mat4(void) {
    mat4 matrix;
    matrix.m[0][0] = 1;
    matrix.m[0][1] = 0;
    matrix.m[0][2] = 0;
    matrix.m[0][3] = 0;
    matrix.m[1][0] = 0;
    matrix.m[1][1] = 1;
    matrix.m[1][2] = 0;
    matrix.m[1][3] = 0;
    matrix.m[2][0] = 0;
    matrix.m[2][1] = 0;
    matrix.m[2][2] = 1;
    matrix.m[2][3] = 0;
    matrix.m[3][0] = 0;
    matrix.m[3][1] = 0;
    matrix.m[3][2] = 0;
    matrix.m[3][3] = 1;
    return matrix;
}

static mat4 cco_mat4_mul(const mat4 mat4_a, const mat4 mat4_b) {
    mat4 result = cco_mat4();
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            result.m[i][j] = mat4_a.m[i][0] * mat4_b.m[0][j] +
                 mat4_a.m[i][1] * mat4_b.m[1][j] +
                 mat4_a.m[i][2] * mat4_b.m[2][j] +
                 mat4_a.m[i][3] * mat4_b.m[3][j];
        }
    }
    return result;
}

static mat4 cco_mat4_transpose(const mat4 matrix) {
    mat4 result = cco_mat4();
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            result.m[i][j] = matrix.m[j][i];
        }
    }
    return result;
}

static mat4 cco_mat4_translation(const vec3 position) {
    mat4 result = cco_mat4();
    result.m[0][3] = position.x;
    result.m[1][3] = position.y;
    result.m[2][3] = position.z;
    return result;
}

static mat4 cco_mat4_x_rotation(const cco_rad radians) {
    mat4 result = cco_mat4();
    result.m[1][1] = (f32)cos(radians);
    result.m[1][2] = (f32)-sin(radians);
    result.m[2][1] = (f32)sin(radians);
    result.m[2][2] = (f32)cos(radians);
    return result;
}

static mat4 cco_mat4_y_rotation(const cco_rad radians) {
    mat4 result = cco_mat4();
    result.m[0][0] = (f32)cos(radians);
    result.m[0][2] = (f32)sin(radians);
    result.m[2][0] = (f32)-sin(radians);
    result.m[2][2] = (f32)cos(radians);
    return result;
}

static mat4 cco_mat4_z_rotation(const cco_rad radians) {
    mat4 result = cco_mat4();
    result.m[0][0] = (f32)cos(radians);
    result.m[0][1] = (f32)-sin(radians);
    result.m[1][0] = (f32)sin(radians);
    result.m[1][1] = (f32)cos(radians);
    return result;
}

static mat4 cco_mat4_rotation(const vec3 rotation) {
    const mat4 x_rotation = cco_mat4_x_rotation(rotation.x);
    const mat4 y_rotation = cco_mat4_y_rotation(rotation.y);
    const mat4 z_rotation = cco_mat4_z_rotation(rotation.z);
    return cco_mat4_mul(x_rotation, cco_mat4_mul(y_rotation, z_rotation));
}

static mat4 cco_mat4_quat(const quat q) {
    mat4 result = cco_mat4();

    const f32 xx = q.x * q.x;
    const f32 yy = q.y * q.y;
    const f32 zz = q.z * q.z;
    const f32 xy = q.x * q.y;
    const f32 xz = q.x * q.z;
    const f32 yz = q.y * q.z;
    const f32 wx = q.w * q.x;
    const f32 wy = q.w * q.y;
    const f32 wz = q.w * q.z;

    result.m[0][0] = 1.0f - 2.0f * (yy + zz);
    result.m[0][1] = 2.0f * (xy - wz);
    result.m[0][2] = 2.0f * (xz + wy);
    result.m[0][3] = 0.0f;

    result.m[1][0] = 2.0f * (xy + wz);
    result.m[1][1] = 1.0f - 2.0f * (xx + zz);
    result.m[1][2] = 2.0f * (yz - wx);
    result.m[1][3] = 0.0f;

    result.m[2][0] = 2.0f * (xz - wy);
    result.m[2][1] = 2.0f * (yz + wx);
    result.m[2][2] = 1.0f - 2.0f * (xx + yy);
    result.m[2][3] = 0.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
}

static mat4 cco_mat4_scale(const vec3 scale) {
    mat4 result = cco_mat4();
    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;
    return result;
}

static mat4 cco_mat4_perspective(const cco_bool flip_y, const cco_bool zero_to_one_depth, const cco_rad fov, const f32 aspect_ratio,
                                                  const f32 near_clipping_point, const f32 far_clipping_point) {
    mat4 result = cco_mat4();

    const cco_rad tan_half_fov = tan(fov / 2.0f);
    const f32 x_zoom = 1.0f / (aspect_ratio * (f32)tan_half_fov);
    const f32 y_zoom = 1.0f / (f32)tan_half_fov;

    f32 coefficient, constant_offset;
    if (zero_to_one_depth) {
        coefficient = -far_clipping_point / (far_clipping_point - near_clipping_point);
        constant_offset = -(far_clipping_point * near_clipping_point) / (far_clipping_point - near_clipping_point);
    } else {
        coefficient = -(far_clipping_point + near_clipping_point) / (far_clipping_point - near_clipping_point);
        constant_offset = -(2.0f * far_clipping_point * near_clipping_point) / (far_clipping_point - near_clipping_point);
    }

    result.m[0][0] = x_zoom;
    result.m[1][1] = y_zoom * (flip_y ? -1.0f : 1.0f);
    result.m[2][2] = coefficient;
    result.m[2][3] = constant_offset;
    result.m[3][2] = -1.0f;
    result.m[3][3] = 0.0f;

    return result;
}

static mat4 cco_mat4_eye(const vec3 eye_position, const vec3 eye_target,
                                          const vec3 eye_up) {
    const vec3 forward = cco_vec3_normalized(cco_vec3_sub(eye_position, eye_target));
    const vec3 right = cco_vec3_normalized(cco_vec3_cross(eye_up, forward));
    const vec3 camera_up = cco_vec3_cross(forward, right);

    mat4 result = cco_mat4();
    result.m[0][0] = right.x;
    result.m[0][1] = right.y;
    result.m[0][2] = right.z;

    result.m[1][0] = camera_up.x;
    result.m[1][1] = camera_up.y;
    result.m[1][2] = camera_up.z;

    result.m[2][0] = forward.x;
    result.m[2][1] = forward.y;
    result.m[2][2] = forward.z;

    result.m[0][3] = -cco_vec3_dot(right, eye_position);
    result.m[1][3] = -cco_vec3_dot(camera_up, eye_position);
    result.m[2][3] = -cco_vec3_dot(forward, eye_position);

    result.m[3][3] = 1.0f;
    return result;
}

static mat4 cco_mat4_model(const mat4 translation, const mat4 rotation,
                                            const mat4 scale) {
    return cco_mat4_mul(translation, cco_mat4_mul(rotation, scale));
}

static void cco_mat4_to_f32_arr(const mat4 mat4, f32 *result) {
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            result[i * 4 + j] = mat4.m[i][j];
        }
    }
}

#ifdef __cplusplus
}
#endif