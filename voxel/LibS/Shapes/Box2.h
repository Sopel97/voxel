#pragma once

#include "Vec2.h"

#include <type_traits>

namespace ls
{
    template <class T>
    class Box2
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;
        using VectorType = Vec2<T>;

        Vec2<T> min, max;

        constexpr Box2() noexcept = default;
        constexpr Box2(const Vec2<T>& min, const Vec2<T>& max) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value);

        static Box2<T> withSize(const Vec2<T>& min, const Vec2<T>& size) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value);
        static const Box2<T>& unitRectangle() noexcept;

        constexpr Box2(const Box2<T>&) = default;
        constexpr Box2(Box2<T>&&) noexcept = default;

        Box2<T>& operator =(const Box2<T>&) = default;
        Box2<T>& operator =(Box2<T> &&) noexcept = default;

        constexpr Box2<T> translated(const Vec2<T>& displacement) const;
        Box2<T>& translate(const Vec2<T>& displacement) &;

        template <class T2>
        explicit operator Box2<T2>() const;

        constexpr T width() const;
        constexpr T height() const;

        constexpr Vec2<T> centerOfMass() const;
        constexpr T area() const;
    };

    using Box2D = Box2<double>;
    using Box2F = Box2<float>;
    using Box2I = Box2<int>;

    template <class T>
    constexpr bool operator== (const Box2<T>& lhs, const Box2<T>& rhs);
    template <class T>
    constexpr bool operator!= (const Box2<T>& lhs, const Box2<T>& rhs);



    template <class T>
    constexpr Box2<T>::Box2(const Vec2<T>& min, const Vec2<T>& max) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value) :
        min(min),
        max(max)
    {
    }

    template <class T>
    constexpr Box2<T> Box2<T>::translated(const Vec2<T>& displacement) const
    {
        return Box2<T>(min + displacement, max + displacement);
    }
    template <class T>
    Box2<T>& Box2<T>::translate(const Vec2<T>& displacement) &
    {
        min += displacement;
        max += displacement;
        return *this;
    }

    template <class T>
    template <class T2>
    Box2<T>::operator Box2<T2>() const
    {
        return Box2<T2>(static_cast<Vec2<T2>>(min), static_cast<Vec2<T2>>(max));
    }

    template <class T>
    constexpr T Box2<T>::width() const
    {
        return max.x - min.x;
    }
    template <class T>
    constexpr T Box2<T>::height() const
    {
        return max.y - min.y;
    }

    template <class T>
    constexpr Vec2<T> Box2<T>::centerOfMass() const
    {
        return (min + max) * T(0.5);
    }
    template <class T>
    constexpr T Box2<T>::area() const
    {
        return width()*height();
    }

    template <class T>
    Box2<T> Box2<T>::withSize(const Vec2<T>& min, const Vec2<T>& size) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value)
    {
        return Box2<T>(min, min + size);
    }
    template <class T>
    const Box2<T>& Box2<T>::unitRectangle() noexcept
    {
        static const Box2<T> rect(Vec2<T>(T(0), T(0)), Vec2<T>(T(1), T(1)));
        return rect;
    }

    template <class T>
    constexpr bool operator== (const Box2<T>& lhs, const Box2<T>& rhs)
    {
        return std::tie(lhs.min, lhs.max) == std::tie(rhs.min, rhs.max);
    }
    template <class T>
    constexpr bool operator!= (const Box2<T>& lhs, const Box2<T>& rhs)
    {
        return !(lhs == rhs);
    }
}
