#pragma once

#include <URect.hpp>
#include <UWindowAttribute.hpp>
#include <UTypes.hpp>

class UWindowInfo {
private:
    uts::u8 window_attributes;
    uts::str window_class_name;
    uts::str window_title;
    URect initial_window_rect;

public:
    UWindowInfo() = default;
    inline constexpr UWindowInfo(const uts::str& window_title, const uts::str& window_class_name = "UlixWindowClass"):
        window_attributes(UWindowAttribute::Normal),
        window_class_name(window_class_name),
        window_title(window_title),
        initial_window_rect(0, 0, 800, 600)
    {}

    inline auto with_initial_rect(const URect& rect) -> UWindowInfo& {
        initial_window_rect = rect;

        return *this;
    }

    inline auto with_attribute(UWindowAttribute attribute) -> UWindowInfo& {
        window_attributes |= attribute;

        return *this;
    }

    inline auto without_attribute(UWindowAttribute attribute) -> UWindowInfo& {
        window_attributes &= ~attribute;

        return *this;
    }

public:
    inline auto get_window_attributes() const -> uts::u8 { return window_attributes; }
    inline auto get_window_class_name() const -> const uts::str& { return window_class_name; }
    inline auto get_window_title() const -> const uts::str& { return window_title; }
    inline auto get_initial_window_rect() const -> URect { return initial_window_rect; }
};
