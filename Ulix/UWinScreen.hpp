#pragma once

#include <UWin32Function.hpp>
#include <URect.hpp>
#include <UTypes.hpp>

namespace UWinScreen {
    inline auto get_rect() -> URect {
        return URect(0, 0,
            UWin32Function::physical_pixels_to_logical(GetSystemMetricsForDpi(SM_CXSCREEN, UWin32Function::system_dpi)),
            UWin32Function::physical_pixels_to_logical(GetSystemMetricsForDpi(SM_CYSCREEN, UWin32Function::system_dpi)));
    }

    inline auto get_dpi() -> uts::u32 {
        return UWin32Function::system_dpi;
    }
}
