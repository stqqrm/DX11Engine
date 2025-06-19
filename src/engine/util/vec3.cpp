#include "vec3.hpp"
#include <stdexcept>
#include <cmath>

#include <iostream> // cout

float vec3::dot(const vec3& rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

vec3 vec3::cross(const vec3& rhs) const {
    return vec3(
        y * rhs.z - z * rhs.y,
        z * rhs.x - x * rhs.z,
        x * rhs.y - y * rhs.x
    );
}

float vec3::magnitude(void) const {
    return std::sqrt(x * x + y * y + z * z);
}

vec3 vec3::normalize(void) const {
    float mag = magnitude();
    return vec3(x / mag, y / mag, z / mag);
}

void vec3::print(void) const {
    printf("vec3(%f, %f, %f)\n", x, y, z);
    //std::cout << "vec3(" << x << ", " << y << ", " << z << ")" << std::endl;
}

vec3& vec3::operator=(const vec3& rhs) {
    if (this != &rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }
    return *this;
}

vec3 vec3::operator+(const vec3& rhs) const {
    return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

vec3 vec3::operator-(const vec3& rhs) const {
    return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

vec3 vec3::operator*(float scalar) const {
    return vec3(x * scalar, y * scalar, z * scalar);
}

vec3 vec3::operator/(float scalar) const {
    return vec3(x / scalar, y / scalar, z / scalar);
}

bool vec3::operator==(const vec3& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool vec3::operator!=(const vec3& rhs) const {
    return !(*this == rhs);
}

vec3& vec3::operator+=(const vec3& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

vec3& vec3::operator-=(const vec3& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

vec3& vec3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

vec3& vec3::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}