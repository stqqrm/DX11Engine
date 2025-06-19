#include "mat4x4.hpp"
#include <iostream>

// Default constructor
mat4x4::mat4x4() : rows{ vec4(), vec4(), vec4(), vec4() } {}

// Parameterized constructor
mat4x4::mat4x4(const vec4& row0, const vec4& row1, const vec4& row2, const vec4& row3) : rows{ row0, row1, row2, row3 } {}

// Identity matrix
mat4x4 mat4x4::identity() {
    return mat4x4(
        vec4(1.0f, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, 1.0f, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

// Matrix multiplication
mat4x4 mat4x4::operator*(const mat4x4& other) const {
    mat4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.rows[i][j] = rows[i].x * other.rows[0][j] +
                rows[i].y * other.rows[1][j] +
                rows[i].z * other.rows[2][j] +
                rows[i].w * other.rows[3][j];
        }
    }
    return result;
}

// Vector multiplication
vec4 mat4x4::operator*(const vec4& vec) const {
    return vec4(
        rows[0].x * vec.x + rows[0].y * vec.y + rows[0].z * vec.z + rows[0].w * vec.w,
        rows[1].x * vec.x + rows[1].y * vec.y + rows[1].z * vec.z + rows[1].w * vec.w,
        rows[2].x * vec.x + rows[2].y * vec.y + rows[2].z * vec.z + rows[2].w * vec.w,
        rows[3].x * vec.x + rows[3].y * vec.y + rows[3].z * vec.z + rows[3].w * vec.w
    );
}

// Transpose
mat4x4 mat4x4::transpose() const {
    return mat4x4(
        vec4(rows[0].x, rows[1].x, rows[2].x, rows[3].x),
        vec4(rows[0].y, rows[1].y, rows[2].y, rows[3].y),
        vec4(rows[0].z, rows[1].z, rows[2].z, rows[3].z),
        vec4(rows[0].w, rows[1].w, rows[2].w, rows[3].w)
    );
}

void mat4x4::rotate_x(float angle) {
    //*this = mat4x4::identity() * RotateX(angle);
    *this = *this * RotateX(angle);
}

void mat4x4::rotate_y(float angle) {
    //*this = mat4x4::identity() * RotateY(angle);
    *this = *this * RotateY(angle);
}

void mat4x4::rotate_z(float angle) {
    //*this = mat4x4::identity() * RotateZ(angle);
    *this = *this * RotateZ(angle);
}

// Translation
mat4x4 mat4x4::Translation(const vec3& translation) {
    return mat4x4(
        vec4(1.0f, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, 1.0f, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec4(translation.x, translation.y, translation.z, 1.0f)
    );
}

// Scaling
mat4x4 mat4x4::Scaling(const vec3& scale) {
    return mat4x4(
        vec4(scale.x, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, scale.y, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, scale.z, 0.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

// Transform vec3 by matrix
vec3 mat4x4::Transform(const mat4x4& mat, const vec3& vec) {
    vec4 result = mat * vec4(vec.x, vec.y, vec.z, 1.0f);
    // Divide by w if it's not 1 (for perspective correctness)
    if (result.w != 1.0f && result.w != 0.0f) {
        return vec3(result.x / result.w, result.y / result.w, result.z / result.w);
    }
    return vec3(result.x, result.y, result.z);
}

mat4x4 mat4x4::PerspectiveFovLH(float fovDegrees, float aspect, float zn, float zf) {
    float r = fovDegrees * (3.1415926535f / 180.0f); // Convert degrees to radians
    float yScale = 1.0f / tanf(r / 2.0f);
	float xScale = yScale / aspect;
	return mat4x4(
		vec4(xScale, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, yScale, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, zf / (zf - zn), 1.0f),
		vec4(0.0f, 0.0f, -zn * zf / (zf - zn), 0.0f));
}

mat4x4 mat4x4::LookAtLH(const vec3& eye, const vec3& target, const vec3& up) {
	vec3 zAxis = (target - eye).normalize();
	vec3 xAxis = up.cross(zAxis).normalize();
	vec3 yAxis = zAxis.cross(xAxis);
	return mat4x4(
        vec4(xAxis.x, xAxis.y, xAxis.z, 0.0f),  // First column: xAxis
        vec4(yAxis.x, yAxis.y, yAxis.z, 0.0f),  // Second column: yAxis
        vec4(zAxis.x, zAxis.y, zAxis.z, 0.0f),  // Third column: zAxis
        vec4(-eye.dot(xAxis), -eye.dot(yAxis), -eye.dot(zAxis), 1.0f));
}

mat4x4 mat4x4::RotateX(float angleDegrees) {
    float r = angleDegrees * (3.1415926535f / 180.0f); // Convert degrees to radians
    float c = cosf(r);
    float s = sinf(r);
    return mat4x4(
        vec4(1.0f, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, c, -s, 0.0f),
        vec4(0.0f, s, c, 0.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

mat4x4 mat4x4::RotateY(float angleDegrees) {
    float r = angleDegrees * (3.1415926535f / 180.0f); // Convert degrees to radians
    float c = cosf(angleDegrees);
    float s = sinf(angleDegrees);
    return mat4x4(
        vec4(c, 0.0f, s, 0.0f),
        vec4(0.0f, 1.0f, 0.0f, 0.0f),
        vec4(-s, 0.0f, c, 0.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

mat4x4 mat4x4::RotateZ(float angleDegrees) {
    float r = angleDegrees * (3.1415926535f / 180.0f); // Convert degrees to radians
    float c = cosf(r);
    float s = sinf(r);
    return mat4x4(
        vec4(c, -s, 0.0f, 0.0f),
        vec4(s, c, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

// Print matrix
void mat4x4::print() const {
    for (int i = 0; i < 4; ++i) {
        rows[i].print();
    }
}


mat4x4 MatrixPerspectiveFovLH(float fovy, float aspect, float zn, float zf) {
    float yScale = 1.0f / tanf(fovy / 2.0f);
    float xScale = yScale / aspect;
    return mat4x4(
        vec4(xScale, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, yScale, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, zf / (zf - zn), 1.0f),
        vec4(0.0f, 0.0f, -zn * zf / (zf - zn), 0.0f)
    );
}

mat4x4 MatrixLookToLH(const vec4& eye, const vec4& direction, const vec4& up) {
    return mat4x4();
}

mat4x4 MatrixLookToLH(const vec3& eye, const vec3& direction, const vec3& up) {
    vec3 zAxis = direction.normalize();
    vec3 xAxis = up.cross(zAxis).normalize();
    vec3 yAxis = zAxis.cross(xAxis);
    return mat4x4(
        vec4(xAxis.x, yAxis.x, zAxis.x, 0.0f),
        vec4(xAxis.y, yAxis.y, zAxis.y, 0.0f),
        vec4(xAxis.z, yAxis.z, zAxis.z, 0.0f),
        vec4(-eye.dot(xAxis), -eye.dot(yAxis), -eye.dot(zAxis), 1.0f)
    );
}

mat4x4 MatrixLookAtLH(const vec3& eye, const vec3& target, const vec3& up) {
    vec3 zAxis = (eye - target).normalize();
    vec3 xAxis = up.cross(zAxis).normalize();
    vec3 yAxis = zAxis.cross(xAxis);
    return mat4x4(
        vec4(xAxis.x, yAxis.x, zAxis.x, 0.0f),
        vec4(xAxis.y, yAxis.y, zAxis.y, 0.0f),
        vec4(xAxis.z, yAxis.z, zAxis.z, 0.0f),
        vec4(-eye.dot(xAxis), -eye.dot(yAxis), -eye.dot(zAxis), 1.0f)
    );
}
