#pragma once
#include <vector>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib, "assimp-vc143-mt.lib")

#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoord;
    glm::vec4 Color;
};

using Index = unsigned int;

class Mesh {
public:
    bool Load(const std::filesystem::path& file_path);
    inline const std::vector<Vertex>& GetVertices() const { return m_vertices; }
    inline const std::vector<Index>& GetIndices() const { return m_indices; }
	void print(void) const;
private:
    std::vector<Vertex> m_vertices;
    std::vector<Index> m_indices;
};

