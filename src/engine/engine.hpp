#pragma once
#include "pipeline.hpp"

class Engine : public Pipeline {
public:
    Engine();
    ~Engine(void);
    void Clear(float r, float g, float b, float a = 1.0f);
    void Present(bool vsync);
    void DrawMesh(const Mesh& mesh);
    inline Camera& GetCamera() const { return (Camera&)m_camera; }
    inline Engine& GetThis(void) const { return (Engine&)*this; }
private:
	GLFWwindow*     m_window;
    Camera          m_camera;
    // Store meshes
    std::vector<wrl::ComPtr<ID3D11ShaderResourceView>>   m_textures;             // Store loaded textures
    std::vector<wrl::ComPtr<ID3D11Buffer>>               m_vertexBuffers;        // Store vertex buffers for models
    std::vector<wrl::ComPtr<ID3D11Buffer>>               m_indexBuffers;         // Store index buffers for models
    std::vector<UINT>                                    m_indexCounts;

    std::unordered_map<const Mesh*,
        std::pair<wrl::ComPtr<ID3D11Buffer>,
        wrl::ComPtr<ID3D11Buffer>>> m_meshBuffers;
};
