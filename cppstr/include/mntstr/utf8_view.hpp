// include/mystr/utf8_view.hpp
// TODO
#pragma once

#include "mntstr/type.hpp"

namespace mntstr {

    class utf8_view {
        sv sv_;

        explicit constexpr utf8_view(sv s) noexcept : sv_(s) {}

    public:
        constexpr utf8_view() noexcept = default;

        constexpr operator sv() const noexcept { return sv_; }

        constexpr sv as_sv() const noexcept { return sv_; }

        constexpr const char* data() const noexcept { return sv_.data(); }
        constexpr s_t size() const noexcept { return sv_.size(); }
        constexpr bool empty() const noexcept { return sv_.empty(); }

        friend constexpr bool operator==(utf8_view a, utf8_view b) noexcept {
            return a.sv_ == b.sv_;
        }
    };

}