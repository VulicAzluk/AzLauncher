#pragma once

#include <UFont.hpp>
#include <__inside_impl/__FontLoader.hpp>
#include <UFile.hpp>
#include <ULogger.hpp>
#include <URect.hpp>
#include <UTypes.hpp>
#include <cassert>
#include <intsafe.h>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <wincodec.h>
#include <winerror.h>
#include <wrl/client.h>
#include <require_libraries/nanosvg/nanosvg.hpp>
#include <require_libraries/stb_image/stb_image.hpp>

template<typename T> using Pointer = Microsoft::WRL::ComPtr<T>;


class UPixmap {
    private:
        uts::u32 texture_width;
        uts::u32 texture_height;
        uts::u64 texture_size;
        uts::vec<uts::u8> texture_pixels;

    public:
        UPixmap() = default;
        UPixmap(uts::vec<uts::u8> pixels, uts::u32 width, uts::u32 height) : texture_pixels(pixels), texture_width(width), texture_height(height), texture_size(pixels.size()) {}

        inline static auto init() -> void {
            HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            if (FAILED(result)) ULogger::ulixerr("failed to initialize WIC");
        }

        inline static auto from_bitmap(const UFile& image_file) -> UPixmap {
            uts::str file_path = image_file.get_file_path();
            uts::i32 texture_width = 0, texture_height = 0, texture_channels = 0;

            uts::u8* data = stbi_load(file_path.c_str(), &texture_width, &texture_height, &texture_channels, STBI_rgb_alpha);
            if (!data) ULogger::ulixerr("failed to load image (stb_image): {}", file_path, stbi_failure_reason());
            uts::vec<uts::u8> texture_pixels(data, data + texture_width * texture_height * 4);
            stbi_image_free(data);

            return UPixmap(texture_pixels, texture_width, texture_height);
        }

        inline static auto from_vector(const UFile& image_file, uts::u32 width, uts::u32 height) -> UPixmap {
            uts::str file_path = image_file.get_file_path();
            NSVGimage* svg = nsvgParseFromFile(file_path.c_str(), "px", 96.0f);
            if (!svg) ULogger::ulixerr("failed to parse svg (nanosvg): {}", file_path);
            NSVGrasterizer* rast = nsvgCreateRasterizer();
            
            uts::f32 scale = std::min(
                static_cast<uts::f32>(width) / svg->width,
                static_cast<uts::f32>(height) / svg->height
            );
            uts::u32 stride = ((width * 4 + 15) & ~15);
            uts::vec<uts::u8> texture_pixels(stride * height, 0);
            
            nsvgRasterize(rast, svg, 0, 0, scale, texture_pixels.data(), width, height, stride);
            nsvgDeleteRasterizer(rast);
            nsvgDelete(svg);
            
            return UPixmap(texture_pixels, width, height);
        }
        
        inline static auto from_text(const uts::str& text, const UFont font = UFont()) -> UPixmap {
            stbtt_fontinfo* font_info = __uii::font::load(font);
            if (!font_info) ULogger::ulixerr("failed to load font info (stb_truetype)");
        
            uts::f32 scale = __uii::font::get_scale(font);
            uts::i32 ascent = 0, descent = 0, line_gap = 0;
            stbtt_GetFontVMetrics(font_info, &ascent, &descent, &line_gap);
        
            uts::i32 x = 0;
            uts::i32 min_x = INT_MAX, max_x = INT_MIN;
            uts::i32 min_y = INT_MAX, max_y = INT_MIN;
            for (size_t index = 0; index < text.size(); ++index) {
                const uts::u32 codepoint = static_cast<uts::u32>(text[index]);
        
                uts::i32 width = 0, height = 0, xoff = 0, yoff = 0;
                __uii::font::get_glyph_bitmap(font_info, codepoint, width, height, xoff, yoff, scale);
        
                uts::i32 x0 = x + xoff; uts::i32 x1 = x0 + width;
                uts::i32 y0 = yoff; uts::i32 y1 = y0 + height;
                min_x = std::min(min_x, x0); max_x = std::max(max_x, x1);
                min_y = std::min(min_y, y0); max_y = std::max(max_y, y1);
        
                uts::i32 advance = 0;
                stbtt_GetCodepointHMetrics(font_info, codepoint, &advance, nullptr);
                x += static_cast<int>(advance * scale);
        
                if (index + 1 >= text.size()) continue;
                uts::u32 next = static_cast<uts::u32>(text[index + 1]);
                int kern = stbtt_GetCodepointKernAdvance(font_info, codepoint, next);
                x += static_cast<int>(kern * scale);
            }
        
            if (min_x > max_x || min_y > max_y) ULogger::ulixerr("failed to rasterize text (stb_truetype): empty text bounds");
            uts::u32 width = max_x - min_x, height = max_y - min_y;
            uts::bytes pixels(static_cast<uts::size>(width * height * 4), 0);
        
            x = -min_x;
            uts::i32 y = -min_y;
            for (uts::size index = 0; index < text.size(); ++index) {
                uts::u32 codepoint = static_cast<uts::u32>(text[index]);
                int w = 0, h = 0, xoff = 0, yoff = 0;
                uts::bytes glyph = __uii::font::get_glyph_bitmap(font_info, codepoint, w, h, xoff, yoff, scale);
                
                for (int gy = 0; gy < h; ++gy) {
                    for (int gx = 0; gx < w; ++gx) {
                        const int px = x + xoff + gx;
                        const int py = y + yoff + gy;
        
                        if (static_cast<unsigned>(px) >= static_cast<unsigned>(width) || static_cast<unsigned>(py) >= static_cast<unsigned>(height))
                            continue;
        
                        uts::u8 alpha = glyph[static_cast<uts::size>(gy * w + gx)];
                        uts::u8* dst = &pixels[static_cast<uts::size>((py * width + px) * 4)];
                        UColor color = font.get_color();
                        
                        dst[0] = color.get_red();
                        dst[1] = color.get_green();
                        dst[2] = color.get_blue();
                        dst[3] = alpha * color.get_alpha() / 255.0f;
                    }
                }
        
                int advance = 0;
                stbtt_GetCodepointHMetrics(font_info, codepoint, &advance, nullptr);
                x += static_cast<int>(advance * scale);
        
                if (index + 1 >= text.size()) continue;
                uts::u32 next = static_cast<uts::u32>(text[index + 1]);
                int kern = stbtt_GetCodepointKernAdvance(font_info, codepoint, next);
                x += static_cast<int>(kern * scale);
            }
        
            return UPixmap(std::move(pixels), static_cast<uts::u32>(width), static_cast<uts::u32>(height));
        }

    public:
        auto get_rect() const -> URect { return URect(0, 0, texture_width, texture_height); }
        auto get_pixel_size() const -> uts::u64 { return texture_size; }
        auto get_pixels() const -> const uts::vec<uts::u8> { return texture_pixels; }
};
