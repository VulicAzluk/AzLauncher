#pragma once

#include <UTypes.hpp>

enum UWindowAttribute: uts::u8 {
    Resizable = 1 << 0,
    Titled = 1 << 1,
    Bordered = 1 << 2,

    Normal = Resizable | Titled | Bordered
};
