#pragma once

#include <UTypes.hpp>

class UColor {
    private:
        uts::f32 red, green, blue, alpha;

    public:
        UColor() = default;
        inline constexpr UColor(uts::f32 red, uts::f32 green, uts::f32 blue, uts::f32 alpha = 255):
            red(red), green(green), blue(blue), alpha(alpha) {}

    public:
        constexpr auto to_rgba() const -> uts::u32 { return (static_cast<uts::u32>(red) << 24) | (static_cast<uts::u32>(green) << 16) | (static_cast<uts::u32>(blue) << 8) | static_cast<uts::u32>(alpha); }
        constexpr auto to_argb() const -> uts::u32 { return (static_cast<uts::u32>(alpha) << 24) | (static_cast<uts::u32>(red) << 16) | (static_cast<uts::u32>(green) << 8) | static_cast<uts::u32>(blue); }
        constexpr auto to_bgra() const -> uts::u32 { return (static_cast<uts::u32>(blue) << 24) | (static_cast<uts::u32>(green) << 16) | (static_cast<uts::u32>(red) << 8) | static_cast<uts::u32>(alpha); }
        constexpr auto to_abgr() const -> uts::u32 { return (static_cast<uts::u32>(alpha) << 24) | (static_cast<uts::u32>(blue) << 16) | (static_cast<uts::u32>(green) << 8) | static_cast<uts::u32>(red); }
        constexpr auto to_bgr() const -> uts::u32 { return (static_cast<uts::u32>(blue) << 16) | (static_cast<uts::u32>(green) << 8) | static_cast<uts::u32>(red); }
        constexpr auto to_rgb() const -> uts::u32 { return (static_cast<uts::u32>(red) << 16) | (static_cast<uts::u32>(green) << 8) | static_cast<uts::u32>(blue); }

        auto get_red() const -> uts::f32 { return red; }
        auto get_green() const -> uts::f32 { return green; }
        auto get_blue() const -> uts::f32 { return blue; }
        auto get_alpha() const -> uts::f32 { return alpha; }
};
