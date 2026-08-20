#pragma once

#include <ULogger.hpp>
#include <UTypes.hpp>
#include <filesystem>
#include <fstream>


class UFile {
    private:
        uts::str file_path;

    public:
        UFile() = default;
        UFile(const uts::str file_path): file_path(file_path) {}
        bool operator==(const UFile&) const = default;

        inline auto read_string() const -> uts::str {
            std::ifstream file_handle = std::ifstream(file_path);
            return uts::str(std::istreambuf_iterator<char>(file_handle), std::istreambuf_iterator<char>());
        }

        inline auto read_bytes() const -> uts::bytes {
            std::ifstream file_handle = std::ifstream(file_path, std::ios::binary);
            return uts::bytes(std::istreambuf_iterator<char>(file_handle), std::istreambuf_iterator<char>());
        }

        inline auto exists() const -> bool { return std::filesystem::exists(file_path); }
        inline auto suffix() const -> uts::str { return std::filesystem::path(file_path).extension().string(); }

    public:
        inline auto get_file_path() const -> const uts::str& { return file_path; }
};
