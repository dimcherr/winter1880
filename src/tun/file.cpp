#include "file.h"
#include "tun/log.h"
#include <cstddef>
#include <fstream>
#include <filesystem>
#include <iterator>

String tun::ReadFile(StringView path) {
    auto size = std::filesystem::file_size(path);
    String content(size, '\0');
    std::ifstream in(path.data());
    in.read(&content[0], size);
    return std::move(content);
}

Bytes tun::ReadFileBinary(StringView path) {
    std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        return {};
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    Bytes buffer(size);
    if (size > 0 && !file.read(buffer.data(), size)) {
        tun::error("Failed to read file: {}", path);
        return {};
    }
    
    return buffer;
}

bool tun::WriteFile(StringView path, StringView content) {
    std::ofstream file(path.data());
    if (!file.is_open()) {
        return false;
    }
    if (content.size() > 0 && !file.write(content.data(), content.size())) {
        return false;
    }
    return true;
}