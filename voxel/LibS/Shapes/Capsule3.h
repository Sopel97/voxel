#pragma once

#include "Vec3.h"

#include <array>
#include <type_traits>

namespace ls
{
    template <class T>
    class Capsule3
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        std::array<Vec3<T>, 2u> vertices;
        T radius;

        constexpr Capsule3() = default;
        constexpr Capsule3(const Vec3<T>& v1, const Vec3<T>& v2, const T& radius) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value);

        constexpr Capsule3(const Capsule3<T>&) = default;
        constexpr Capsule3(Capsule3<T>&&) = default;

        Capsule3<T>& operator=(const Capsule3<T>&) = default;
        Capsule3<T>& operator=(Capsule3<T> &&) = default;

        constexpr Capsule3<T> translated(const Vec3<T>& displacement) const;
        Capsule3<T>& translate(const Vec3<T>& displacement) &;

        template <class T2>
        explicit operator Capsule3<T2>() const;

        constexpr Vec3<T> centerOfMass() const;
    };

    using Capsule3D = Capsule3<double>;
    using Capsule3F = Capsule3<float>;

    template <class T>
    constexpr bool operator== (const Capsule3<T>& lhs, const Capsule3<T>& rhs);
    template <class T>
    constexpr bool operator!= (const Capsule3<T>& lhs, const Capsule3<T>& rhs);



    template <class T>
    constexpr Capsule3<T>::Capsule3(const Vec3<T>& v1, const Vec3<T>& v2, const T& radius) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value) :
        vertices{ v1, v2 },
        radius(radius)
    {
    }

    template <class T>
    constexpr Capsule3<T> Capsule3<T>::translated(const Vec3<T>& displacement) const
    {
        return Capsule3<T>(vertices[0] + displacement, vertices[1] + displacement, radius);
    }
    template <class T>
    Capsule3<T>& Capsule3<T>::translate(const Vec3<T>& displacement) &
    {
        vertices[0] += displacement;
        vertices[1] += displacement;

        return *this;
    }

    template <class T>
    template <class T2>
    Capsule3<T>::operator Capsule3<T2>() const
    {
        return Capsule3<T2>(static_cast<Vec3<T2>>(vertices[0]), static_cast<Vec3<T2>>(vertices[1]));
    }

    template <class T>
    constexpr Vec3<T> Capsule3<T>::centerOfMass() const
    {
        return (vertices[0] + vertices[1]) * T(0.5);
    }

    template <class T>
    constexpr bool operator== (const Capsule3<T>& lhs, const Capsule3<T>& rhs)
    {
        return std::tie(lhs.vertices[0], lhs.vertices[1], radius) == std::tie(rhs.vertices[0], rhs.vertices[1], radius);
    }
    template <class T>
    constexpr bool operator!= (const Capsule3<T>& lhs, const Capsule3<T>& rhs)
    {
        return !(lhs == rhs);
    }
}
