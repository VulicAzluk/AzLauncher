#pragma once

#include <UTypes.hpp>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace UWin32Function {
    inline static const UINT system_dpi = GetDpiForSystem();
    inline static const HINSTANCE hinstance = GetModuleHandleW(nullptr);

    inline constexpr auto logical_pixels_to_physical(uts::f32 logical_pixels, UINT dpi = system_dpi) -> uts::f32 {
        return logical_pixels * (dpi / 96.0f);
    }

    inline constexpr auto physical_pixels_to_logical(uts::f32 physical_pixels, UINT dpi = system_dpi) -> uts::f32 {
        return physical_pixels * (96.0f / dpi);
    }

    inline constexpr auto make_int_resource(WORD id) -> LPCWSTR {
        return reinterpret_cast<LPCWSTR>(static_cast<ULONG_PTR>(id));
    }
}
