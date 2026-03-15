#pragma once

#include "mntstr/type.hpp"
#include "mntstr/ascii_base.hpp"
#include "mntstr/builder.hpp"
#include "mntstr/view.hpp"
#include "mntstr/base/find.hpp"

namespace mntstr::base {
    inline string replace(sv s, sv needle, sv target, s_t pos = 0) noexcept {
        if (!needle.compare(target)) {
            return string{s};
        }
        
        const s_t l = static_cast<s_t>(needle.size());
        if (!l) return string{s};

        const s_t n = static_cast<s_t>(s.size());
        if (pos > n) return string{s};

        const s_t p = static_cast<s_t>(find(s, needle, pos));
        if (p == static_cast<s_t>(sv::npos)) return string{s};

        string_builder ns{};
        ns.reserve(n-l+target.size());
        
        ns.append(substr_safe(s, 0, p));
        ns.append(target);
        ns.append(substr_safe(s, p+l));

        return ns.strval();
    }

    inline string replace_all(sv s, sv needle, sv target) {
        if (!needle.compare(target)) {
            return string{s};
        }

        const s_t l = static_cast<s_t>(needle.size());
        if (!l) return string{s};

        s_t cur = 0;
        string_builder ns{};
        
        while (true) {
            const s_t p = static_cast<s_t>(find(s, needle, cur));
            if (p == static_cast<s_t>(sv::npos)) {
                ns.append(substr_safe(s, cur));
                return ns.strval();
            }
            ns.append(substr_safe(s, cur, p-cur));
            ns.append(target);
            cur = p+l;
        }
    }
}