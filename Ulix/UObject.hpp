#pragma once

#include <UPixmap.hpp>
#include <UAnchor.hpp>
#include <URect.hpp>
#include <UColor.hpp>


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
        UPixmap texture_image_pixmap;

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

        auto rect(const URect& rec) -> UObject& {
            this->rec = rec;
            return *this;
        }

        auto zindex(uts::f32 index) -> UObject& {
            z_index = index;
            return *this;
        }

        auto child(UObject& object) -> UObject& {
            object.rect(object.get_rect().create_with_position(
                object.get_rect().get_x() + rec.get_x(),
                object.get_rect().get_y() + rec.get_y()));
            objects.push_back(object);
            return *this;
        }

        auto texture(const UPixmap& image_pixmap) -> UObject& {
            texture_image_pixmap = image_pixmap;
            return *this;
        }

    public:
        auto get_top_left_color() const -> const UColor& { return top_left_color; }
        auto get_top_right_color() const -> const UColor& { return top_right_color; }
        auto get_bottom_left_color() const -> const UColor& { return bottom_left_color; }
        auto get_bottom_right_color() const -> const UColor& { return bottom_right_color; }

        auto get_top_left_border_color() const -> const UColor& { return top_left_border_color; }
        auto get_top_right_border_color() const -> const UColor& { return top_right_border_color; }
        auto get_bottom_left_border_color() const -> const UColor& { return bottom_left_border_color; }
        auto get_bottom_right_border_color() const -> const UColor& { return bottom_right_border_color; }

        auto get_top_left_corner_radius() const -> uts::f32 { return top_left_corner_radius; }
        auto get_top_right_corner_radius() const -> uts::f32 { return top_right_corner_radius; }
        auto get_bottom_left_corner_radius() const -> uts::f32 { return bottom_left_corner_radius; }
        auto get_bottom_right_corner_radius() const -> uts::f32 { return bottom_right_corner_radius; }

        auto get_top_left_border_width() const -> uts::f32 { return top_left_border_width; }
        auto get_top_right_border_width() const -> uts::f32 { return top_right_border_width; }
        auto get_bottom_left_border_width() const -> uts::f32 { return bottom_left_border_width; }
        auto get_bottom_right_border_width() const -> uts::f32 { return bottom_right_border_width; }

        auto get_rect() const -> const URect& { return rec; }
        auto get_z_index() const -> uts::f32 { return z_index; }
        auto get_objects() const -> std::vector<UObject> { return objects; }
        auto get_texture_image() const -> const UPixmap& { return texture_image_pixmap; }
};
