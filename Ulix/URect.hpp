#pragma once

#include <UTypes.hpp>
#include <UMacros.hpp>

class URect {
    private:
        uts::f32 x, y;
        uts::f32 width, height;

    public:
        URect() = default;
        inline constexpr URect(uts::f32 x, uts::f32 y, uts::f32 width, uts::f32 height):
            x(x), y(y), width(width), height(height) {}

    public:
        forceinline auto create_with_position(uts::f32 x, uts::f32 y) const -> URect { return URect(x, y, width, height); }
        forceinline auto create_with_size(uts::f32 width, uts::f32 height) const -> URect { return URect(x, y, width, height); }
        forceinline auto create_with_percent(uts::f32 width, uts::f32 height) const -> URect {
            return URect(0, 0, get_width() * width, get_height() * height);
        }
        forceinline auto create_with_percent_of_width(uts::f32 width, uts::f32 height_percent_of_width) const -> URect {
            return URect(0, 0, get_width() * width, get_width() * width * height_percent_of_width);
        }
        forceinline auto create_center_rect(const URect& rect) const -> URect {
            return URect((get_width() - rect.get_width()) / 2,
                         (get_height() - rect.get_height()) / 2,
                         rect.get_width(), rect.get_height());
        }

    public:
        forceinline auto get_x() const -> uts::f32 { return x; }
        forceinline auto get_y() const -> uts::f32 { return y; }
        forceinline auto get_width() const -> uts::f32 { return width; }
        forceinline auto get_height() const -> uts::f32 { return height; }
};
