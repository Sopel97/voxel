#pragma once

namespace ls
{
    template <class T>
    constexpr bool almostZero(const T& val, const T& tolerance)
    {
        using std::abs;

        return abs(val) <= tolerance;
    }
    template <class T>
    constexpr bool almostEqual(const T& lhs, const T& rhs, const T& tolerance)
    {
        using std::abs;

        return abs(lhs - rhs) <= tolerance;
    }

    template <class T, class U>
    constexpr T lerp(const T& lhs, const T& rhs, const U& t)
    {
        return lhs + (rhs - lhs) * t;
    }
    template <class T, class U>
    constexpr T nlerp(const T& lhs, const T& rhs, const U& t)
    {
        return (lhs + (rhs - lhs) * t).normalized();
    }
    // evaluates v%p but converts negative results so that they are not symetric to positive ones
    template <class IntType>
    IntType periodic(IntType v, IntType p)
    {
        v %= p;
        IntType vp = (v >= 0) ? v : v + p;
        return vp;
    }

    template <class FloatType, class IntType = int>
    IntType floorToInt(const FloatType& value)
    {
        const int i = static_cast<IntType>(value);
        return i - (value < FloatType(0.0));
    }
}
