#pragma once

#include <UTypes.hpp>
#include <UPixmap.hpp>


class URenderInfo {
    private:
        void* render_callback;
        uts::vec<UPixmap> texture_images;
        uts::u32 max_texture_count = UINT8_MAX;

    public:
        URenderInfo() = default;

        template<typename RenderCallback>
        URenderInfo(RenderCallback render_callback): render_callback(reinterpret_cast<void*>(render_callback)) {}

    public:
        inline auto textures(uts::vec<UPixmap> pixmaps) -> URenderInfo& {
            this->texture_images = pixmaps;
            return *this;
        }

        inline auto max_textures(uts::u32 count) -> URenderInfo& {
            this->max_texture_count = count;
            return *this;
        }

    public:
        inline auto get_render_callback() const -> void* { return render_callback; }
        inline auto get_texture_images() const -> const uts::vec<UPixmap>& { return texture_images; }
        inline auto get_max_texture_count() const -> uts::u32 { return max_texture_count; }
};
