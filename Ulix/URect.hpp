#pragma once

#include <UTypes.hpp>

class URect {
    public:
        enum Pixels: uts::i8 { Full = -1 };

    private:
        uts::f32 x, y;
        uts::f32 width, height;

    public:
        URect() = default;
        inline constexpr URect(uts::f32 x, uts::f32 y, uts::f32 width, uts::f32 height):
            x(x), y(y), width(width), height(height) {}
        bool operator==(const URect&) const = default;

    public:
        inline static auto new_pos(uts::f32 x, uts::f32 y) -> URect { return URect(x, y, 0, 0); }
        inline static auto new_size(uts::f32 width, uts::f32 height) -> URect { return URect(0, 0, width, height); }
        
    public:
        inline auto new_with_pos(uts::f32 x, uts::f32 y) const -> URect { return URect(x, y, width, height); }
        inline auto new_with_size(uts::f32 width, uts::f32 height) const -> URect { return URect(x, y, width, height); }
        inline auto new_with_sizeper(uts::f32 width, uts::f32 height) const -> URect { return URect(0, 0, get_width() * width, get_height() * height); }
        inline auto new_with_widthper(uts::f32 width, uts::f32 height_percent_of_width) const -> URect { return URect(0, 0, get_width() * width, get_width() * width * height_percent_of_width); }
        inline auto new_center(const URect& rect) const -> URect { return URect((get_width() - rect.get_width()) / 2, (get_height() - rect.get_height()) / 2, rect.get_width(), rect.get_height()); }

    public:
        inline auto get_x() const -> uts::f32 { return x; }
        inline auto get_y() const -> uts::f32 { return y; }
        inline auto get_width() const -> uts::f32 { return width; }
        inline auto get_height() const -> uts::f32 { return height; }
};
