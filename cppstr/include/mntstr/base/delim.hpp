#pragma once

#include "mntstr/type.hpp"

namespace mntstr::base {

    struct delim_char {
        char c{};
        constexpr s_t size() const noexcept { return 1; }
        constexpr char raw() const noexcept { return c; }
    };

    struct delim_sv {
        sv d{};
        constexpr s_t size() const noexcept { return static_cast<s_t>(d.size()); }
        constexpr sv raw() const noexcept { return d; }
    };

}