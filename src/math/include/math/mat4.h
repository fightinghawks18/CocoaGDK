//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "angles.h"
#include "core/core_types.h"
#include "vec3.h"

typedef struct {
    f32 m[4][4];
} Mat4;
typedef Mat4 CcoMat4;

/**
 * @brief Creates an identity matrix with 4 rows and 4 columns
 * @return Mat4
 */
inline Mat4 ccoMat4() {
    Mat4 matrix;
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

inline Mat4 ccoMat4Mul(const Mat4 mat4_a, const Mat4 mat4_b) {
    Mat4 result = ccoMat4();
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

inline Mat4 ccoMat4Transpose(const Mat4 mat4) {
    Mat4 result = ccoMat4();
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            result.m[i][j] = mat4.m[j][i];
        }
    }
    return result;
}

inline Mat4 ccoMat4Translation(const Vec3 position) {
    Mat4 result = ccoMat4();
    result.m[0][3] = position.x;
    result.m[1][3] = position.y;
    result.m[2][3] = position.z;
    return result;
}

inline Mat4 ccoMat4XRotation(const CcoRadians radians) {
    Mat4 result = ccoMat4();
    result.m[1][1] = (f32)cos(radians);
    result.m[1][2] = (f32)-sin(radians);
    result.m[2][1] = (f32)sin(radians);
    result.m[2][2] = (f32)cos(radians);
    return result;
}

inline Mat4 ccoMat4YRotation(const CcoRadians radians) {
    Mat4 result = ccoMat4();
    result.m[0][0] = (f32)cos(radians);
    result.m[0][2] = (f32)sin(radians);
    result.m[2][0] = (f32)-sin(radians);
    result.m[2][2] = (f32)cos(radians);
    return result;
}

inline Mat4 ccoMat4ZRotation(const CcoRadians radians) {
    Mat4 result = ccoMat4();
    result.m[0][0] = (f32)cos(radians);
    result.m[0][1] = (f32)-sin(radians);
    result.m[1][0] = (f32)sin(radians);
    result.m[1][1] = (f32)cos(radians);
    return result;
}

inline Mat4 ccoMat4Rotation(const Vec3 rotation) {
    const Mat4 xRotation = ccoMat4XRotation(rotation.x);
    const Mat4 yRotation = ccoMat4YRotation(rotation.y);
    const Mat4 zRotation = ccoMat4ZRotation(rotation.z);
    return ccoMat4Mul(xRotation, ccoMat4Mul(yRotation, zRotation));
}

inline Mat4 ccoMat4Scale(const Vec3 scale) {
    Mat4 result = ccoMat4();
    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;
    return result;
}

inline Mat4 ccoMat4Perspective(const CcoBool flipY, const CcoBool zeroToOneDepth, const CcoRadians fov, const f32 aspectRatio,
                                                  const f32 nearClippingPoint, const f32 farClippingPoint) {
    Mat4 result = ccoMat4();

    const CcoRadians tanHalfFov = tan(fov / 2.0f);
    const f32 xZoom = 1.0f / (aspectRatio * (f32)tanHalfFov);
    const f32 yZoom = 1.0f / (f32)tanHalfFov;

    f32 coefficient, constantOffset;
    if (zeroToOneDepth) {
        coefficient = -farClippingPoint / (farClippingPoint - nearClippingPoint);
        constantOffset = -(farClippingPoint * nearClippingPoint) / (farClippingPoint - nearClippingPoint);
    } else {
        coefficient = -(farClippingPoint + nearClippingPoint) / (farClippingPoint - nearClippingPoint);
        constantOffset = -(2.0f * farClippingPoint * nearClippingPoint) / (farClippingPoint - nearClippingPoint);
    }

    result.m[0][0] = xZoom;
    result.m[1][1] = yZoom * (flipY ? -1.0f : 1.0f);
    result.m[2][2] = coefficient;
    result.m[2][3] = constantOffset;
    result.m[3][2] = -1.0f;
    result.m[3][3] = 0.0f;

    return result;
}

inline Mat4 ccoMat4Eye(const Vec3 eyePosition, const Vec3 eyeTarget,
                                          const Vec3 eyeUp) {
    const Vec3 forward = ccoVec3Normalized(ccoVec3Sub(eyePosition, eyeTarget));
    const Vec3 right = ccoVec3Normalized(ccoVec3Cross(eyeUp, forward));
    const Vec3 cameraUp = ccoVec3Cross(forward, right);

    Mat4 result = ccoMat4();
    result.m[0][0] = right.x;
    result.m[0][1] = right.y;
    result.m[0][2] = right.z;

    result.m[1][0] = cameraUp.x;
    result.m[1][1] = cameraUp.y;
    result.m[1][2] = cameraUp.z;

    result.m[2][0] = forward.x;
    result.m[2][1] = forward.y;
    result.m[2][2] = forward.z;

    result.m[0][3] = -ccoVec3Dot(right, eyePosition);
    result.m[1][3] = -ccoVec3Dot(cameraUp, eyePosition);
    result.m[2][3] = -ccoVec3Dot(forward, eyePosition);

    result.m[3][3] = 1.0f;
    return result;
}

inline Mat4 ccoMat4Model(const Mat4 translation, const Mat4 rotation,
                                            const Mat4 scale) {
    return ccoMat4Mul(translation, ccoMat4Mul(rotation, scale));
}

inline void ccoMat4ToF32Arr(const Mat4 mat4, f32 *result) {
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            result[i * 4 + j] = mat4.m[i][j];
        }
    }
}