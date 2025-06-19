#pragma once
#include "mat4x4.hpp"
#include "math.hpp"
#include <vector>
#include <string>
#include <fstream>

#ifdef UNICODE
    #define tstring wstring
    #define tchar wchar_t
    #define _T(str) (L##str)
#else
    #define tstring string
    #define tchar char
    #define _T(str) (str)
#endif

std::vector<char> ReadFile(const std::string& file);
