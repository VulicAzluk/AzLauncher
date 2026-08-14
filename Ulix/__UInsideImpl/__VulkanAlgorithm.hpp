#pragma once

#include <UColor.hpp>
#include <ULogger.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <fstream>

namespace __uii::vkalg {
    inline static constexpr glm::vec4 color_to_vec4(const UColor& color) {
        return glm::vec4(color.get_red() / 255.0f,
                         color.get_green() / 255.0f,
                         color.get_blue() / 255.0f,
                         color.get_alpha() / 255.0f);
    }

    inline auto read_cache(const uts::str& file_path) -> uts::vec<char> {
        std::ifstream file = std::ifstream(file_path, std::ios::ate | std::ios::binary);
        if (!file.is_open()) ULogger::ulixerr("Failed to read cache");
        size_t file_size = (size_t) file.tellg();
        uts::vec<char> buffer = uts::vec<char>(file_size);
        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), file_size);
        file.close();
        return buffer;
    }

    inline void write_cache(const uts::str& file_path, const uts::vec<char>& buffer) {
        std::ofstream file = std::ofstream(file_path, std::ios::binary);
        if (!file.is_open()) ULogger::ulixerr("Failed to write cache");
        file.write(buffer.data(), buffer.size());
        file.close();
    }

    inline void ensure_cache_exists(const uts::str& file_path) {
        if (!std::filesystem::exists(file_path))
            write_cache(file_path, {});
    }
}
