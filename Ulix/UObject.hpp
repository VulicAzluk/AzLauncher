#pragma once

#include "ULayout.hpp"
#include <UPixmap.hpp>
#include <UAnchor.hpp>
#include <URect.hpp>
#include <UColor.hpp>
#include <UAlignment.hpp>


class UObject {
    private:
        UColor top_left_color = UColor(255, 255, 255, 255);
        UColor top_right_color = UColor(255, 255, 255, 255);
        UColor bottom_left_color = UColor(255, 255, 255, 255);
        UColor bottom_right_color = UColor(255, 255, 255, 255);
        UColor top_left_border_color = UColor(255, 255, 255, 255);
        UColor top_right_border_color = UColor(255, 255, 255, 255);
        UColor bottom_left_border_color = UColor(255, 255, 255, 255);
        UColor bottom_right_border_color = UColor(255, 255, 255, 255);
        uts::f32 top_left_corner_radius = 16.0f;
        uts::f32 top_right_corner_radius = 16.0f;
        uts::f32 bottom_left_corner_radius = 16.0f;
        uts::f32 bottom_right_corner_radius = 16.0f;
        uts::f32 top_left_border_width = 1.0f;
        uts::f32 top_right_border_width = 1.0f;
        uts::f32 bottom_left_border_width = 1.0f;
        uts::f32 bottom_right_border_width = 1.0f;
        URect rec = URect(100, 100, 200, 200);
        uts::f32 z_index = 0.0f;
        std::vector<UObject> objects;
        uts::u32 texture_index;
        uts::u8 alignment = UAlignment::Center;
        uts::u32 vpaddin = 5;
        uts::u32 hpaddin = 5;
        ULayout objlayout = ULayout::None;

    public:
        UObject() = default;

    public:
        inline auto fill(const UAnchor anchor, const UColor& color) -> UObject& {
            switch (anchor) {
                case UAnchor::TopLeft: top_left_color = color; break;
                case UAnchor::TopRight: top_right_color = color; break;
                case UAnchor::BottomLeft: bottom_left_color = color; break;
                case UAnchor::BottomRight: bottom_right_color = color; break;
                case UAnchor::AllAnchor:
                    top_left_color = color;
                    top_right_color = color;
                    bottom_left_color = color;
                    bottom_right_color = color;
                    break;
            }

            return *this;
        }

        inline auto border(const UAnchor anchor, const UColor& color) -> UObject& {
            switch (anchor) {
                case UAnchor::TopLeft: top_left_border_color = color; break;
                case UAnchor::TopRight: top_right_border_color = color; break;
                case UAnchor::BottomLeft: bottom_left_border_color = color; break;
                case UAnchor::BottomRight: bottom_right_border_color = color; break;
                case UAnchor::AllAnchor:
                    top_left_border_color = color;
                    top_right_border_color = color;
                    bottom_left_border_color = color;
                    bottom_right_border_color = color;
                    break;
            }

            return *this;
        }

        inline auto corner(const UAnchor anchor, uts::f32 radius) -> UObject& {
            switch (anchor) {
                case UAnchor::TopLeft: top_left_corner_radius = radius; break;
                case UAnchor::TopRight: top_right_corner_radius = radius; break;
                case UAnchor::BottomLeft: bottom_left_corner_radius = radius; break;
                case UAnchor::BottomRight: bottom_right_corner_radius = radius; break;
                case UAnchor::AllAnchor:
                    top_left_corner_radius = radius;
                    top_right_corner_radius = radius;
                    bottom_left_corner_radius = radius;
                    bottom_right_corner_radius = radius;
                    break;
            }

            return *this;
        }

        inline auto border(const UAnchor anchor, uts::f32 width) -> UObject& {
            switch (anchor) {
                case UAnchor::TopLeft: top_left_border_width = width; break;
                case UAnchor::TopRight: top_right_border_width = width; break;
                case UAnchor::BottomLeft: bottom_left_border_width = width; break;
                case UAnchor::BottomRight: bottom_right_border_width = width; break;
                case UAnchor::AllAnchor:
                    top_left_border_width = width;
                    top_right_border_width = width;
                    bottom_left_border_width = width;
                    bottom_right_border_width = width;
                    break;
            }

            return *this;
        }

        inline auto rect(const URect& rec) -> UObject& {
            this->rec = rec;
            return *this;
        }

        inline auto zindex(uts::f32 index) -> UObject& {
            z_index = index;
            return *this;
        }

        inline auto child(UObject& object) -> UObject& {
            object.rect(object.get_rect().new_with_pos(
                object.get_rect().get_x() + rec.get_x(),
                object.get_rect().get_y() + rec.get_y()));
            objects.push_back(object);
            return *this;
        }

        inline auto texture(const uts::u32 index) -> UObject& {
            texture_index = index;
            return *this;
        }

        inline auto align(uts::u8 alignment) -> UObject& {
            this->alignment = alignment;
            return *this;
        }

        inline auto vpadding(uts::u32 padding) -> UObject& {
            vpaddin = padding;
            return *this;
        }

        inline auto hpadding(uts::u32 padding) -> UObject& {
            hpaddin = padding;
            return *this;
        }

        inline auto layout(ULayout objlayout) -> UObject& {
            this->objlayout = objlayout;
            return *this;
        }
        
    public:
        inline auto get_top_left_color() const -> const UColor& { return top_left_color; }
        inline auto get_top_right_color() const -> const UColor& { return top_right_color; }
        inline auto get_bottom_left_color() const -> const UColor& { return bottom_left_color; }
        inline auto get_bottom_right_color() const -> const UColor& { return bottom_right_color; }

        inline auto get_top_left_border_color() const -> const UColor& { return top_left_border_color; }
        inline auto get_top_right_border_color() const -> const UColor& { return top_right_border_color; }
        inline auto get_bottom_left_border_color() const -> const UColor& { return bottom_left_border_color; }
        inline auto get_bottom_right_border_color() const -> const UColor& { return bottom_right_border_color; }

        inline auto get_top_left_corner_radius() const -> uts::f32 { return top_left_corner_radius; }
        inline auto get_top_right_corner_radius() const -> uts::f32 { return top_right_corner_radius; }
        inline auto get_bottom_left_corner_radius() const -> uts::f32 { return bottom_left_corner_radius; }
        inline auto get_bottom_right_corner_radius() const -> uts::f32 { return bottom_right_corner_radius; }

        inline auto get_top_left_border_width() const -> uts::f32 { return top_left_border_width; }
        inline auto get_top_right_border_width() const -> uts::f32 { return top_right_border_width; }
        inline auto get_bottom_left_border_width() const -> uts::f32 { return bottom_left_border_width; }
        inline auto get_bottom_right_border_width() const -> uts::f32 { return bottom_right_border_width; }

        inline auto get_rect() const -> const URect& { return rec; }
        inline auto get_z_index() const -> uts::f32 { return z_index; }
        inline auto get_objects() const -> std::vector<UObject> { return objects; }
        inline auto get_texture_index() const -> uts::u32 { return texture_index; }
        inline auto get_alignment() const -> uts::u8 { return alignment; }
        inline auto get_vpadding() const -> uts::u32 { return vpaddin; }
        inline auto get_hpadding() const -> uts::u32 { return hpaddin; }
        inline auto get_layout() const -> ULayout { return objlayout; }
};
