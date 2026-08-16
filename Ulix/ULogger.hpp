#pragma once

#include <format>
#include <print>

namespace ULogger {
    template<typename... Args> [[noreturn]] inline void ulixerr(const char* format, Args&&... args) {
        std::println("ulix: \x1b[91merror:\x1b[0m {}", std::vformat(format, std::make_format_args(args...)));
        std::abort();
    }

    template<typename... Args>
    inline void ulixwarn(const char* format, Args&&... args) {
        std::println("ulix: \x1b[93mwarning:\x1b[0m {}", std::vformat(format, std::make_format_args(args...)));
    }
}
