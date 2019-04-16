#pragma once

#include "gdl/math/serial/quatSerial.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"


namespace GDL
{

template <typename _type>
QuatSerial<_type>::QuatSerial()
    : mData{{0, 0, 0, 0}}
{
}



template <typename _type>
QuatSerial<_type>::QuatSerial(std::array<_type, 4> data)
    : mData{data}
{
}



template <typename _type>
QuatSerial<_type>::QuatSerial(_type x, _type y, _type z, _type w)
    : mData{{x, y, z, w}}
{
}



template <typename _type>
_type QuatSerial<_type>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 3, "Invalid index value! [0..3]");
    return mData[index];
}



template <typename _type>
bool QuatSerial<_type>::operator==(const QuatSerial& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < 4; ++i)
        result = result && mData[i] == Approx(rhs.mData[i]);
    return result;
}



template <typename _type>
bool QuatSerial<_type>::operator!=(const QuatSerial& rhs) const
{
    return !(operator==(rhs));
}



template <typename _type>
const std::array<_type, 4> QuatSerial<_type>::Data() const
{
    return mData;
}



// LCOV_EXCL_START

template <typename _type>
std::ostream& operator<<(std::ostream& os, const QuatSerial<_type>& quat)
{
    os << "| " << quat[0] << " |\n| " << quat[1] << " |\n| " << quat[2] << " |\n| " << quat[3] << " |" << std::endl;
    return os;
}



// LCOV_EXCL_STOP

} // namespace GDL
