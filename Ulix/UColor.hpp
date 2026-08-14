#pragma once

#include <UTypes.hpp>

class UColor {
    private:
        uts::u8 red, green, blue, alpha;

    public:
        UColor() = default;
        inline constexpr UColor(uts::u8 red, uts::u8 green, uts::u8 blue, uts::u8 alpha = 255):
            red(red), green(green), blue(blue), alpha(alpha) {}

    public:
        constexpr auto to_rgba() const -> uts::u32 { return (static_cast<uts::u32>(red) << 24) | (static_cast<uts::u32>(green) << 16) | (static_cast<uts::u32>(blue) << 8) | static_cast<uts::u32>(alpha); }
        constexpr auto to_argb() const -> uts::u32 { return (static_cast<uts::u32>(alpha) << 24) | (static_cast<uts::u32>(red) << 16) | (static_cast<uts::u32>(green) << 8) | static_cast<uts::u32>(blue); }
        constexpr auto to_bgra() const -> uts::u32 { return (static_cast<uts::u32>(blue) << 24) | (static_cast<uts::u32>(green) << 16) | (static_cast<uts::u32>(red) << 8) | static_cast<uts::u32>(alpha); }
        constexpr auto to_abgr() const -> uts::u32 { return (static_cast<uts::u32>(alpha) << 24) | (static_cast<uts::u32>(blue) << 16) | (static_cast<uts::u32>(green) << 8) | static_cast<uts::u32>(red); }
        constexpr auto to_bgr() const -> uts::u32 { return (static_cast<uts::u32>(blue) << 16) | (static_cast<uts::u32>(green) << 8) | static_cast<uts::u32>(red); }
        constexpr auto to_rgb() const -> uts::u32 { return (static_cast<uts::u32>(red) << 16) | (static_cast<uts::u32>(green) << 8) | static_cast<uts::u32>(blue); }

        auto get_red() const -> std::uint8_t { return red; }
        auto get_green() const -> std::uint8_t { return green; }
        auto get_blue() const -> std::uint8_t { return blue; }
        auto get_alpha() const -> std::uint8_t { return alpha; }
};
