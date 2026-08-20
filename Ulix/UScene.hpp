#pragma once

#include <URect.hpp>
#include <UTypes.hpp>
#include <UColor.hpp>
#include <UObject.hpp>
#include <UAlignment.hpp>

class UScene {
    private:
        uts::vec<UObject> objects;
        UColor background_color;
        uts::u8 alignment = UAlignment::Center;
        uts::u32 vpaddin = 5;
        uts::u32 hpaddin = 5;
        ULayout objlayout = ULayout::None;

    public:
        UScene() = default;

    public:
        inline auto child(const UObject& object) -> UScene& {
            objects.push_back(object);
            for (const auto& child : object.get_objects())
                objects.push_back(child);
            return *this;
        }

        inline auto background(const UColor& color) -> UScene& {
            background_color = color;
            return *this;
        }

        inline auto align(uts::u8 alignment) -> UScene& {
            this->alignment = alignment;
            return *this;
        }

        inline auto vpadding(uts::u32 padding) -> UScene& {
            vpaddin = padding;
            return *this;
        }

        inline auto hpadding(uts::u32 padding) -> UScene& {
            hpaddin = padding;
            return *this;
        }

        inline auto layout(ULayout objlayout) -> UScene& {
            this->objlayout = objlayout;
            return *this;
        }

    public:
        inline auto get_objects() const -> const uts::vec<UObject>& { return objects; }
        inline auto get_background_color() const -> const UColor& { return background_color; }
        inline auto get_alignment() const -> uts::u8 { return alignment; }
        inline auto get_vpadding() const -> uts::u32 { return vpaddin; }
        inline auto get_hpadding() const -> uts::u32 { return hpaddin; }
        inline auto get_layout() const -> ULayout { return objlayout; }
};
