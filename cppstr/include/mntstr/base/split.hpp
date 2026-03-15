#pragma once

#include "mntstr/type.hpp"
#include "mntstr/ascii_base.hpp"
#include "mntstr/view.hpp"
#include "mntstr/base/find.hpp"
#include "mntstr/base/delim.hpp"

#include <iterator>

namespace mntstr::base {

    enum class split_mode : uc {
        keep_empty,
        skip_empty
    };

    template <class Delim>
    class basic_split_view {
        sv s_;
        Delim delim_;
        split_mode mode_;
    public:
        basic_split_view(sv s, Delim delim, split_mode mode) noexcept
            : s_(s), delim_(delim), mode_(mode) {}

        struct end_sentinel {};

        class iterator {
            sv s_;
            Delim delim_;
            split_mode mode_;
            s_t pos_ = 0;
            bool end_ = false;
            sv cur_{};

            static constexpr s_t npos_() noexcept {
                return static_cast<s_t>(sv::npos); 
            }

            void advance_() noexcept {
                const s_t n = static_cast<s_t>(s_.size());
                const s_t dsz = delim_.size();

                if (pos_ > n) { end_ = true; cur_ = sv{}; return; }

                if (dsz == 0) {
                    const s_t n = static_cast<s_t>(s_.size());
                    cur_ = s_;
                    pos_ = n + 1;
                    if (mode_ == split_mode::skip_empty && cur_.empty()) {
                        end_ = true; cur_ = sv{};
                    }
                    return;
                }

                while (true) {
                    const s_t found = find(s_, delim_.raw(), pos_);
                    if (found == npos_()) {
                        cur_ = substr_safe(s_, pos_);
                        pos_ = n + 1;
                    } else {
                        cur_ = substr_safe(s_, pos_, found - pos_);
                        pos_ = found + dsz;
                    }
                    if (mode_ == split_mode::skip_empty && cur_.empty()) {
                        continue;
                    }
                    return;
                }
            }

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = sv;
            using difference_type = std::ptrdiff_t;
            using reference = sv;

            iterator() noexcept = default;

            iterator(sv s, Delim delim, split_mode mode) noexcept
                : s_(s), delim_(delim), mode_(mode) {
                advance_();
            }

            reference operator*() const noexcept { return cur_; }

            iterator& operator++() noexcept {
                advance_();
                return *this;
            }
            iterator operator++(int) noexcept {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const iterator& it, end_sentinel) noexcept { return it.end_; }
            friend bool operator!=(const iterator& it, end_sentinel s) noexcept { return !(it == s); }
        
        };

        iterator begin() const noexcept { return iterator{s_, delim_, mode_}; }
        end_sentinel end() const noexcept { return {}; }

    };

    // split

    inline auto split(sv s, char delim, split_mode mode = split_mode::keep_empty) noexcept
        -> basic_split_view<delim_char> {
        return {s, delim_char{delim}, mode};
    }

    inline auto split(sv s, sv delim, split_mode mode = split_mode::keep_empty) noexcept
        -> basic_split_view<delim_sv> {
        return {s, delim_sv{delim}, mode};
    }

    // spplit_once

    inline std::optional<std::pair<sv, sv>> split_once(sv s, char delim) noexcept {
        const s_t pos = find(s, delim);
        if (pos == static_cast<s_t>(sv::npos)) return std::nullopt;
        return std::pair<sv, sv>{ substr_safe(s, 0, pos), substr_safe(s, pos + 1) };
    }

    inline std::optional<std::pair<sv, sv>> split_once(sv s, sv delim) noexcept {
        if (delim.empty()) return std::pair<sv, sv>{sv{}, s};

        const s_t pos = find(s, delim);
        if (pos == static_cast<s_t>(sv::npos)) return std::nullopt;
        const s_t dsz = static_cast<s_t>(delim.size());
        return std::pair<sv, sv>{ substr_safe(s, 0, pos), substr_safe(s, pos + dsz) };
    }

    // rsplit_once

