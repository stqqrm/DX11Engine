#include "engine.hpp"

Engine::Engine() : m_window(nullptr), m_camera() {
	m_camera.MoveAbsolute(glm::vec3(0.0f, -5.0f, 0.0f));
	m_camera.SetProjectionMatrix(90.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
}

Engine::~Engine(void) {
}

void Engine::Clear(float r, float g, float b, float a) {
    const float color[4] = { r, g, b, a };
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);
    m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Engine::Present(bool vsync) {
    m_swapChain->Present(vsync ? 1 : 0, 0);
}

void Engine::DrawMesh(const Mesh& mesh) {
    // Check if we already have buffers for this mesh
    auto it = m_meshBuffers.find(&mesh);
    if (it == m_meshBuffers.end()) {
        // Create new buffers
        auto vertexBuffer = Pipeline::CreateVertexBuffer(mesh.GetVertices());
        auto indexBuffer = Pipeline::CreateIndexBuffer(mesh.GetIndices());

        // Store in cache
        m_meshBuffers[&mesh] = { vertexBuffer, indexBuffer };
        it = m_meshBuffers.find(&mesh);
    }

    // Draw the mesh
    Pipeline::DrawMesh(it->second.first, it->second.second,
        static_cast<uint32_t>(mesh.GetIndices().size()));
}