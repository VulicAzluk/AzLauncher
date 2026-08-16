#pragma once

#include <UTypes.hpp>
#include <chrono>

namespace chrono = std::chrono;
template<typename... Args> class UTimer {
    private:
        using TimeoutTask = void(*)(UTimer&, Args&...);

    private:
        uts::i64 last_time;
        uts::u64 last_timeout_delay = 0;
        uts::u64 timeout_delay = 0;
        uts::vec<TimeoutTask> timeout_events;

    public:
        inline constexpr UTimer():
            last_time(chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count())
        {}

        inline auto get_delta() -> uts::i64 {
            uts::i64 current_time = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
            return current_time - last_time;
        }

        inline auto rmv_events() -> void {
            timeout_events = {};
        }

        inline auto pop_event() -> void {
            timeout_events.pop_back();
        }

        inline auto delay(uts::u64 delay) -> UTimer& {
            timeout_delay = delay;
            return *this;
        }

        inline auto event(TimeoutTask event) -> UTimer& {
            timeout_events.push_back(event);
            return *this;
        }

    public:
        inline void update(Args&... args) {
            if (get_delta() > static_cast<uts::i32>(last_timeout_delay) && !timeout_events.empty()) {
                last_timeout_delay += timeout_delay;
                for (const auto& event : timeout_events)
                    event(*this, args...);
            }
        }
};
