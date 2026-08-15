#pragma once

#include <URect.hpp>
#include <UWindowAttribute.hpp>
#include <UTypes.hpp>

class UWindowInfo {
private:
    uts::u8 window_attrs;
    uts::str window_class_name;
    uts::str window_title;
    URect initial_window_rect;

public:
    UWindowInfo() = default;
    inline constexpr UWindowInfo(const uts::str& window_title, const uts::str& window_class_name = "UlixWindowClass"):
        window_attrs(UWindowAttrs::Normal),
        window_class_name(window_class_name),
        window_title(window_title),
        initial_window_rect(0, 0, 800, 600)
    {}

    inline auto rect(const URect& rec) -> UWindowInfo& { initial_window_rect = rec; return *this; }
    inline auto attrs(UWindowAttrs attrs) -> UWindowInfo& { window_attrs |= attrs; return *this; }
    inline auto unattrs(UWindowAttrs attrs) -> UWindowInfo& { window_attrs &= ~attrs; return *this; }

public:
    inline auto get_window_attrs() const -> uts::u8 { return window_attrs; }
    inline auto get_window_class_name() const -> const uts::str& { return window_class_name; }
    inline auto get_window_title() const -> const uts::str& { return window_title; }
    inline auto get_initial_window_rect() const -> URect { return initial_window_rect; }
};
