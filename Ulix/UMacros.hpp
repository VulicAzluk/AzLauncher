#pragma once

#include <ULogger.hpp>

#define TODO(x) ([]() -> decltype(auto) { ULogger::ulixwarn("unfinished todo: {}", (x)); return (x); })()
#define BUG(x) ([]() -> decltype(auto) { ULogger::ulixerr("unresolved bug: {}", (x)); })()
#define DEBUG(x) ([]() -> decltype(auto) { ULogger::ulixwarn("debug index: {}", (x)); })()
