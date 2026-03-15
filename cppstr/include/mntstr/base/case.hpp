#pragma once

#include "mntstr/type.hpp"
#include "mntstr/ascii_base.hpp"
#include "mntstr/builder.hpp"
#include "mntstr/view.hpp"
#include "mntstr/base/find.hpp"

#include <algorithm>

namespace mntstr::base {

    constexpr uc to_lower_uc(uc c) noexcept { return ascii_to_lower(c); }
    constexpr uc to_upper_uc(uc c) noexcept { return ascii_to_upper(c); }

    inline bool iequals_ascii(sv a, sv b) noexcept {
        if (a.size() != b.size()) return false;
        const s_t n = a.size();
        for (s_t i = 0; i < n; ++i) {
            const uc ca = to_lower_uc(static_cast<uc>(a[i]));
            const uc cb = to_lower_uc(static_cast<uc>(b[i]));
            if (ca != cb) return false;
        }
        return true;
    }

    inline bool iless_ascii(sv a, sv b) noexcept {
        const s_t n = std::min(a.size(), b.size());
        for (s_t i = 0; i < n; ++i) {
            const uc ca = to_lower_uc(static_cast<uc>(a[i]));
            const uc cb = to_lower_uc(static_cast<uc>(b[i]));
            if (ca < cb) return true;
            if (ca > cb) return false;
        }
        return a.size() < b.size();
    }

    inline bool istarts_with_ascii(sv s, sv prefix) noexcept {
        if (prefix.size() > s.size()) return false;
        return iequals_ascii(substr_safe(s, 0, prefix.size()), prefix);
    }

    inline bool iends_with_ascii(sv s, sv suffix) noexcept {
        if (suffix.size() > s.size()) return false;
        return iequals_ascii(substr_safe(s, s.size() - suffix.size()), suffix);
    }

    inline bool icontains_ascii(sv s, sv needle) noexcept {
        if (needle.empty()) return true;
        if (needle.size() > s.size()) return false;

        const s_t n = s.size();
        const s_t m = needle.size();

        for (s_t i = 0; i + m <= n; ++i) {
            if (iequals_ascii(substr_safe(s, i, m), needle)) return true;
        }
        return false;
    }

    inline sv to_lower_ascii_into(string_builder<>& out, sv s) {
        out.clear();
        out.reserve(s.size());
        for (char ch : s) {
            out.append(static_cast<char>(to_lower_uc(static_cast<uc>(ch))));
        }
        return out.view();
    }

    inline sv to_upper_ascii_into(string_builder<>& out, sv s) {
        out.clear();
        out.reserve(s.size());
        for (char ch : s) {
            out.append(static_cast<char>(to_upper_uc(static_cast<uc>(ch))));
        }
        return out.view();
    }

    inline string to_lower_ascii(sv s) {
        string_builder<> out;
        to_lower_ascii_into(out, s);
        return out.strval();
    }

    inline string to_upper_ascii(sv s) {
        string_builder<> out;
        to_upper_ascii_into(out, s);
        return out.strval();
    }

    // with new strings
    inline bool icontains_ascii_cp(sv s, sv needle) noexcept {
        if (needle.empty()) return true;
        if (needle.size() > s.size()) return false;

        string_builder<> out_s;
        sv out_sv = to_lower_ascii_into(out_s, s);
        string_builder<> out_n;
        sv out_nv = to_lower_ascii_into(out_n, needle);

        return contains(out_sv, out_nv);
    }

}