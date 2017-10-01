#pragma once

#include <type_traits>
#include <cmath>

#include "../MathConstants.h"

#include "../Common.h"

namespace ls
{
    namespace util
    {
        template <class T>
        constexpr T radiansToDegrees(const T& radians)
        {
            return radians * (T(180.0) / pi<T>);
        }
        template <class T>
        constexpr T degreesToRadians(const T& degrees)
        {
            return degrees * (pi<T> / T(180.0));
        }
    }
    template <class T>
    class Angle2 //directed angle 
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;

        constexpr static Angle2<T> radians(const T& rad) noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value);
        constexpr static Angle2<T> degrees(const T& deg) noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value);
        constexpr static const Angle2<T>& fullTurn();

        constexpr Angle2() noexcept = default;

        constexpr Angle2(const Angle2<T>&) = default;
        constexpr Angle2(Angle2<T>&&) noexcept = default;

        constexpr Angle2<T>& operator=(const Angle2<T>&) = default;
        constexpr Angle2<T>& operator=(Angle2<T> &&) noexcept = default;

        constexpr Angle2<T>& operator+=(const Angle2<T>& rhs);
        constexpr Angle2<T>& operator-=(const Angle2<T>& rhs);
        constexpr Angle2<T>& operator*=(const T& rhs);
        constexpr Angle2<T>& operator/=(const T& rhs);

        constexpr void normalize() &;
        constexpr Angle2<T> normalized() const; // [0, 2pi)

        template <class T2>
        constexpr explicit operator Angle2<T2>() const;

        constexpr T sin() const;
        constexpr T cos() const;
        constexpr T tan() const;
        constexpr T cot() const;

        constexpr static Angle2<T> asin(const T& val);
        constexpr static Angle2<T> acos(const T& val);
        constexpr static Angle2<T> atan(const T& val);
        constexpr static Angle2<T> acot(const T& val);

        constexpr T sinh() const;
        constexpr T cosh() const;
        constexpr T tanh() const;
        constexpr T coth() const;

        constexpr static Angle2<T> asinh(const T& val);
        constexpr static Angle2<T> acosh(const T& val);
        constexpr static Angle2<T> atanh(const T& val);
        constexpr static Angle2<T> acoth(const T& val);

        constexpr static Angle2<T> atan2(const T& y, const T& x);

        constexpr T radians() const;
        constexpr T degrees() const;
    protected:
    private:
        T m_radians;

        constexpr Angle2(const T& rad) noexcept(std::is_nothrow_constructible<T, T>::value);
    };

    using Angle2D = Angle2<double>;
    using Angle2F = Angle2<float>;

    template <class T>
    constexpr bool operator==(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr bool operator!=(const Angle2<T>& lhs, const Angle2<T>& rhs);

    template <class T>
    constexpr bool operator<(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr bool operator<=(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr bool operator>(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr bool operator>=(const Angle2<T>& lhs, const Angle2<T>& rhs);

    template <class T>
    constexpr Angle2<T> operator+(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr Angle2<T> operator-(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr Angle2<T> operator*(const Angle2<T>& lhs, const T& rhs);
    template <class T>
    constexpr Angle2<T> operator/(const Angle2<T>& lhs, const T& rhs);

    template <class T>
    constexpr Angle2<T> operator-(const Angle2<T>& angle);



    template <class T>
    constexpr Angle2<T>::Angle2(const T& rad) noexcept(std::is_nothrow_constructible<T, T>::value) :
        m_radians(rad)
    {
    }

    template <class T>
    constexpr Angle2<T>& Angle2<T>::operator+=(const Angle2<T>& rhs)
    {
        m_radians += rhs.m_radians;
        return *this;
    }
    template <class T>
    constexpr Angle2<T>& Angle2<T>::operator-=(const Angle2<T>& rhs)
    {
        m_radians -= rhs.m_radians;
        return *this;
    }
    template <class T>
    constexpr Angle2<T>& Angle2<T>::operator*=(const T& rhs)
    {
        m_radians *= rhs;
        return *this;
    }
    template <class T>
    constexpr Angle2<T>& Angle2<T>::operator/=(const T& rhs)
    {
        m_radians /= rhs;
        return *this;
    }

    template <class T>
    constexpr void Angle2<T>::normalize() &
    {
        const int fullTurns = floorToInt(m_radians / fullTurn().m_radians);
        m_radians -= fullTurns * fullTurn().m_radians;
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::normalized() const
    {
        Angle2<T> result(*this);

        result.normalize();

        return result;
    }

    template <class T>
    template <class T2>
    constexpr Angle2<T>::operator Angle2<T2>() const
    {
        return Angle2<T2>(static_cast<T2>(m_radians));
    }

    template <class T>
    constexpr T Angle2<T>::sin() const
    {
        using std::sin;

        return static_cast<T>(sin(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::cos() const
    {
        using std::cos;

        return static_cast<T>(cos(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::tan() const
    {
        using std::tan;

        return static_cast<T>(tan(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::cot() const
    {
        using std::tan;

        return T(1) / static_cast<T>(tan(m_radians));
    }

    template <class T>
    constexpr Angle2<T> Angle2<T>::asin(const T& val)
    {
        using std::asin;

        return Angle2<T>(static_cast<T>(asin(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::acos(const T& val)
    {
        using std::acos;

        return Angle2<T>(static_cast<T>(acos(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::atan(const T& val)
    {
        using std::atan;

        return Angle2<T>(static_cast<T>(atan(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::acot(const T& val)
    {
        using std::atan;

        return Angle2<T>(static_cast<T>(atan(T(1))) / val);
    }

    template <class T>
    constexpr T Angle2<T>::sinh() const
    {
        using std::sinh;

        return static_cast<T>(sinh(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::cosh() const
    {
        using std::cosh;

        return static_cast<T>(cosh(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::tanh() const
    {
        using std::tanh;

        return static_cast<T>(tanh(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::coth() const
    {
        using std::tanh;

        return T(1) / static_cast<T>(tanh(m_radians));
    }

    template <class T>
    constexpr Angle2<T> Angle2<T>::asinh(const T& val)
    {
        using std::atanh;

        return Angle2<T>(static_cast<T>(atanh(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::acosh(const T& val)
    {
        using std::acosh;

        return Angle2<T>(static_cast<T>(acosh(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::atanh(const T& val)
    {
        using std::atanh;

        return Angle2<T>(static_cast<T>(atanh(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::acoth(const T& val)
    {
        using std::atanh;

        return Angle2<T>(static_cast<T>(atanh(T(1))) / val);
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::atan2(const T& y, const T& x)
    {
        using std::atan2;

        return Angle2<T>(static_cast<T>(std::atan2(y, x)));
    }

    template <class T>
    constexpr T Angle2<T>::radians() const
    {
        return m_radians;
    }
    template <class T>
    constexpr T Angle2<T>::degrees() const
    {
        return util::radiansToDegrees(m_radians);
    }

    template <class T>
    constexpr Angle2<T> Angle2<T>::radians(const T& rad) noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value)
    {
        return Angle2<T>(rad);
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::degrees(const T& deg) noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value)
    {
        return Angle2<T>(util::degreesToRadians(deg));
    }
    template <class T>
    constexpr const Angle2<T>& Angle2<T>::fullTurn()
    {
        constexpr static Angle2<T> angle(T(2.0) * pi<T>);
        return angle;
    }

    template <class T>
    constexpr bool operator==(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return lhs.radians() == rhs.radians();
    }
    template <class T>
    constexpr bool operator!=(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class T>
    constexpr bool operator<(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return lhs.radians() < rhs.radians();
    }
    template <class T>
    constexpr bool operator<=(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    constexpr bool operator>(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return rhs.radians() < lhs.radians();
    }
    template <class T>
    constexpr bool operator>=(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return !(lhs < rhs);
    }

    template <class T>
    constexpr Angle2<T> operator+(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return Angle2<T>::radians(lhs.radians() + rhs.radians());
    }
    template <class T>
    constexpr Angle2<T> operator-(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return Angle2<T>::radians(lhs.radians() - rhs.radians());
    }
    template <class T>
    constexpr Angle2<T> operator*(const Angle2<T>& lhs, const T& rhs)
    {
        return Angle2<T>::radians(lhs.radians() * rhs);
    }
    template <class T>
    constexpr Angle2<T> operator/(const Angle2<T>& lhs, const T& rhs)
    {
        return Angle2<T>::radians(lhs.radians() / rhs);
    }

    template <class T>
    constexpr Angle2<T> operator-(const Angle2<T>& angle)
    {
        return Angle2<T>::radians(-angle.radians());
    }
}
