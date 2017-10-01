#pragma once

#include "Vec3.h"

#include <type_traits>

namespace ls
{
    template <class T>
    class Ray3
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        constexpr Ray3() noexcept = default;
        constexpr Ray3(const Vec3<T>& origin, const Vec3<T>& direction) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value);

        constexpr Ray3(const Ray3<T>&) = default;
        constexpr Ray3(Ray3<T>&&) noexcept = default;

        Ray3<T>& operator=(const Ray3<T>&) = default;
        Ray3<T>& operator=(Ray3<T> &&) noexcept = default;

        constexpr Ray3<T> translated(const Vec3<T>& displacement) const;
        Ray3<T>& translate(const Vec3<T>& displacement) &;

        template <class T2>
        explicit operator Ray3<T2>() const;

    private:
        Vec3<T> m_origin;
        Vec3<T> m_direction;
    };

    template <class T>
    constexpr bool operator== (const Ray3<T>& lhs, const Ray3<T>& rhs);
    template <class T>
    constexpr bool operator!= (const Ray3<T>& lhs, const Ray3<T>& rhs);

    using Ray3D = Ray3<double>;
    using Ray3F = Ray3<float>;



    template <class T>
    constexpr Ray3<T>::Ray3(const Vec3<T>& origin, const Vec3<T>& direction) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value) :
        m_origin(origin),
        m_direction(direction.normalized())
    {

    }

    template <class T>
    constexpr Ray3<T> Ray3<T>::translated(const Vec3<T>& displacement) const
    {
        return Ray3<T>(m_origin + displacement, m_direction);
    }
    template <class T>
    Ray3<T>& Ray3<T>::translate(const Vec3<T>& displacement) &
    {
        m_origin += displacement;
        return *this;
    }

    template <class T>
    template <class T2>
    Ray3<T>::operator Ray3<T2>() const
    {
        return Ray3<T2>(static_cast<Vec3<T2>>(m_origin), static_cast<Vec3<T2>>(m_direction));
    }

    template <class T>
    constexpr bool operator== (const Ray3<T>& lhs, const Ray3<T>& rhs)
    {
        return std::tie(lhs.m_origin, lhs.m_direction) == std::tie(rhs.m_origin, rhs.m_direction);
    }
    template <class T>
    constexpr bool operator!= (const Ray3<T>& lhs, const Ray3<T>& rhs)
    {
        return !(lhs == rhs);
    }
}
