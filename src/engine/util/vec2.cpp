#include "vec2.hpp"


float vec2::dot(const vec2& rhs) const {
    return x * rhs.x + y * rhs.y;
}
float vec2::magnitude(void) const {
    return std::sqrt(x * x + y * y);
}
vec2 vec2::normalized(void) const {
    float mag = magnitude();
    return mag > 0 ? vec2(x / mag, y / mag) : vec2(0.0f, 0.0f);
}
vec2& vec2::operator=(const vec2& rhs) const {
    return (vec2&)*this;
}
vec2 vec2::operator+(const vec2& rhs) const {
    return vec2(x + rhs.x, y + rhs.y);
}
vec2 vec2::operator-(const vec2& rhs) const {
    return vec2(x - rhs.x, y - rhs.y);
}
vec2 vec2::operator*(float scalar) const {
    return vec2(x * scalar, y * scalar);
}
vec2 vec2::operator/(float scalar) const {
    return vec2(x / scalar, y / scalar);
}
bool vec2::operator==(const vec2& rhs) const {
    return x == rhs.x && y == rhs.y;
}
bool vec2::operator!=(const vec2& rhs) const {
    return !(*this == rhs);
}
vec2& vec2::operator+=(const vec2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}
vec2& vec2::operator-=(const vec2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}
vec2& vec2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}
vec2& vec2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}