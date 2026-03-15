#pragma once

#include "mntstr/type.hpp"

#include <array>
#include <cstring>
#include <algorithm>
#include <string>

namespace mntstr {

    template <s_t InlineCap = 256>
    class string_builder {

        std::array<char, InlineCap> inline_{};
        s_t inline_size_ = 0;

        bool heap_mode_ = false;
        string heap_;

        s_t size_() const noexcept {
            return heap_mode_ ? static_cast<s_t>(heap_.size()) : inline_size_;
        }

        void ensure_capacity_(s_t need) {
            if (!heap_mode_) {
                if (need <= InlineCap) return;
                heap_mode_ = true;
                heap_.assign(inline_.data(), inline_size_);
                heap_.reserve(std::max<s_t>(need, InlineCap * 2));
                return;
            }
            if (need > heap_.capacity()) {
                const s_t cap = static_cast<s_t>(heap_.capacity());
                heap_.reserve(std::max<s_t>(need, cap * 2));
            }
        }

    public:

        string_builder() = default;
        explicit string_builder(s_t reserve_n) { ensure_capacity_(reserve_n); }

        s_t size() const noexcept { return size_(); }
        bool empty() const noexcept { return size() == 0; }

        void clear() noexcept {
            heap_.clear();
            heap_mode_ = false;
            inline_size_ = 0;
        }

        void reserve(s_t n) { ensure_capacity_(n); }

        void append(sv s) {
            const s_t old = size();
            const s_t need = old + static_cast<s_t>(s.size());
            ensure_capacity_(need);

            if (!heap_mode_) {
                std::memcpy(inline_.data() + inline_size_, s.data(), s.size());
                inline_size_ = need;
            } else {
                heap_.append(s.data(), s.size());
            }
        }

        void append(char c) {
            const s_t need = size() + 1;
            ensure_capacity_(need);

            if (!heap_mode_) {
                inline_[inline_size_++] = c;
            } else {
                heap_.push_back(c);
            }
        }

        void append_repeat(char c, s_t n) {
            if (n == 0) return;
            const s_t need = size() + n;
            ensure_capacity_(need);

            if (!heap_mode_) {
                std::memset(inline_.data() + inline_size_, static_cast<unsigned char>(c), n);
                inline_size_ += n;
            } else {
                heap_.append(n, c);
            }
        }

        sv view() const noexcept {
            if (!heap_mode_) return sv{inline_.data(), inline_size_};
            return sv{heap_.data(), heap_.size()};
        }

        string strval() {
            if (!heap_mode_) return string(inline_.data(), inline_size_);
            return std::move(heap_);
        }

    };
        
}