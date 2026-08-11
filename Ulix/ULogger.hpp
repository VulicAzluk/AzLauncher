#pragma once

#include <format>
#include <print>

namespace ULogger {
    template<typename... Args> [[noreturn]] inline void ulixerr(const char* format, Args&&... args) {
        std::println("\x1b[31m[Ulix] Error: {}\x1b[0m", std::vformat(format, std::make_format_args(args...)));
        std::abort();
    }

    template<typename... Args>
    inline void ulixwarn(const char* format, Args&&... args) {
        std::println("\x1b[33m[Ulix] Warning: {}\x1b[0m", std::vformat(format, std::make_format_args(args...)));
    }
}
