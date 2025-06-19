#pragma once
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

struct mat4x4 {
    vec4 rows[4];
    mat4x4(void);
    mat4x4(const vec4& row0, const vec4& row1, const vec4& row2, const vec4& row3);
    mat4x4 operator*(const mat4x4& other) const;
    vec4 operator*(const vec4& vec) const;
    mat4x4 transpose(void) const;
    void rotate_x(float angleDegrees);
    void rotate_y(float angleDegrees);
    void rotate_z(float angleDegrees);
    void print(void) const;

    // Static functions for transformation
    static mat4x4 identity(void);
    static mat4x4 RotateX(float angleDegrees);
    static mat4x4 RotateY(float angleDegrees);
    static mat4x4 RotateZ(float angleDegrees);
    static mat4x4 Translation(const vec3& translation);
    static mat4x4 Scaling(const vec3& scale);
    static vec3 Transform(const mat4x4& mat, const vec3& vec);
    static mat4x4 PerspectiveFovLH(float fovy, float aspect, float zn, float zf);
    static mat4x4 LookAtLH(const vec3& eye, const vec3& target, const vec3& up);
};