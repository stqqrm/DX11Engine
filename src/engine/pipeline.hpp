#pragma once
#include "inc.h"

#include "camera.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "shader.hpp"

class Pipeline {
public:
    Pipeline() = default;
    Pipeline(GLFWwindow* window);
    ~Pipeline();
    void Initialize(GLFWwindow* window);
    wrl::ComPtr<ID3D11Buffer> CreateVertexBuffer(const std::vector<Vertex>& vertices);
    wrl::ComPtr<ID3D11Buffer> CreateIndexBuffer(const std::vector<Index>& indices);
    template <typename _Buffer>
    wrl::ComPtr<ID3D11Buffer> CreateConstantBuffer();
    wrl::ComPtr<ID3D11ShaderResourceView> CreateTexture(const Texture& tex);
    std::tuple<wrl::ComPtr<ID3D11VertexShader>, wrl::ComPtr<ID3D11InputLayout>> CompileVertexShader(const fs::path& filePath, const std::string& entryPoint);
    wrl::ComPtr<ID3D11PixelShader> CompilePixelShader(const fs::path& filePath, const std::string& entryPoint);
    
    inline void SetVertexShader(wrl::ComPtr<ID3D11VertexShader> vertexShader) { m_deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0); }
    inline void SetPixelShader(wrl::ComPtr<ID3D11PixelShader> pixelShader) { m_deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0); }
    inline void SetInputLayout(wrl::ComPtr<ID3D11InputLayout> inputLayout) { m_deviceContext->IASetInputLayout(inputLayout.Get()); }
    inline void SetTexture(wrl::ComPtr<ID3D11ShaderResourceView> tex) { m_deviceContext->PSSetShaderResources(0, 1, tex.GetAddressOf()); }

    template <typename _Buffer>
    void UpdateConstantBuffer(wrl::ComPtr<ID3D11Buffer> d3dbuffer, const _Buffer& buffer, UINT _register);
    void Clear(float r, float g, float b, float a = 1.0f);
    void Present(bool vsync);
    void DrawMesh(const wrl::ComPtr<ID3D11Buffer>& vertexBuffer, const wrl::ComPtr<ID3D11Buffer>& indexBuffer, uint32_t indexCount);
    wrl::ComPtr<ID3D11Device> GetDevice() const;
    wrl::ComPtr<ID3D11DeviceContext> GetDeviceContext() const;
protected:
    GLFWwindow* m_window;
    wrl::ComPtr<ID3D11Device>            m_device;
    wrl::ComPtr<ID3D11DeviceContext>     m_deviceContext;
    wrl::ComPtr<IDXGISwapChain>          m_swapChain;
    wrl::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    wrl::ComPtr<ID3D11Texture2D>         m_depthStencilBuffer;
    wrl::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
    wrl::ComPtr<ID3D11RasterizerState>   m_rasterizerState;
    wrl::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
    wrl::ComPtr<ID3D11BlendState>        m_blendState;
private:
    void CreateDeviceAndSwapChain();
    void CreateRenderTargetView();
    void CreateDepthStencilBuffer();
    void SetViewport();
    void CreateRasterizerState();
    void CreateDepthStencilState();
    void CreateBlendState();
    template<typename _Shader>
    std::tuple<
        wrl::ComPtr<_Shader>,
        wrl::ComPtr<ID3D11InputLayout>> CompileShader(
        const fs::path& filePath,
        const std::string& entryPoint,
        const std::string& profile);
    wrl::ComPtr<ID3D11InputLayout> _CreateInputLayoutFromVertexShader(
        wrl::ComPtr<ID3DBlob> pShaderBlob);
};

template<typename _Buffer>
inline wrl::ComPtr<ID3D11Buffer> Pipeline::CreateConstantBuffer() {
    wrl::ComPtr<ID3D11Buffer> buffer;
    D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(_Buffer);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    HRESULT result = m_device->CreateBuffer(&matrixBufferDesc, nullptr, &buffer);
    if (FAILED(result)) {
        throw std::runtime_error("Failed to create buffer");
    }

    return buffer;
}

template<typename _Buffer>
inline void Pipeline::UpdateConstantBuffer(wrl::ComPtr<ID3D11Buffer> d3dbuffer, const _Buffer& buffer, UINT _register) {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    // Lock the constant buffer
    HRESULT hr = m_deviceContext->Map((ID3D11Resource*)d3dbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr)) {
        throw std::runtime_error("UpdateConstantBuffer, ->Map failed");
    }
    // Write data to constant buffer
    void* dataPtr = mappedResource.pData;
    memcpy(dataPtr, &buffer, sizeof(_Buffer));

    // Unlock the constant buffer
    m_deviceContext->Unmap((ID3D11Resource*)d3dbuffer.Get(), 0);

    m_deviceContext->VSSetConstantBuffers(_register, 1, d3dbuffer.GetAddressOf());
}

template<typename _Shader>
std::tuple<wrl::ComPtr<_Shader>, wrl::ComPtr<ID3D11InputLayout>> Pipeline::CompileShader(const fs::path& filePath, const std::string& entryPoint, const std::string& profile) {
    if (!fs::exists(filePath)) {
        throw std::runtime_error("Shader file not found: " + filePath.string());
    }

    wrl::ComPtr<ID3DBlob> shaderBlob;
    wrl::ComPtr<ID3DBlob> errorBlob;

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    std::wstring widePath = filePath.wstring();
    HRESULT hr = D3DCompileFromFile(
        widePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint.c_str(),
        profile.c_str(),
        flags,
        0,
        shaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf());

    if (FAILED(hr)) {
        if (errorBlob) {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            throw std::runtime_error("Shader compilation failed: " + std::string((char*)errorBlob->GetBufferPointer()));
        }
        throw std::runtime_error("Failed to compile shader");
    }

    wrl::ComPtr<_Shader> shader;
    wrl::ComPtr<ID3D11InputLayout> inputLayout;
    if constexpr (std::is_same_v<_Shader, ID3D11VertexShader>) {
        hr = m_device->CreateVertexShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            nullptr,
            shader.GetAddressOf());
        // Create input layout...
        inputLayout = _CreateInputLayoutFromVertexShader(shaderBlob);
    }
    else if constexpr (std::is_same_v<_Shader, ID3D11PixelShader>) {
        hr = m_device->CreatePixelShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            nullptr,
            shader.GetAddressOf());
    }

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create shader");
    }

    return std::make_tuple(shader, inputLayout);
}