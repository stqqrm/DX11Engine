#include "util.hpp"

std::vector<char> ReadFile(const std::string& file) {
    std::ifstream stream(file, std::ios::ate | std::ios::binary);

    if (!stream.is_open()) {
        printf("Failed to open file: %s", file.c_str());
        return std::vector<char>();
    }

    size_t fileSize = (size_t)stream.tellg();
    std::vector<char> buffer(fileSize);

    stream.seekg(0);
    stream.read(buffer.data(), fileSize);
    stream.close();

    return buffer;
}
