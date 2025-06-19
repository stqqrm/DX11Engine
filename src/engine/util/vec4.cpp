#include "vec4.hpp"
#include "mat4x4.hpp"
#include <stdexcept>
#include <cmath>

// Assignment operator
vec4& vec4::operator=(const vec4& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
    return *this;
}

// Addition operator
vec4 vec4::operator+(const vec4& other) const {
    return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

// Subtraction operator
vec4 vec4::operator-(const vec4& other) const {
    return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}

// Multiplication by scalar
vec4 vec4::operator*(float scalar) const {
    return vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

// Division by scalar
vec4 vec4::operator/(float scalar) const {
    if (scalar == 0.0f) {
        throw std::runtime_error("Division by zero");
    }
    return vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}

// Dot product
float vec4::dot(const vec4& other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

// Magnitude
float vec4::magnitude() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

// Normalize
vec4 vec4::normalize() const {
    float mag = magnitude();
    if (mag == 0.0f) {
        throw std::runtime_error("Cannot normalize a zero vector");
    }
    return vec4(x / mag, y / mag, z / mag, w / mag);
}

// Print vector
void vec4::print() const {

}

// Access components by index
float& vec4::operator[](int index) {
    if (index < 0 || index > 3) {
        throw std::out_of_range("Index out of range");
    }
    return *(&x + index);
}

const float& vec4::operator[](int index) const {
    if (index < 0 || index > 3) {
        throw std::out_of_range("Index out of range");
    }
    return *(&x + index);
}