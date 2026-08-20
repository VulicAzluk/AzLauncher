#pragma once

#include <UColor.hpp>
#include <UTypes.hpp>

class UFont {
    private:
        uts::str font_name;
        uts::u32 font_size;
        bool font_bold;
        bool font_italic;
        bool font_underline;
        bool font_strikethrough;
        UColor font_color;
        
    public:
        inline UFont(const uts::str& name = "arial.ttf", uts::u32 size = 12, UColor color = UColor(255, 255, 255), bool bold = false, bool italic = false, bool underline = false, bool strikethrough = false) : font_name(name), font_size(size), font_bold(bold), font_italic(italic), font_underline(underline), font_strikethrough(strikethrough), font_color(color) {}
        bool operator==(const UFont& other) const {
            return font_name == other.font_name && font_size == other.font_size && font_bold == other.font_bold && font_italic == other.font_italic && font_underline == other.font_underline && font_strikethrough == other.font_strikethrough && font_color.get_red() == other.font_color.get_red() && font_color.get_green() == other.font_color.get_green() && font_color.get_blue() == other.font_color.get_blue() && font_color.get_alpha() == other.font_color.get_alpha();
        };
        
        inline auto font(const uts::str& name) -> UFont& {
            font_name = name;
            return *this;
        }

        inline auto size(uts::u32 size) -> UFont& {
            font_size = size;
            return *this;
        }

        inline auto color(const UColor& color) -> UFont& {
            font_color = color;
            return *this;
        }

        inline auto bold(bool bold) -> UFont& {
            this->font_bold = bold;
            return *this;
        }

        inline auto italic(bool italic) -> UFont& {
            this->font_italic = italic;
            return *this;
        }

        inline auto underline(bool underline) -> UFont& {
            this->font_underline = underline;
            return *this;
        }

        inline auto strikethrough(bool strikethrough) -> UFont& {
            this->font_strikethrough = strikethrough;
            return *this;
        }

    public:
        inline auto get_name() const -> uts::str { return font_name; }
        inline auto get_size() const -> uts::u32 { return font_size; }
        inline auto is_bold() const -> bool { return font_bold; }
        inline auto is_italic() const -> bool { return font_italic; }
        inline auto is_underline() const -> bool { return font_underline; }
        inline auto is_strikethrough() const -> bool { return font_strikethrough; }
        inline auto get_color() const -> const UColor& { return font_color; }
};
