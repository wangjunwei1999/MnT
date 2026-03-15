#pragma once

#include "mntstr/type.hpp"
#include "mntstr/ascii_base.hpp"
#include "mntstr/view.hpp"

#include <optional>

namespace mntstr::base {

    // trim
    constexpr sv ltrim_ascii(sv s) noexcept {
        s_t i = 0;
        while (i < s.size() && mntstr::ascii_is_space(static_cast<uc>(s[i]))) {
            ++i;
        }
        return substr_safe(s, i);
    }

    constexpr sv rtrim_ascii(sv s) noexcept {
        s_t i = s.size();
        while (i && mntstr::ascii_is_space(static_cast<uc>(s[i-1]))) {
            --i;
        }
        return substr_safe(s, 0, i);
    }

    constexpr sv trim_ascii(sv s) noexcept {
        return rtrim_ascii(ltrim_ascii(s));
    }

    // strip
    constexpr std::optional<sv> strip_prefix(sv s, sv prefix) noexcept {
        if (mntstr::starts_with(s, prefix)) return substr_safe(s, prefix.size());
        return std::nullopt;
    }

    constexpr std::optional<sv> strip_suffix(sv s, sv suffix) noexcept {
        if (mntstr::ends_with(s, suffix)) return substr_safe(s, 0, s.size() - suffix.size());
        return std::nullopt;
    }

}