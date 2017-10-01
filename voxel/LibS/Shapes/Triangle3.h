#pragma once

#include "Vec3.h"

#include <array>
#include <type_traits>

namespace ls
{
    template <class T>
    class Triangle3
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        std::array<Vec3<T>, 3u> vertices;

        constexpr Triangle3() noexcept = default;
        constexpr Triangle3(const Vec3<T>& v1, const Vec3<T>& v2, const Vec3<T>& v3) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value);

        constexpr Triangle3(const Triangle3<T>&) = default;
        constexpr Triangle3(Triangle3<T>&&) noexcept = default;

        Triangle3<T>& operator =(const Triangle3<T>&) = default;
        Triangle3<T>& operator =(Triangle3<T>&&) noexcept = default;

        constexpr Triangle3<T> translated(const Vec3<T>& displacement) const;
        Triangle3<T>& translate(const Vec3<T>& displacement) &;

        template <class T2>
        explicit operator Triangle3<T2>() const;

        constexpr Vec3<T> centerOfMass() const;
        constexpr T area() const;
    };

    using Triangle3D = Triangle3<double>;
    using Triangle3F = Triangle3<float>;


    template <class T>
    constexpr Triangle3<T>::Triangle3(const Vec3<T>& v1, const Vec3<T>& v2, const Vec3<T>& v3) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value) :
        vertices{ v1, v2, v3 }
    {
    }

    template <class T>
    constexpr Triangle3<T> Triangle3<T>::translated(const Vec3<T>& displacement) const
    {
        return Triangle3<T>(vertices[0] + displacement, vertices[1] + displacement, vertices[2] + displacement);
    }
    template <class T>
    Triangle3<T>& Triangle3<T>::translate(const Vec3<T>& displacement) &
    {
        vertices[0] += displacement;
        vertices[1] += displacement;
        vertices[2] += displacement;
        return *this;
    }

    template <class T>
    template <class T2>
    Triangle3<T>::operator Triangle3<T2>() const
    {
        return Triangle3<T2>(static_cast<Vec3<T2>>(vertices[0]), static_cast<Vec3<T2>>(vertices[1]), static_cast<Vec3<T2>>(vertices[2]));
    }

    template <class T>
    constexpr Vec3<T> Triangle3<T>::centerOfMass() const
    {
        return (vertices[0] + vertices[1] + vertices[2]) / T(3);
    }
    template <class T>
    constexpr T Triangle3<T>::area() const
    {
        const auto v1 = vertices[1] - vertices[0];
        const auto v2 = vertices[2] - vertices[0];
        return v1.cross(v2).length()*T(0.5);
    }
}
