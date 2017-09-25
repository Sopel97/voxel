#pragma once

#include "Vec3.h"
#include "Box3.h"

#include <type_traits>

namespace ls
{
    template <class T>
    class Sphere3
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        Vec3<T> origin;
        T radius;

        constexpr Sphere3() = default;
        constexpr Sphere3(const Vec3<T>& origin, const T& radius) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value && std::is_nothrow_copy_constructible<T>::value);

        constexpr Sphere3(const Sphere3<T>&) = default;
        constexpr Sphere3(Sphere3<T>&&) = default;

        Sphere3<T>& operator =(Sphere3<T> &&) = default;
        Sphere3<T>& operator =(const Sphere3<T>&) = default;

        constexpr Sphere3<T> translated(const Vec3<T>& displacement) const;
        Sphere3<T>& translate(const Vec3<T>& displacement) &;

        Box3<T> aabb() const;

        template <class T2>
        explicit operator Sphere3<T2>() const;

        constexpr T volume() const;
    };

    using Sphere3D = Sphere3<double>;
    using Sphere3F = Sphere3<float>;

    template <class T>
    constexpr bool operator==(const Sphere3<T>& lhs, const Sphere3<T>& rhs);
    template <class T>
    constexpr bool operator!=(const Sphere3<T>& lhs, const Sphere3<T>& rhs);



    template <class T>
    constexpr Sphere3<T>::Sphere3(const Vec3<T>& origin, const T& radius) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value && std::is_nothrow_copy_constructible<T>::value) :
        origin(origin),
        radius(radius)
    {
    }

    template <class T>
    constexpr Sphere3<T> Sphere3<T>::translated(const Vec3<T>& displacement) const
    {
        return Sphere3<T>(origin + displacement, radius);
    }
    template <class T>
    Sphere3<T>& Sphere3<T>::translate(const Vec3<T>& displacement) &
    {
        origin += displacement;
        return *this;
    }
    template <class T>
    Box3<T> Sphere3<T>::aabb() const
    {
        const Vec3<T> halfDiagonal(radius, radius, radius);
        return Box3<T>(origin - halfDiagonal, origin + halfDiagonal);
    }

    template <class T>
    template <class T2>
    Sphere3<T>::operator Sphere3<T2>() const
    {
        return Sphere3<T2>(static_cast<Vec3<T2>>(origin), static_cast<T2>(radius));
    }

    template <class T>
    constexpr T Sphere3<T>::volume() const
    {
        return radius*radius*radius*(pi<T>*T(4.0/3.0));
    }

    template <class T>
    constexpr bool operator==(const Sphere3<T>& lhs, const Sphere3<T>& rhs)
    {
        return std::tie(lhs.origin, lhs.radius) == std::tie(rhs.origin, rhs.radius);
    }
    template <class T>
    constexpr bool operator!=(const Sphere3<T>& lhs, const Sphere3<T>& rhs)
    {
        return !(lhs == rhs);
    }
}
