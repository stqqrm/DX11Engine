#pragma once
// std
#include <iostream>
#include <vector>
#include <filesystem>
#include <tuple>
// api
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib") // IID_ID3D11ShaderReflection
// extern
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#pragma comment(lib, "glfw3.lib")

namespace fs = std::filesystem;
namespace wrl = Microsoft::WRL;
