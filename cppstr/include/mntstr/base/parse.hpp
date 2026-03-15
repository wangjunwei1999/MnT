#pragma once

#include "mntstr/type.hpp"
#include "mntstr/ascii_base.hpp"
#include "mntstr/base/trim.hpp"

#include <charconv>
#include <type_traits>
#include <limits>
#include <string>
#include <cerrno>
#include <cstdlib>

namespace mntstr::base {

    enum class parse_ws : uc { 
        none, 
        trim_ascii 
    };
    enum class parse_errc : uc {
        ok,
        empty,
        invalid,
        overflow,
        trailing,
        bad_base
    };

    struct parse_error {
        parse_errc code{parse_errc::ok};
        s_t pos{0};
    };

    template <class T>
    struct parse_result {
        T value{};
        parse_error err{};
        constexpr explicit operator bool() const noexcept { return err.code == parse_errc::ok; }
    };

    template <class Int>
    concept has_int_from_chars =
        std::is_integral_v<Int> &&
        requires(const char* a, const char* b, Int& v) {
        { std::from_chars(a, b, v, 10) } -> std::same_as<std::from_chars_result>;
    };

    // to integers
    template <has_int_from_chars Int>
    inline parse_result<Int> to_int(sv s, int base = 10,
                                parse_ws ws = parse_ws::none) noexcept {
        parse_result<Int> r{};

        if (ws == parse_ws::trim_ascii) s = trim_ascii(s);
        if (s.empty()) { 
            r.err = {parse_errc::empty, 0}; 
            return r; 
        }
        if (base != 0 && (base < 2 || base > 36)) { 
            r.err = {parse_errc::bad_base, 0}; 
            return r; 
        }

        const char* first = s.data();
        const char* last  = s.data() + s.size();

        Int v{};
        auto fc = std::from_chars(first, last, v, base);
        const char* ptr = fc.ptr;

        if (fc.ec == std::errc::invalid_argument) {
            r.err = {parse_errc::invalid, 0};
            return r;
        }
        if (fc.ec == std::errc::result_out_of_range) {
            r.err = {parse_errc::overflow, static_cast<s_t>(ptr - first)};
            return r;
        }
        if (ptr != last) {
            r.err = {parse_errc::trailing, static_cast<s_t>(ptr - first)};
            return r;
        }

        r.value = v;
        return r;
    }

    inline parse_result<int> to_i32(sv s, int base = 10, 
        parse_ws ws = parse_ws::none) noexcept {
        return to_int<int>(s, base, ws); 
    }
    inline parse_result<long long> to_i64(sv s, int base = 10, 
        parse_ws ws = parse_ws::none) noexcept { 
        return to_int<long long>(s, base, ws); 
    }
    inline parse_result<unsigned> to_u32(sv s, int base = 10, 
        parse_ws ws = parse_ws::none) noexcept { 
        return to_int<unsigned>(s, base, ws); 
    }
    inline parse_result<unsigned long long> to_u64(sv s, int base = 10, 
        parse_ws ws = parse_ws::none) noexcept { 
        return to_int<unsigned long long>(s, base, ws); 
    }


    // to float
    template <class Float>
    concept has_float_from_chars =
        std::is_floating_point_v<Float> &&
        requires(const char* a, const char* b, Float& v) {
        { std::from_chars(a, b, v, std::chars_format::general) } 
            -> std::same_as<std::from_chars_result>;
    };

    template <has_float_from_chars Float>
    inline parse_result<Float> to_float(sv s,
                                    parse_ws ws = parse_ws::none) noexcept {
        parse_result<Float> r{};

        if (ws == parse_ws::trim_ascii) s = trim_ascii(s);
        if (s.empty()) { 
            r.err = {parse_errc::empty, 0}; 
            return r; 
        }

        const char* first = s.data();
        const char* last  = s.data() + s.size();

            Float v{};
            auto fc = std::from_chars(first, last, v, std::chars_format::general);
            const char* ptr = fc.ptr;

            if (fc.ec == std::errc::invalid_argument) {
                r.err = {parse_errc::invalid, 0};
                return r;
            }
            if (fc.ec == std::errc::result_out_of_range) {
                r.err = {parse_errc::overflow, static_cast<s_t>(ptr - first)};
                return r;
            }
            if (ptr != last) {
                r.err = {parse_errc::trailing, static_cast<s_t>(ptr - first)};
                return r;
            }

            r.value = v;
            return r;
    }

    inline parse_result<float>  to_f32(sv s, parse_ws ws = parse_ws::none) noexcept { 
        return to_float<float>(s, ws); 
    }
    inline parse_result<double> to_f64(sv s, parse_ws ws = parse_ws::none) noexcept { 
        return to_float<double>(s, ws); 
    }

}