#pragma once

#include <UFile.hpp>
#include <UTypes.hpp>
#include <UFont.hpp>
#include <require_libraries/stb_truetype/stb_truetype.hpp>

namespace __uii::font {
    struct FontData {
        uts::bytes bytes;
        stbtt_fontinfo info;
        float scale;
    };
    inline static uts::hashmap<uts::str, FontData> font_cache;
    
    inline static auto load(const UFont& font) -> stbtt_fontinfo* {
        uts::str key = font.get_name();

        if (font_cache.find(key) == font_cache.end()) {
            uts::str font_path = key;
            if (!UFile(key).exists()) font_path = "C:\\Windows\\Fonts\\" + key;
            UFile font_file(font_path);
            if (!font_file.exists()) ULogger::ulixerr("failed to find font file: {}", font_path);

            uts::bytes bytes = font_file.read_bytes();
            if (bytes.empty()) ULogger::ulixerr("failed to read font file: {}", font_path);

            FontData data; data.bytes = std::move(bytes);
            if (!stbtt_InitFont(&data.info, data.bytes.data(), 0)) 
                ULogger::ulixerr("failed to initialize font: {}", font_path);

            font_cache[key] = std::move(data);
        }

        auto& fd = font_cache[key];
        fd.scale = stbtt_ScaleForPixelHeight(&fd.info, static_cast<uts::f32>(font.get_size()));
        return &fd.info;
    }

    inline static auto get_scale(const UFont& font) -> float {
        auto it = font_cache.find(font.get_name());
        return (it != font_cache.end()) ? it->second.scale : 1.0f;
    }

    inline static auto get_glyph_bitmap(stbtt_fontinfo* font, uts::u32 codepoint, int& w, int& h, int& xoff, int& yoff, float scale) -> uts::bytes {
        uts::u8* bmp = stbtt_GetCodepointBitmap(font, scale, scale, (int)codepoint, &w, &h, &xoff, &yoff);
        if (!bmp) return {};
        uts::bytes result(bmp, bmp + w * h);
        stbtt_FreeBitmap(bmp, nullptr);
        return result;
    }
}