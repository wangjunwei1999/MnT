#pragma once

#include "mntstr/type.hpp"
#include "mntstr/ascii_base.hpp"

#include <algorithm>

namespace mntstr::base {

    inline s_t find(sv s, char needle, s_t pos = 0) noexcept {
        return static_cast<s_t>(s.find(needle, pos));
    }
    inline s_t find(sv s, sv needle, s_t pos = 0) noexcept {
        if (needle.size() == 1) return find(s, needle[0], pos);
        return static_cast<s_t>(s.find(needle, pos));
    }

    inline s_t rfind(sv s, char needle, s_t pos = 0) noexcept {
        return static_cast<s_t>(s.rfind(needle, pos));
    }
    inline s_t rfind(sv s, sv needle, s_t pos = 0) noexcept {
        if (needle.size() == 1) return rfind(s, needle[0], pos);
        return static_cast<s_t>(s.rfind(needle, pos));
    }

    inline bool contains(sv s, char needle) noexcept {
        return find(s, needle) != static_cast<s_t>(sv::npos);
    }
    inline bool contains(sv s, sv needle) noexcept {
        return find(s, needle) != static_cast<s_t>(sv::npos);
    }

    inline s_t find_nth(sv s, char needle, s_t nth, s_t pos = 0) noexcept {
        const s_t n = static_cast<s_t>(s.size());
        if (pos > n) return static_cast<s_t>(sv::npos);

        s_t cur = pos;
        for (s_t k = 0; k <= nth; ++k) {
            const s_t p = find(s, needle, cur);
            if (p == static_cast<s_t>(sv::npos)) return static_cast<s_t>(sv::npos);
            if (k == nth) return p;
            cur = p + 1;

            if (cur > n) return static_cast<s_t>(sv::npos);
        }

        return static_cast<s_t>(sv::npos);
    }

    inline s_t find_nth(sv s, sv needle, s_t nth, s_t pos = 0,
                            bool allow_overlap = true) noexcept {
        const s_t n = static_cast<s_t>(s.size());
        if (pos > n) return static_cast<s_t>(sv::npos);

        const s_t m = static_cast<s_t>(needle.size());
        if (m == 0) {
            const s_t target = pos + nth;
            return (target <= n) ? target : static_cast<s_t>(sv::npos);
        }

        s_t cur = pos;
        for (s_t k = 0; k <= nth; ++k) {
            const s_t p = find(s, needle, cur);
            if (p == static_cast<s_t>(sv::npos)) return static_cast<s_t>(sv::npos);
            if (k == nth) return p;
            cur = allow_overlap ? (p + 1) : (p + m);

            if (cur > n) return static_cast<s_t>(sv::npos);
        }

        return static_cast<s_t>(sv::npos);
    }

}