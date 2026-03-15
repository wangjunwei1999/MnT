#pragma once

#include "mntstr/type.hpp"

#include <cstring>
#include <algorithm>

namespace mntstr {
    
    constexpr bool ascii_is_space(uc c) noexcept {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
    }
    constexpr bool ascii_is_space(char c) noexcept {
        return ascii_is_space(static_cast<uc>(c));
    }

    constexpr bool ascii_is_digit(uc c) noexcept { return c >= '0' && c <= '9'; }
    constexpr bool ascii_is_digit(char c) noexcept {
        return ascii_is_digit(static_cast<uc>(c));
    }

    constexpr uc ascii_to_lower(uc c) noexcept {
        return (c >= 'A' && c <= 'Z') ? static_cast<uc>(c + ('a' - 'A')) : c;
    }
    constexpr char ascii_to_lower(char c) noexcept {
        return static_cast<char>(ascii_to_lower(static_cast<uc>(c)));
    }

    constexpr uc ascii_to_upper(uc c) noexcept {
        return (c >= 'a' && c <= 'z') ? static_cast<uc>(c + ('A' - 'a')) : c;
    }
    constexpr char ascii_to_upper(char c) noexcept {
        return static_cast<char>(ascii_to_upper(static_cast<uc>(c)));
    }

}
