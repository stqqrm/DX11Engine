#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

bool Texture::Load(const fs::path& path) {
    // Load the texture image from the file using stb_image
    int width, height, numChannels;

    // stbi_load loads the image and provides width, height, and the number of channels (RGB, RGBA, etc.)
    unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &numChannels, 0);
    if (data) {
        // Successfully loaded the image, store the details in the class
        m_width = width;
        m_height = height;
        m_numChannels = numChannels;

        // Copy the image data into a vector for easy manipulation later
        m_data.assign(data, data + (width * height * numChannels));

        // Free the image data after loading
        stbi_image_free(data);

        return true;  // Loading was successful
    }
    else {
        std::cerr << "Failed to load texture image from: " << path << std::endl;
        return false;  // Loading failed
    }
}