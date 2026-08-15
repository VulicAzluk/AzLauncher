#pragma once

#include <UWin32Functions.hpp>
#include <URect.hpp>
#include <UTypes.hpp>

class UScreen {
    public:
        inline static auto get_rect() -> URect {
            return URect(0, 0,
                UWin32Functions::logical_cast(GetSystemMetricsForDpi(SM_CXSCREEN, UWin32Functions::system_dpi)),
                UWin32Functions::logical_cast(GetSystemMetricsForDpi(SM_CYSCREEN, UWin32Functions::system_dpi)));
        }
        
        inline static auto get_dpi() -> uts::u32 {
            return UWin32Functions::system_dpi;
        }
};
