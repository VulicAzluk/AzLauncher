#pragma once

#include <URenderInfo.hpp>
#include <UTimer.hpp>
#include <ULogger.hpp>
#include <UScreen.hpp>
#include <UAnchor.hpp>
#include <UColor.hpp>
#include <UObject.hpp>
#include <URect.hpp>
#include <UScene.hpp>
#include <UApp.hpp>
#include <UWindowAttribute.hpp>
#include <UWindowInfo.hpp>
#include <UAppInfo.hpp>
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

namespace AppSetup {
    inline UWindowInfo window_info;
    inline UAppInfo appinfo;
    inline URenderInfo render_info;

    inline void init(UApp::RenderCallback render_callback, uts::vec<UPixmap> texture_images) {
        UApp::init();

        window_info = UWindowInfo("AzLauncher", "AzLauncherWindowClass")
            .unattrs(UWindowAttrs::Titled)
            .rect(UScreen::get_rect().new_with_widthper(0.8, 0.625));
        appinfo = UAppInfo("AzLauncher", (get_cache_path() / "cache.bin").string())
            .version(1, 0, 0)
            .vkdbg(enabled_vulkan_debug);
        render_info = URenderInfo(render_callback)
            .textures(texture_images);
    }

    inline int exec(UApp::TickTimer tick_timer) {
        UApp application = UApp(appinfo, window_info, render_info);
        application.set_rect(UScreen::get_rect().new_center(application.rect()));
        application.set_timer(tick_timer);

        application.show();
        return application.exec();
    }
}
