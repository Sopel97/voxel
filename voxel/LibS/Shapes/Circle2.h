#pragma once

#include "Vec2.h"
#include "Box2.h"

#include <type_traits>

namespace ls
{
    template <class T>
    class Circle2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec2<T>;

        Vec2<T> origin;
        T radius;

        constexpr Circle2() = default;
        constexpr Circle2(const Vec2<T>& origin, const T& radius) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value && std::is_nothrow_copy_constructible<T>::value);

        constexpr Circle2(const Circle2<T>&) = default;
        constexpr Circle2(Circle2<T>&&) = default;

        Circle2<T>& operator =(Circle2<T> &&) = default;
        Circle2<T>& operator =(const Circle2<T>&) = default;

        constexpr Circle2<T> translated(const Vec2<T>& displacement) const;
        Circle2<T>& translate(const Vec2<T>& displacement) &;

        Box2<T> aabb() const;

        template <class T2>
        explicit operator Circle2<T2>() const;

        constexpr T area() const;
    };

    using Circle2D = Circle2<double>;
    using Circle2F = Circle2<float>;

    template <class T>
    constexpr bool operator==(const Circle2<T>& lhs, const Circle2<T>& rhs);
    template <class T>
    constexpr bool operator!=(const Circle2<T>& lhs, const Circle2<T>& rhs);



    template <class T>
    constexpr Circle2<T>::Circle2(const Vec2<T>& origin, const T& radius) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value && std::is_nothrow_copy_constructible<T>::value) :
        origin(origin),
        radius(radius)
    {
    }

    template <class T>
    constexpr Circle2<T> Circle2<T>::translated(const Vec2<T>& displacement) const
    {
        return Circle2<T>(origin + displacement, radius);
    }
    template <class T>
    Circle2<T>& Circle2<T>::translate(const Vec2<T>& displacement) &
    {
        origin += displacement;
        return *this;
    }
    template <class T>
    Box2<T> Circle2<T>::aabb() const
    {
        const Vec2<T> halfDiagonal(radius, radius);
        return Box2<T>(origin - halfDiagonal, origin + halfDiagonal);
    }

    template <class T>
    template <class T2>
    Circle2<T>::operator Circle2<T2>() const
    {
        return Circle2<T2>(static_cast<Vec2<T2>>(origin), static_cast<T2>(radius));
    }

    template <class T>
    constexpr T Circle2<T>::area() const
    {
        return radius*radius*pi<T>;
    }

    template <class T>
    constexpr bool operator==(const Circle2<T>& lhs, const Circle2<T>& rhs)
    {
        return std::tie(lhs.origin, lhs.radius) == std::tie(rhs.origin, rhs.radius);
    }
    template <class T>
    constexpr bool operator!=(const Circle2<T>& lhs, const Circle2<T>& rhs)
    {
        return !(lhs == rhs);
    }
}
