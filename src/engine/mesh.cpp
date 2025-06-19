#include "mesh.hpp"
#include <iostream>

bool Mesh::Load(const std::filesystem::path& file_path) {
    Assimp::Importer importer;

    // Read file with Assimp
    const aiScene* scene = importer.ReadFile(
        file_path.string(),
        aiProcess_Triangulate |           // Convert polygons to triangles
        aiProcess_GenNormals |            // Generate normals if missing
        aiProcess_FlipUVs |               // Flip UVs if needed (for OpenGL)
        aiProcess_CalcTangentSpace        // Calculate tangents (optional)
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return false;
    }

    // Clear previous data
    m_vertices.clear();
    m_indices.clear();

    // Process all meshes in the file
    for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++) {
        const aiMesh* mesh = scene->mMeshes[meshIdx];

        // Vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;

            // Position
            vertex.Position = {
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            };

            // Normal (if available)
            if (mesh->HasNormals()) {
                vertex.Normal = {
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                };
            }
            else {
                vertex.Normal = { 0.0f, 0.0f, 0.0f }; // Default normal
            }

            // Texture coordinates (if available)
            if (mesh->mTextureCoords[0]) {
                vertex.TextureCoord = {
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                };
            }
            else {
                vertex.TextureCoord = { 0.0f, 0.0f }; // Default UVs
            }

            // Vertex color (if available, default to white)
            if (mesh->HasVertexColors(0)) {
                vertex.Color = {
                    mesh->mColors[0][i].r,
                    mesh->mColors[0][i].g,
                    mesh->mColors[0][i].b,
                    mesh->mColors[0][i].a
                };
            }
            else {
                vertex.Color = { 1.0f, 1.0f, 1.0f, 1.0f }; // Default: white
            }

            m_vertices.push_back(vertex);
        }

        // Indices (faces)
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            const aiFace& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                m_indices.push_back(face.mIndices[j]);
            }
        }
    }

    std::cout << "Loaded mesh: " << file_path << std::endl;
    std::cout << "Vertices: " << m_vertices.size() << ", Indices: " << m_indices.size() << std::endl;
    return true;
}

// Optional: Print mesh data for debugging
void Mesh::print() const {
    std::cout << "Mesh Data:" << std::endl;
    for (size_t i = 0; i < m_vertices.size(); i++) {
        const Vertex& v = m_vertices[i];
        std::cout << "Vertex " << i << ": " << std::endl;
        std::cout << "  Position: (" << v.Position.x << ", " << v.Position.y << ", " << v.Position.z << ")" << std::endl;
        std::cout << "  Normal: (" << v.Normal.x << ", " << v.Normal.y << ", " << v.Normal.z << ")" << std::endl;
        std::cout << "  UV: (" << v.TextureCoord.x << ", " << v.TextureCoord.y << ")" << std::endl;
        std::cout << "  Color: (" << v.Color.r << ", " << v.Color.g << ", " << v.Color.b << ", " << v.Color.a << ")" << std::endl;
    }
    for (size_t i = 0; i < m_indices.size(); i += 3) {
        std::cout << "Triangle: " << m_indices[i] << ", " << m_indices[i + 1] << ", " << m_indices[i + 2] << std::endl;
    }
}