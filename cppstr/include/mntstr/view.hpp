#pragma once

#include "mntstr/type.hpp"

#include <cstring>
#include <algorithm>

namespace mntstr {

    // Define as_sv

    constexpr sv as_sv(sv s) noexcept { return s; }

    inline sv as_sv(const std::string& s) noexcept {
        return sv{s.data(), s.size()};
    }

    template <s_t N>
    constexpr sv as_sv(const char (&lit)[N]) noexcept {
        static_assert(N > 0);
        return (lit[N - 1] == '\0') ? sv{lit, N - 1} : sv{lit, N};
    }

    constexpr sv as_sv(const char* p, s_t n) noexcept { return sv{p, n}; }

    // substr && consume prefix/suffix

    constexpr sv substr_safe(sv s, s_t pos, s_t n = sv::npos) noexcept {
        if (pos >= s.size()) return sv{};
        const s_t max_n = s.size() - pos;
        if (n > max_n) n = max_n;
        return s.substr(pos, n);
    }

    // constexpr bool consume_prefix(sv& s, sv prefix) noexcept {
    //     if (s.size() < prefix.size()) return false;
    //     if (s.compare(0, prefix.size(), prefix) != 0) return false;
    //     s.remove_prefix(prefix.size());
    //     return true;
    // }

    // constexpr bool consume_suffix(sv& s, sv suffix) noexcept {
    //     if (s.size() < suffix.size()) return false;
    //     if (s.compare(s.size() - suffix.size(), suffix.size(), suffix) != 0) return false;
    //     s.remove_suffix(suffix.size());
    //     return true;
    // }

    constexpr bool starts_with(sv s, sv prefix) noexcept {
        return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
    }

    constexpr bool ends_with(sv s, sv suffix) noexcept {
        return s.size() >= suffix.size() && 
            s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

}