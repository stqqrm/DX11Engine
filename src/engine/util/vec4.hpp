#pragma once
#include <iostream>

struct vec4 {
    float x, y, z, w;

    constexpr vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) {}
    constexpr vec4(const vec4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
    ~vec4(void) = default;

    vec4& operator=(const vec4& other);
    vec4 operator+(const vec4& other) const;
    vec4 operator-(const vec4& other) const;
    vec4 operator*(float scalar) const;
    vec4 operator/(float scalar) const;
    float dot(const vec4& other) const;
    float magnitude() const;
    vec4 normalize() const;
    void print() const;
    float& operator[](int index);
    const float& operator[](int index) const;
};
