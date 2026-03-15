#include "mntstr/core.hpp"

#include <cassert>

using string = mntstr::string;
using sv = mntstr::sv;

int main() {

    string s1 = "  123abDF. ";
    string s2 = "123";
    string s3 = "abd";

    sv s1v = mntstr::as_sv(s1);
    sv s2v = mntstr::as_sv(s2);
    sv s3v = mntstr::as_sv(s3);

    auto s1vtrim = mntstr::base::trim_ascii(s1v);
    auto r = mntstr::base::to_i32(s2v);
    
    assert(r);
    assert(r.value == 123);

    assert(mntstr::starts_with(s1vtrim, s2v));
    assert(!mntstr::ends_with(s1vtrim, mntstr::as_sv(" ")));

    assert(mntstr::base::contains(s1v, s1vtrim));
    assert(mntstr::base::icontains_ascii(s1v, s3v));

    return 0;
}