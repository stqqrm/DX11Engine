#include "pipeline.hpp"
#include <stdexcept>
#include <fstream>

Pipeline::Pipeline(GLFWwindow* window) {
    Initialize(window);
}

Pipeline::~Pipeline() {

}

void Pipeline::Initialize(GLFWwindow* window) {
    m_window = window;
    CreateDeviceAndSwapChain();
    CreateRenderTargetView();
    CreateDepthStencilBuffer();
    SetViewport();
    CreateRasterizerState();
    CreateDepthStencilState();
    CreateBlendState();
}

wrl::ComPtr<ID3D11Buffer> Pipeline::CreateVertexBuffer(const std::vector<Vertex>& vertices) {
    D3D11_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd = { 0 };
    sd.pSysMem = vertices.data();

    wrl::ComPtr<ID3D11Buffer> vertexBuffer;
    HRESULT hr = m_device->CreateBuffer(&bd, &sd, vertexBuffer.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create vertex buffer");
    }

    return vertexBuffer;
}

wrl::ComPtr<ID3D11Buffer> Pipeline::CreateIndexBuffer(const std::vector<Index>& indices) {
    D3D11_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = static_cast<UINT>(sizeof(Index) * indices.size());
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd = { 0 };
    sd.pSysMem = indices.data();

    wrl::ComPtr<ID3D11Buffer> indexBuffer;
    HRESULT hr = m_device->CreateBuffer(&bd, &sd, indexBuffer.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create index buffer");
    }

    return indexBuffer;
}

std::tuple<wrl::ComPtr<ID3D11VertexShader>, wrl::ComPtr<ID3D11InputLayout>>
Pipeline::CompileVertexShader(const fs::path& filePath, const std::string& entryPoint) {
    return CompileShader<ID3D11VertexShader>(filePath, entryPoint, "vs_5_0");
}

wrl::ComPtr<ID3D11PixelShader>
    Pipeline::CompilePixelShader(const fs::path& filePath, const std::string& entryPoint) {
    return std::get<wrl::ComPtr<ID3D11PixelShader>>(
        CompileShader<ID3D11PixelShader>(filePath, entryPoint, "ps_5_0"));
}

/*
void Pipeline::SetVertexShader(wrl::ComPtr<ID3D11VertexShader> vertexShader) {
    m_deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
}

void Pipeline::SetPixelShader(wrl::ComPtr<ID3D11PixelShader> pixelShader) {
    m_deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
}

void Pipeline::SetInputLayout(wrl::ComPtr<ID3D11InputLayout> inputLayout) {
    m_deviceContext->IASetInputLayout(inputLayout.Get());
}
*/

void Pipeline::DrawMesh(const wrl::ComPtr<ID3D11Buffer>& vertexBuffer, const wrl::ComPtr<ID3D11Buffer>& indexBuffer, uint32_t indexCount) {
    // Set vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    // Set index buffer
    m_deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // Set primitive topology
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw indexed
    m_deviceContext->DrawIndexed(indexCount, 0, 0);
}

wrl::ComPtr<ID3D11Device> Pipeline::GetDevice() const {
    return m_device;
}

wrl::ComPtr<ID3D11DeviceContext> Pipeline::GetDeviceContext() const {
    return m_deviceContext;
}

void Pipeline::CreateDeviceAndSwapChain() {
    DXGI_SWAP_CHAIN_DESC scd = { 0 };
    ZeroMemory(&scd, sizeof(scd));
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;

    scd.OutputWindow = glfwGetWin32Window(m_window);
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        featureLevels,
        1,
        D3D11_SDK_VERSION,
        &scd,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        &featureLevel,
        m_deviceContext.GetAddressOf());

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create device and swap chain");
    }
}

void Pipeline::CreateRenderTargetView() {
    wrl::ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to get back buffer");
    }

    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create render target view");
    }
}

void Pipeline::CreateDepthStencilBuffer() {
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    D3D11_TEXTURE2D_DESC depthDesc = { 0 };
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    HRESULT hr = m_device->CreateTexture2D(&depthDesc, nullptr, m_depthStencilBuffer.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create depth stencil buffer");
    }

    hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, m_depthStencilView.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create depth stencil view");
    }

    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}

void Pipeline::SetViewport() {
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    D3D11_VIEWPORT viewport = { 0 };
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    m_deviceContext->RSSetViewports(1, &viewport);
}

void Pipeline::CreateRasterizerState() {
    D3D11_RASTERIZER_DESC rsDesc;
    ZeroMemory(&rsDesc, sizeof(rsDesc));
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_BACK;
    rsDesc.FrontCounterClockwise = FALSE;
    rsDesc.DepthClipEnable = TRUE;

    HRESULT hr = m_device->CreateRasterizerState(&rsDesc, m_rasterizerState.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create rasterizer state");
    }
    m_deviceContext->RSSetState(m_rasterizerState.Get());
}

void Pipeline::CreateDepthStencilState() {
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(dsDesc));
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    HRESULT hr = m_device->CreateDepthStencilState(&dsDesc, m_depthStencilState.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create depth stencil state");
    }
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
}

void Pipeline::CreateBlendState() {
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));
    blendDesc.RenderTarget[0].BlendEnable = FALSE;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRESULT hr = m_device->CreateBlendState(&blendDesc, m_blendState.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create blend state");
    }
    m_deviceContext->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);
}

wrl::ComPtr<ID3D11InputLayout> Pipeline::_CreateInputLayoutFromVertexShader(wrl::ComPtr<ID3DBlob> pShaderBlob) {
    // Reflect shader info
    wrl::ComPtr<ID3D11ShaderReflection> reflector = NULL;
    if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflector))) {
        throw std::runtime_error("D3DReflect");
    }

    // Get shader info
    D3D11_SHADER_DESC shaderDesc;
    reflector->GetDesc(&shaderDesc);

    // Read input layout description from shader info
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
    for (int i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        reflector->GetInputParameterDesc(i, &paramDesc);

        // fill out input element desc
        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticName = paramDesc.SemanticName;
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;
        elementDesc.InputSlot = 0;
        elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate = 0;

        // determine DXGI format
        if (paramDesc.Mask == 1)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
        }
        else if (paramDesc.Mask <= 3)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        else if (paramDesc.Mask <= 7)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        }
        else if (paramDesc.Mask <= 15)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

        //save element desc
        inputLayoutDesc.push_back(elementDesc);
    }

    // Try to create Input Layout
    wrl::ComPtr<ID3D11InputLayout> inputLayout;
    HRESULT hr = m_device->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), inputLayout.GetAddressOf());

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create input layout");
    }

    return inputLayout;
}
