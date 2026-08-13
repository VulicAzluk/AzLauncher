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
        UFile(const uts::str file_path, bool create = false) {
            if (!std::filesystem::exists(file_path)) {
                if (!create) ULogger::ulixerr("File {} doesn't exists", file_path);
                std::ofstream file_handle = std::ofstream(file_path);
                file_handle << "";
            }
            this->file_path = file_path;
        }

    public:
        inline auto get_file_path() const -> const uts::str& { return file_path; }
};
