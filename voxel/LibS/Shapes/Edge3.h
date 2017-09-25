#pragma once

#include "Vec3.h"

#include <array>
#include <type_traits>

namespace ls
{
    template <class T>
    class Edge3
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        std::array<Vec3<T>, 2u> vertices;

        constexpr Edge3() = default;
        constexpr Edge3(const Vec3<T>& v1, const Vec3<T>& v2) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value);

        constexpr Edge3(const Edge3<T>&) = default;
        constexpr Edge3(Edge3<T>&&) = default;

        Edge3<T>& operator=(const Edge3<T>&) = default;
        Edge3<T>& operator=(Edge3<T> &&) = default;

        constexpr Edge3<T> translated(const Vec3<T>& displacement) const;
        Edge3<T>& translate(const Vec3<T>& displacement) &;

        template <class T2>
        explicit operator Edge3<T2>() const;

        constexpr T length() const;
        constexpr T lengthSquared() const;

        constexpr Vec3<T> centerOfMass() const;
    };

    using Edge3D = Edge3<double>;
    using Edge3F = Edge3<float>;

    template <class T>
    constexpr bool operator== (const Edge3<T>& lhs, const Edge3<T>& rhs);
    template <class T>
    constexpr bool operator!= (const Edge3<T>& lhs, const Edge3<T>& rhs);



    template <class T>
    constexpr Edge3<T>::Edge3(const Vec3<T>& v1, const Vec3<T>& v2) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value) :
        vertices{ v1, v2 }
    {
    }

    template <class T>
    constexpr Edge3<T> Edge3<T>::translated(const Vec3<T>& displacement) const
    {
        return Edge3<T>(vertices[0] + displacement, vertices[1] + displacement);
    }
    template <class T>
    Edge3<T>& Edge3<T>::translate(const Vec3<T>& displacement) &
    {
        vertices[0] += displacement;
        vertices[1] += displacement;

        return *this;
    }

    template <class T>
    template <class T2>
    Edge3<T>::operator Edge3<T2>() const
    {
        return Edge3<T2>(static_cast<Vec3<T2>>(vertices[0]), static_cast<Vec3<T2>>(vertices[1]));
    }

    template <class T>
    constexpr T Edge3<T>::length() const
    {
        return vertices[0].distance(vertices[1]);
    }

    template <class T>
    constexpr T Edge3<T>::lengthSquared() const
    {
        return vertices[0].distanceSquared(vertices[1]);
    }

    template <class T>
    constexpr Vec3<T> Edge3<T>::centerOfMass() const
    {
        return (vertices[0] + vertices[1]) * T(0.5);
    }

    template <class T>
    constexpr bool operator== (const Edge3<T>& lhs, const Edge3<T>& rhs)
    {
        return std::tie(lhs.vertices[0], lhs.vertices[1]) == std::tie(rhs.vertices[0], rhs.vertices[1]);
    }
    template <class T>
    constexpr bool operator!= (const Edge3<T>& lhs, const Edge3<T>& rhs)
    {
        return !(lhs == rhs);
    }
}
