//
// Created by fightinghawks18 on 12/11/25.
//

#pragma once

#include "../core.h"
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
    matrix.m[0][0] = 1; matrix.m[0][1] = 0; matrix.m[0][2] = 0; matrix.m[0][3] = 0;
    matrix.m[1][0] = 0; matrix.m[1][1] = 1; matrix.m[1][2] = 0; matrix.m[1][3] = 0;
    matrix.m[2][0] = 0; matrix.m[2][1] = 0; matrix.m[2][2] = 1; matrix.m[2][3] = 0;
    matrix.m[3][0] = 0; matrix.m[3][1] = 0; matrix.m[3][2] = 0; matrix.m[3][3] = 1;
    return matrix;
}

inline CcoMatrix4X4 ccoMultiplyMatrix4X4_Matrix4X4(const CcoMatrix4X4 matrix4x4_a, const CcoMatrix4X4 matrix4x4_b) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            result.m[i][j] = matrix4x4_a.m[0][j] * matrix4x4_b.m[i][0] +
                        matrix4x4_a.m[1][j] * matrix4x4_b.m[i][1] +
                        matrix4x4_a.m[2][j] * matrix4x4_b.m[i][2] +
                        matrix4x4_a.m[3][j] * matrix4x4_b.m[i][3];
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
    result.m[1][1] = cos(radians);
    result.m[1][2] = -sin(radians);
    result.m[2][1] = sin(radians);
    result.m[2][2] = cos(radians);
    return result;
}

inline CcoMatrix4X4 ccoCreateYRotationMatrix4X4(const CcoRadians radians) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    result.m[0][0] = cos(radians);
    result.m[0][2] = -sin(radians);
    result.m[2][0] = cos(radians);
    result.m[2][2] = sin(radians);
    return result;
}

inline CcoMatrix4X4 ccoCreateZRotationMatrix4X4(const CcoRadians radians) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    result.m[0][0] = cos(radians);
    result.m[0][1] = -sin(radians);
    result.m[1][0] = sin(radians);
    result.m[1][1] = cos(radians);
    return result;
}

inline CcoMatrix4X4 ccoCreateRotationMatrix4X4(const CcoVector3 rotation) {
    const CcoMatrix4X4 xRotation = ccoCreateXRotationMatrix4X4(rotation.x);
    const CcoMatrix4X4 yRotation = ccoCreateYRotationMatrix4X4(rotation.y);
    const CcoMatrix4X4 zRotation = ccoCreateZRotationMatrix4X4(rotation.z);
    return ccoMultiplyMatrix4X4_Matrix4X4(xRotation,
        ccoMultiplyMatrix4X4_Matrix4X4(yRotation, zRotation));
}

inline CcoMatrix4X4 ccoCreateScaleMatrix4x4(const CcoVector3 scale) {
    CcoMatrix4X4 result = ccoCreateMatrix4X4();
    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;
    return result;
}

inline void ccoConvertMatrix4X4ToFloatArray(const CcoMatrix4X4 matrix4x4, f32* result) {
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            result[i * 4 + j] = matrix4x4.m[i][j];
        }
    }
}