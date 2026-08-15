#pragma once

#include <UFile.hpp>
#include <UTypes.hpp>

class UAppInfo {
    private:
        uts::str application_id;
        UFile cache_file_path;
        uts::u32 application_version;
        bool enabled_vulkan_debug;

    public:
        UAppInfo() = default;
        inline constexpr UAppInfo(const uts::str& application_id, const UFile& cache_file_path):
            application_id(application_id),
            cache_file_path(cache_file_path),
            application_version(1),
            enabled_vulkan_debug(true)
        {}

        inline auto version(uts::u32 major, uts::u32 minor, uts::u32 patch) -> UAppInfo& {
            application_version = (major << 22) | (minor << 12) | patch;

            return *this;
        }

        inline auto vkdbg(bool enabled = true) -> UAppInfo& {
            enabled_vulkan_debug = enabled;

            return *this;
        }

    public:
        inline auto get_application_id() const -> const uts::str& { return application_id; }
        inline auto get_application_version() const -> uts::u32 { return application_version; }
        inline auto get_pipeline_cache_file_path() const -> const UFile& { return cache_file_path; }
        inline auto get_enabled_vulkan_debug() const -> bool { return enabled_vulkan_debug; }
};
