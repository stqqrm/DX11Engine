#pragma once
#include "util.hpp"
#include <cmath>

#define PI 3.14159265358979323846f

constexpr float ConvertToRadians(float fDegrees) noexcept { return fDegrees * (PI / 180.0f); }
constexpr float ConvertToDegrees(float fRadians) noexcept { return fRadians * (180.0f / PI); }
