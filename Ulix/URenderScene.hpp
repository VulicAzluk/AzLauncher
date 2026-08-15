#pragma once

#include <UTypes.hpp>
#include <UColor.hpp>
#include <UObject.hpp>

class URenderScene {
    private:
        uts::vec<UObject> objects;
        UColor background_color;

    public:
        URenderScene() = default;

    public:
        inline auto child(const UObject& object) -> URenderScene& {
            objects.push_back(object);
            return *this;
        }

        inline auto background(const UColor& color) -> URenderScene& {
            background_color = color;
            return *this;
        }

    public:
        inline auto get_objects() const -> const uts::vec<UObject>& { return objects; }
        inline auto get_background_color() const -> const UColor& { return background_color; }
};
