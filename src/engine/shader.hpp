#pragma once
// std
#include <vector>
#include <filesystem>
// api
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

/*

class Shader {
public:
	Shader() = default;
	Shader(wrl::ComPtr<ID3DBlob> blob, wrl::ComPtr<ID3D11VertexShader> shader) {
		m_shader = shader;
		m_blob = blob;
	}
	Shader(wrl::ComPtr<ID3DBlob> blob, wrl::ComPtr<ID3D11PixelShader> shader) {
		m_shader = shader;
		m_blob = blob;
	}
	wrl::ComPtr<ID3DBlob>& GetBlob() const { return m_blob; }
	template<typename T>
	wrl::ComPtr<T>& GetShader() const { return static_cast<wrl::ComPtr<T>>(m_shader); }
private:
	wrl::ComPtr<ID3DBlob> m_blob;
	wrl::ComPtr<void*> m_shader;
};
*/