//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "core/core_types.h"
#include "angles.h"
#include "vector3.h"

typedef struct {
    f32 m[4][4];
} CcoMatrix4X4;

/**
 * @brief Creates an identity matrix with 4 rows and 4 columns
 * @return Matrix4X4
 */
inline CcoMatrix4X4 ccoCreateMatrix4X4() {
    CcoMatrix4X4 matrix;
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

inline CcoMatrix4X4 ccoMultiplyMatrix4X4_Matrix4X4(const CcoMatrix4X4 matrix4x4_a, const CcoMatrix4X4 matrix4x4_b) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            result.m[i][j] = matrix4x4_a.m[i][0] * matrix4x4_b.m[0][j] +
                 matrix4x4_a.m[i][1] * matrix4x4_b.m[1][j] +
                 matrix4x4_a.m[i][2] * matrix4x4_b.m[2][j] +
                 matrix4x4_a.m[i][3] * matrix4x4_b.m[3][j];
        }
    }
    return result;
}

inline CcoMatrix4X4 ccoTransposeMatrix4X4(const CcoMatrix4X4 matrix4x4) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            result.m[i][j] = matrix4x4.m[j][i];
        }
    }
    return result;
}

inline CcoMatrix4X4 ccoCreateTranslationMatrix4X4(const CcoVector3 position) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    result.m[0][3] = position.x;
    result.m[1][3] = position.y;
    result.m[2][3] = position.z;
    return result;
}

inline CcoMatrix4X4 ccoCreateXRotationMatrix4X4(const CcoRadians radians) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    result.m[1][1] = (f32)cos(radians);
    result.m[1][2] = (f32)-sin(radians);
    result.m[2][1] = (f32)sin(radians);
    result.m[2][2] = (f32)cos(radians);
    return result;
}

inline CcoMatrix4X4 ccoCreateYRotationMatrix4X4(const CcoRadians radians) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    result.m[0][0] = (f32)cos(radians);
    result.m[0][2] = (f32)sin(radians);
    result.m[2][0] = (f32)-sin(radians);
    result.m[2][2] = (f32)cos(radians);
    return result;
}

inline CcoMatrix4X4 ccoCreateZRotationMatrix4X4(const CcoRadians radians) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    result.m[0][0] = (f32)cos(radians);
    result.m[0][1] = (f32)-sin(radians);
    result.m[1][0] = (f32)sin(radians);
    result.m[1][1] = (f32)cos(radians);
    return result;
}

inline CcoMatrix4X4 ccoCreateRotationMatrix4X4(const CcoVector3 rotation) {
    const CcoMatrix4X4 xRotation = ccoCreateXRotationMatrix4X4(rotation.x);
    const CcoMatrix4X4 yRotation = ccoCreateYRotationMatrix4X4(rotation.y);
    const CcoMatrix4X4 zRotation = ccoCreateZRotationMatrix4X4(rotation.z);
    return ccoMultiplyMatrix4X4_Matrix4X4(xRotation, ccoMultiplyMatrix4X4_Matrix4X4(yRotation, zRotation));
}

inline CcoMatrix4X4 ccoCreateScaleMatrix4x4(const CcoVector3 scale) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;
    return result;
}

inline CcoMatrix4X4 ccoCreatePerspectiveMatrix4X4(CcoBool flipY, CcoBool zeroToOneDepth, const CcoRadians fov, const f32 aspectRatio,
                                                  const f32 nearClippingPoint, const f32 farClippingPoint) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();

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

inline CcoMatrix4X4 ccoCreateEyeMatrix4X4(const CcoVector3 eyePosition, const CcoVector3 eyeTarget,
                                          const CcoVector3 eyeUp) {
    const CcoVector3 forward = ccoNormalizeVector3(ccoSubtractVector3_Vector3(eyePosition, eyeTarget));
    const CcoVector3 right = ccoNormalizeVector3(ccoCrossVector3(eyeUp, forward));
    const CcoVector3 cameraUp = ccoCrossVector3(forward, right);

    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    result.m[0][0] = right.x;
    result.m[0][1] = right.y;
    result.m[0][2] = right.z;

    result.m[1][0] = cameraUp.x;
    result.m[1][1] = cameraUp.y;
    result.m[1][2] = cameraUp.z;

    result.m[2][0] = forward.x;
    result.m[2][1] = forward.y;
    result.m[2][2] = forward.z;

    result.m[0][3] = -ccoDotVector3(right, eyePosition);
    result.m[1][3] = -ccoDotVector3(cameraUp, eyePosition);
    result.m[2][3] = -ccoDotVector3(forward, eyePosition);

    result.m[3][3] = 1.0f;
    return result;
}

inline CcoMatrix4X4 ccoCreateModelMatrix4X4(const CcoMatrix4X4 translation, const CcoMatrix4X4 rotation,
                                            const CcoMatrix4X4 scale) {
    return ccoMultiplyMatrix4X4_Matrix4X4(translation, ccoMultiplyMatrix4X4_Matrix4X4(rotation, scale));
}

inline void ccoConvertMatrix4X4ToFloatArray(const CcoMatrix4X4 matrix4x4, f32 *result) {
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            result[i * 4 + j] = matrix4x4.m[i][j];
        }
    }
}