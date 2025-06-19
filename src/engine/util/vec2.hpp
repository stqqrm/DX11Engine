#pragma once
#include <cmath>


struct vec2 {
    constexpr vec2(const vec2& v) : x(v.x), y(v.y) {}
    constexpr vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    ~vec2() = default;
    float dot(const vec2& rhs) const;
    float magnitude(void) const;
    vec2 normalized(void) const;
    vec2& operator=(const vec2& rhs) const;
    vec2 operator+(const vec2& rhs) const;
    vec2 operator-(const vec2& rhs) const;
    vec2 operator*(float scalar) const;
    vec2 operator/(float scalar) const;
    bool operator==(const vec2& rhs) const;
    bool operator!=(const vec2& rhs) const;
    vec2& operator+=(const vec2& rhs);
    vec2& operator-=(const vec2& rhs);
    vec2& operator*=(float scalar);
    vec2& operator/=(float scalar);
    float x, y;
};