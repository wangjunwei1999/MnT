#pragma once

#include "mntstr/type.hpp"
#include "mntstr/view.hpp"
#include "mntstr/builder.hpp"
#include "mntstr/base/delim.hpp"

#include <initializer_list>
#include <iterator>
#include <ranges>

namespace mntstr::base {

    enum class join_mode : uc {
        keep_empty,
        skip_empty
    };

    template <class It, class Delim>
    inline s_t joined_size(It first, It last, const Delim& delim, join_mode mode) {
        s_t count = 0;
        s_t sum = 0;
        for (auto it = first; it != last; ++it) {
            const sv part = static_cast<sv>(*it);
            if ((mode == join_mode::skip_empty) && part.empty()) { continue; }
            sum += static_cast<s_t>(part.size());
            ++count;
        }
        if (count <= 1) return sum;
        return sum + static_cast<s_t>(delim.size()) * (count - 1);
    }

    template <class It, class Delim>
    inline void join_append(string_builder<>& out, It first, It last, const Delim& delim, join_mode mode) {
        bool first_elem = true;
        for (auto it = first; it != last; ++it) {
            const sv part = static_cast<sv>(*it);
            if ((mode == join_mode::skip_empty) && part.empty()) { continue; }
            if (!first_elem) out.append(delim.raw());
            out.append(part);
            first_elem = false;
        }
    }

    template <class It>
    inline string join(It first, It last, sv delim, join_mode mode = join_mode::skip_empty) {
        string_builder out;
        out.reserve(joined_size(first, last, delim_sv{delim}, mode));
        join_append(out, first, last, delim_sv{delim}, mode);
        return out.strval();
    }
    template <class It>
    inline string join(It first, It last, char delim, join_mode mode = join_mode::skip_empty) {
        string_builder out;
        out.reserve(joined_size(first, last, delim_char{delim}, mode));
        join_append(out, first, last, delim_char{delim}, mode);
        return out.strval();
    }

    inline string join(std::initializer_list<sv> parts, sv delim, join_mode mode = join_mode::skip_empty) {
        return join(parts.begin(), parts.end(), delim, mode);
    }
    inline string join(std::initializer_list<sv> parts, char delim, join_mode mode = join_mode::skip_empty) {
        return join(parts.begin(), parts.end(), delim, mode);
    }

    template <class Range>
    inline string join(const Range& r, sv delim, join_mode mode = join_mode::skip_empty) {
        return join(std::begin(r), std::end(r), delim, mode);
    }
    template <class Range>
    inline string join(const Range& r, char delim, join_mode mode = join_mode::skip_empty) {
        return join(std::begin(r), std::end(r), delim, mode);
    }

}