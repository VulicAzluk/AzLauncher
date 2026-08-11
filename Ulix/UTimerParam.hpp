#pragma once

#include <functional>
#include <cstddef>

class UTimerParam {
    private:
        std::reference_wrapper<const std::byte> data;
    
    public:
        template<typename U>
        UTimerParam(const U& data): data(reinterpret_cast<const std::byte&>(data)) {}
    
        template<typename T> const T& get() const {
            return *reinterpret_cast<const T*>(&data.get());
        }
};