    inline std::optional<std::pair<sv, sv>> rsplit_once(sv s, char delim) noexcept {
        const s_t pos = rfind(s, delim);
        if (pos == static_cast<s_t>(sv::npos)) return std::nullopt;
        return std::pair<sv, sv>{ substr_safe(s, 0, pos), substr_safe(s, pos + 1) };
    }

    inline std::optional<std::pair<sv, sv>> rsplit_once(sv s, sv delim) noexcept {
        if (delim.empty()) return std::pair<sv, sv>{s, sv{}};
        
        const s_t pos = rfind(s, delim);
        if (pos == static_cast<s_t>(sv::npos)) return std::nullopt;
        const s_t dsz = static_cast<s_t>(delim.size());
        return std::pair<sv, sv>{ substr_safe(s, 0, pos), substr_safe(s, pos + dsz) };
    }

    // lines
    class lines_view {
        sv s_;

    public:
        explicit lines_view(sv s) noexcept : s_(s) {}
        struct end_sentinel {};

        class iterator {
            sv s_;
            s_t pos_ = 0;
            bool end_ = false;
            sv cur_{};

            static sv strip_cr_(sv x) noexcept {
                if (!x.empty() && x.back() == '\r') x.remove_suffix(1);
                return x;
            }

            void advance_() noexcept {
                const s_t n = static_cast<s_t>(s_.size());
                if (pos_ > n) { end_ = true; cur_ = sv{}; return; }

                const s_t found = find(s_, '\n', pos_);

                if (found == static_cast<s_t>(sv::npos)) {
                    cur_ = strip_cr_(substr_safe(s_, pos_));
                    pos_ = n + 1;
                    return;
                }

                cur_ = strip_cr_(substr_safe(s_, pos_, found - pos_));
                pos_ = found + 1;
            }

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = sv;
            using difference_type = std::ptrdiff_t;
            using reference = sv;

            iterator() noexcept = default;

            explicit iterator(sv s) noexcept : s_(s) { advance_(); }

            reference operator*() const noexcept { return cur_; }

            iterator& operator++() noexcept { 
                advance_(); 
                return *this; 
            }
            iterator operator++(int) noexcept {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const iterator& it, end_sentinel) noexcept { return it.end_; }
            friend bool operator!=(const iterator& it, end_sentinel s) noexcept { return !(it == s); }
        };

        iterator begin() const noexcept { return iterator{s_}; }
        end_sentinel end() const noexcept { return {}; }

    };

    inline lines_view lines(sv s) noexcept { return lines_view{s}; }

    // split_whitespace
    class split_whitespace_view {
        sv s_;

    public:
        explicit split_whitespace_view(sv s) noexcept : s_(s) {}

        struct end_sentinel {};

        class iterator {
            sv s_;
            s_t i_ = 0;
            bool end_ = false;
            sv cur_{};

            static bool is_space_(char c) noexcept {
                return mntstr::ascii_is_space(static_cast<uc>(c));
            }

            void skip_spaces_() noexcept {
                const s_t n = static_cast<s_t>(s_.size());
                while (i_ < n && is_space_(s_[i_])) ++i_;
            }

            void advance_() noexcept {
                const s_t n = static_cast<s_t>(s_.size());
                skip_spaces_();

                if (i_ > n) { end_ = true; cur_ = sv{}; return; }

                const s_t start = i_;
                while (i_ < n && !is_space_(s_[i_])) ++i_;
                cur_ = substr_safe(s_, start, i_ - start);
            }

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = sv;
            using difference_type = std::ptrdiff_t;
            using reference = sv;

            iterator() noexcept = default;
            explicit iterator(sv s) noexcept : s_(s) { advance_(); }

            reference operator*() const noexcept { return cur_; }
            
            iterator& operator++() noexcept {
                advance_();
                return *this;
            }
            iterator operator++(int) noexcept {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const iterator& it, end_sentinel) noexcept { return it.end_; }
            friend bool operator!=(const iterator& it, end_sentinel s) noexcept { return !(it == s); }
        };

        iterator begin() const noexcept { return iterator{s_}; }
        end_sentinel end() const noexcept { return {}; }
    };

    inline split_whitespace_view split_whitespace(sv s) noexcept { return split_whitespace_view{s}; }

}