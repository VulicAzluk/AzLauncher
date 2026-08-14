#pragma once

#include <URenderInfo.hpp>
#include <UTimer.hpp>
#include <ULogger.hpp>
#include <UWinScreen.hpp>
#include <UAnchor.hpp>
#include <UColor.hpp>
#include <UObject.hpp>
#include <URect.hpp>
#include <URenderScene.hpp>
#include <UApplication.hpp>
#include <UWindowAttribute.hpp>
#include <UWindowInfo.hpp>
#include <UApplicationInfo.hpp>
#include <UExtensions/AzPragmaComments.hpp>


#ifdef NDEBUG
    inline constexpr bool enabled_vulkan_debug = false;
#else
    inline constexpr bool enabled_vulkan_debug = true;
#endif


static uts::str get_env(const char* name) {
    char* buffer = nullptr;
    size_t size = 0;
    _dupenv_s(&buffer, &size, name);
    if (!buffer) return {};
    uts::str value = uts::str(buffer);
    free(buffer);
    return value;
}

inline std::filesystem::path get_cache_path() {
    uts::str local_appdata = get_env("LOCALAPPDATA");
    if (local_appdata.empty()) ULogger::ulixerr("LOCALAPPDATA not set");

    std::filesystem::path cache_dir = std::filesystem::path(local_appdata) / "AzLauncher" / "cache";
    std::error_code error_code;

    if (!std::filesystem::create_directories(cache_dir, error_code))
        if (error_code) ULogger::ulixerr("Failed to create cache directory: {}", error_code.message());

    return cache_dir;
}

namespace ApplicationSetup {
    inline UWindowInfo window_info;
    inline UApplicationInfo application_info;
    inline URenderInfo render_info;

    inline void initialize(UApplication::RenderCallback render_callback, uts::vec<UPixmap> texture_images) {
        UApplication::initialize();

        window_info = UWindowInfo("AzLauncher", "AzLauncherWindowClass")
            .without_attribute(UWindowAttribute::Titled)
            .with_initial_rect(UWinScreen::get_rect().create_with_percent_of_width(0.8, 0.625));
        application_info = UApplicationInfo("AzLauncher", (get_cache_path() / "cache.bin").string())
            .with_version(1, 0, 0)
            .with_vulkan_debug(enabled_vulkan_debug);
        render_info = URenderInfo(render_callback)
            .with_texture_images(texture_images);
    }

    inline int execute(UApplication::TickTimer tick_timer) {
        UApplication application = UApplication(application_info, window_info, render_info);
        application.set_window_rect(UWinScreen::get_rect().create_center_rect(application.get_window_rect()));
        application.set_tick_timer(tick_timer);

        application.show_window();
        return application.execute();
    }
}
