#pragma once


#include "gdl/base/approx.h"
#include "gdl/base/fundamentalTypes.h"

#include <cmath>

namespace GDL {



template <typename _type>
bool CheckArrayZero(const _type& a)
{
    for (U32 i = 0; i < a.size(); ++i)
        if (std::fabs(a[i]) > static_cast<decltype(a[i])>(0.))
            return false;
    return true;
}



template <typename _type>
bool CheckCloseArray(const _type& a, const _type& b)
{
    if (a.size() != b.size())
        return false;
    for (U32 i = 0; i < a.size(); ++i)
        if (a[i] != Approx(b[i]))
            return false;
    return true;
}

} // namespace GDL
