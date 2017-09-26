#pragma once

#include <type_traits>
#include <cmath>

#include "../Common.h"

namespace ls
{
    /**
    * Class representing 3 dimensional vector
    */
    template <class T>
    class Vec3
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        T x, y, z;

        /**
        * Default constructor
        *
        * Does not initialize the components
        */
        constexpr Vec3() = default;

        /**
        * Constructor
        *
        * @param xyz Value to be assigned to all components
        */
        constexpr explicit Vec3(const T& xyz) noexcept(std::is_nothrow_copy_constructible<T>::value);

        /**
        * Constructor
        *
        * @param x Value to be assigned to the x component
        * @param y Value to be assigned to the y component
        * @param z Value to be assigned to the z component
        */
        constexpr Vec3(const T& x, const T& y, const T& z) noexcept(std::is_nothrow_copy_constructible<T>::value);

        /**
        * Named constructor that returns a zero vector
        *
        * @return A zero vector
        */
        constexpr static const Vec3<T>& zero() noexcept;

        /**
        * Named constructor that returns a (1, 1, 1) vector
        *
        * @return A (1, 1, 1) vector
        */
        constexpr static const Vec3<T>& one() noexcept;

        /**
        * Default copy constructor
        */
        constexpr Vec3(const Vec3<T>&) = default;

        /**
        * Default move constructor
        */
        constexpr Vec3(Vec3<T>&&) = default;

        /**
        * Default copy assign
        */
        Vec3<T>& operator=(const Vec3<T>&) = default;

        /**
        * Default move assign
        */
        Vec3<T>& operator=(Vec3<T>&&) = default;

        /**
        * Adds a vector to this
        *
        * @param rhs Vector to be added to this
        * @return reference to this after operation
        */
        Vec3<T>& operator+=(const Vec3<T>& rhs);

        /**
        * Subtracts a vector from this
        *
        * @param rhs Vector to be subtracted from this
        * @return reference to this after operation
        */
        Vec3<T>& operator-=(const Vec3<T>& rhs);

        /**
        * Multiplies this vector in place by a scalar
        *
        * @param rhs Scalar to multiply by
        * @return reference to this after operation
        */
        Vec3<T>& operator*=(const T& rhs);

        /**
        * Multiplies this vector in place by a another one
        * This is component-wise multiplication
        *
        * @param rhs Vector to multiply by
        * @return reference to this after operation
        */
        Vec3<T>& operator*=(const Vec3<T>& rhs);

        /**
        * Divides this vector in place by a scalar
        *
        * @param rhs Scalar to divide by
        * @return reference to this after operation
        */
        Vec3<T>& operator/=(const T& rhs);

        /**
        * Divides this vector in place by a another one
        * This is component-wise division
        *
        * @param rhs Vector to divide by
        * @return reference to this after operation
        */
        Vec3<T>& operator/=(const Vec3<T>& rhs);

        /**
        * Accesses component by index
        *
        * @param i Zero-based index of the component
        * @return Const refence to the component
        */
        const T& operator[](int i) const;

        /**
        * Accesses component by index
        *
        * @param i Zero-based index of the component
        * @return Reference to the component
        */
        T& operator[](int i);

        /**
        * Converts to a Vec3 with different underlying type
        */
        template <class T2>
        explicit operator Vec3<T2>() const;

        /**
        * Returns the length of the vector
        *
        * @return The length of this vector
        */
        constexpr T length() const;

        /**
        * Returns the squared length of the vector
        *
        * @return The quared length of this vector
        */
        constexpr T lengthSquared() const;

        /**
        * Computes the distance between to vectors
        *
        * @param other Vector to compute distance to
        * @return Distance between vectors
        */
        constexpr T distance(const Vec3<T>& other) const;

        /**
        * Computes the squared distance between to vectors
        *
        * @param other Vector to compute squared distance to
        * @return Squared distance between vectors
        */
        constexpr T distanceSquared(const Vec3<T>& other) const;

        /**
        * Normalizes this vector in place (divides itself by the length)
        */
        void normalize() &;

        /**
        * Returns a normalized copy of this
        *
        * @return A normalized copy of this
        */
        Vec3<T> normalized() const;

        /**
        * Performs a dot product on this and a given vector
        *
        * @param rhs Vector to dot with
        * @return The dot product of the two vectors
        */
        T dot(const Vec3<T>& rhs) const;

        /**
        * Performs a cross product on this and a given vector
        * Determinant of a 2x2 matrix made by these vectors
        *
        * @param rhs Vector to cross with
        * @return The cross product of the two vectors
        */
        Vec3<T> cross(const Vec3<T>& rhs) const;

        /**
        * Named constructor returning unit vector on OX axis
        *
        * @return Vector (1, 0, 0)
        */
        static constexpr const Vec3<T>& unitX() noexcept;

        /**
        * Named constructor returning unit vector on OY axis
        *
        * @return Vector (0, 1, 0)
        */
        static constexpr const Vec3<T>& unitY() noexcept;

        /**
        * Named constructor returning unit vector on OZ axis
        *
        * @return Vector (0, 0, 1)
        */
        static constexpr const Vec3<T>& unitZ() noexcept;
    };

    using Vec3D = Vec3<double>;
    using Vec3F = Vec3<float>;
    using Vec3I = Vec3<int>;

    /**
    * Compares vectors for equality
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether vector are exactly equal
    */
    template <class T>
    constexpr bool operator==(const Vec3<T>& lhs, const Vec3<T>& rhs);

    /**
    * Compares vectors for inequality
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether vector are not exactly equal
    */
    template <class T>
    constexpr bool operator!=(const Vec3<T>& lhs, const Vec3<T>& rhs);

    /**
    * Orders vectors (total order)
    * First compares x, if it is equal then compares y, then z
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is less than rhs
    */
    template <class T>
    constexpr bool operator<(const Vec3<T>& lhs, const Vec3<T>& rhs);

    /**
    * Orders vectors (total order)
    * First compares x, if it is equal then compares y, then z
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is greater than rhs
    */
    template <class T>
    constexpr bool operator>(const Vec3<T>& lhs, const Vec3<T>& rhs);

    /**
    * Orders vectors
    * First compares x, if it is equal then compares y, then z
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is less or equal than rhs
    */
    template <class T>
    constexpr bool operator<=(const Vec3<T>& lhs, const Vec3<T>& rhs);

    /**
    * Orders vectors
    * First compares x, if it is equal then compares y, then z
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is greater or equal than rhs
    */
    template <class T>
    constexpr bool operator>=(const Vec3<T>& lhs, const Vec3<T>& rhs);

    /**
    * Returns negation of a vector
    *
    * @param vector Vector to negate
    * @return Negated copy of a vector
    */
    template <class T>
    constexpr Vec3<T> operator-(const Vec3<T>& vector);

    /**
    * Adds two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Sum of the two vectors
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() + std::declval<T2>())>
    constexpr Vec3<R> operator+(const Vec3<T1>& lhs, const Vec3<T2>& rhs);

    /**
    * Subtracts two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs - rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() - std::declval<T2>())>
    constexpr Vec3<R> operator-(const Vec3<T1>& lhs, const Vec3<T2>& rhs);

    /**
    * Multiplies vector by scalar
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs*rhs
    */
    template <class T1, class T2, class R = std::enable_if_t<std::is_arithmetic_v<T1>, decltype(std::declval<T1>() * std::declval<T2>())>>
    constexpr Vec3<R> operator*(const Vec3<T1>& lhs, const T2& rhs);

    /**
    * Divides vector by scalar
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs/rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec3<R> operator/(const Vec3<T1>& lhs, const T2& rhs);

    /**
    * Multiplies vector by a scalar (takes them in inverted order)
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs*rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec3<R> operator*(const T1& lhs, const Vec3<T2>& rhs);

    /**
    * Divides scalar (converted to a vector) by a vector
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Vec2(lhs, lhs)/rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec3<R> operator/(const T1& lhs, const Vec3<T2>& rhs);

    /**
    * Component-wise multiplies two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Component-wise lhs*rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec3<R> operator*(const Vec3<T1>& lhs, const Vec3<T2>& rhs);

    /**
    * Component-wise divides two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Component-wise lhs/rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec3<R> operator/(const Vec3<T1>& lhs, const Vec3<T2>& rhs);



    template <class T>
    constexpr Vec3<T>::Vec3(const T& xyz) noexcept(std::is_nothrow_copy_constructible<T>::value) :
        x(xyz),
        y(xyz),
        z(xyz)
    {
    }
    template <class T>
    constexpr Vec3<T>::Vec3(const T& x, const T& y, const T& z) noexcept(std::is_nothrow_copy_constructible<T>::value) :
        x(x),
        y(y),
        z(z)
    {
    }
    template <class T>
    constexpr const Vec3<T>& Vec3<T>::zero() noexcept
    {
        constexpr static Vec3<T> vec(T(0), T(0), T(0));
        return vec;
    }
    template <class T>
    constexpr const Vec3<T>& Vec3<T>::one() noexcept
    {
        constexpr static Vec3<T> vec(T(1), T(1), T(1));
        return vec;
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator+(const Vec3<T1>& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator-(const Vec3<T1>& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs.x - rhs.x,
            lhs.y - rhs.y,
            lhs.z - rhs.z
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator*(const Vec3<T1>& lhs, const T2& rhs)
    {
        return Vec3<R>(
            lhs.x * rhs,
            lhs.y * rhs,
            lhs.z * rhs
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator/(const Vec3<T1>& lhs, const T2& rhs)
    {
        return Vec3<R>(
            lhs.x / rhs,
            lhs.y / rhs,
            lhs.z / rhs
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator*(const T1& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs * rhs.x,
            lhs * rhs.y,
            lhs * rhs.z
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator/(const T1& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs / rhs.x,
            lhs / rhs.y,
            lhs / rhs.z
            );
    }

    template <class T1, class T2, class R>
    constexpr Vec3<R> operator*(const Vec3<T1>& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs.x * rhs.x,
            lhs.y * rhs.y,
            lhs.z * rhs.z
            );
    }

    template <class T1, class T2, class R>
    constexpr Vec3<R> operator/(const Vec3<T1>& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs.x / rhs.x,
            lhs.y / rhs.y,
            lhs.z / rhs.z
            );
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator*=(const T& rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator*=(const Vec3<T>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator/=(const T& rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator/=(const Vec3<T>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    template <class T>
    const T& Vec3<T>::operator[](int i) const
    {
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        default: return z;
        }
    }
    template <class T>
    T& Vec3<T>::operator[](int i)
    {
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        default: return z;
        }
    }

    template <class T>
    template <class T2>
    Vec3<T>::operator Vec3<T2>() const
    {
        return Vec3<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z));
    }

    template <class T>
    constexpr T Vec3<T>::length() const
    {
        using std::sqrt;

        return static_cast<T>(sqrt(lengthSquared()));
    }
    template <class T>
    constexpr T Vec3<T>::lengthSquared() const
    {
        return static_cast<T>((x * x) + (y * y) + (z * z));
    }
    template <class T>
    constexpr T Vec3<T>::distance(const Vec3<T>& other) const
    {
        return (other - *this).length();
    }
    template <class T>
    constexpr T Vec3<T>::distanceSquared(const Vec3<T>& other) const
    {
        return (other - *this).lengthSquared();
    }
    template <class T>
    void Vec3<T>::normalize() &
    {
        const T invMag = T(1) / length();
        x *= invMag;
        y *= invMag;
        z *= invMag;
    }
    template <class T>
    Vec3<T> Vec3<T>::normalized() const
    {
        Vec3<T> result(*this);
        result.normalize();
        return result;
    }
    template <class T>
    T Vec3<T>::dot(const Vec3<T>& rhs) const
    {
        return x*rhs.x + y*rhs.y + z*rhs.z;
    }
    template <class T>
    Vec3<T> Vec3<T>::cross(const Vec3<T>& rhs) const
    {
        return Vec3<T>(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
    }

    template <class T>
    constexpr const Vec3<T>& Vec3<T>::unitX() noexcept
    {
        constexpr static Vec3<T> vec(T(1), T(0), T(0));
        return vec;
    }
    template <class T>
    constexpr const Vec3<T>& Vec3<T>::unitY() noexcept
    {
        constexpr static Vec3<T> vec(T(0), T(1), T(0));
        return vec;
    }
    template <class T>
    constexpr const Vec3<T>& Vec3<T>::unitZ() noexcept
    {
        constexpr static Vec3<T> vec(T(0), T(0), T(1));
        return vec;
    }

    template <class T>
    constexpr bool operator==(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z) == std::tie(rhs.x, rhs.y, rhs.z);
    }
    template <class T>
    constexpr bool operator!=(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    constexpr bool operator<(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
    }
    template <class T>
    constexpr bool operator>(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    constexpr bool operator<=(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    constexpr bool operator>=(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return !(lhs < rhs);
    }
    template <class T>
    constexpr Vec3<T> operator-(const Vec3<T>& vector)
    {
        return Vec3<T>(-vector.x, -vector.y, -vector.z);
    }


    template <class T>
    constexpr bool almostZero(const Vec3<T>& val, const T& tolerance)
    {
        using std::abs;

        return abs(val.x) <= tolerance && abs(val.y) <= tolerance && abs(val.z) <= tolerance;
    }
    template <class T>
    constexpr bool almostEqual(const Vec3<T>& lhs, const Vec3<T>& rhs, const T& tolerance)
    {
        using std::abs;

        return abs(lhs.x - rhs.x) <= tolerance && abs(lhs.y - rhs.y) <= tolerance && abs(lhs.z - rhs.z) <= tolerance;
    }
    
    // expects lhs and rhs to be normalized
    template <class T>
    constexpr Vec3<T> slerp(const Vec3<T>& lhs, const Vec3<T>& rhs, const T& t)
    {
        using std::acos;
        using std::sin;
        using std::cos;

        static constexpr T eps = T(0.01);

        // without std:: (with using) can sometimes crash msvc's optimizer (?!)
        const T cosTheta = std::clamp(lhs.dot(rhs), T(-1.0), T(1.0));

        if (cosTheta > T(1) - eps)
        {
            return nlerp(lhs, rhs, t);
        }
        else
        {
            const Vec3<T> rel = (rhs - lhs*cosTheta).normalized();
            // { lhs, rel } now an orthonormal basis
            const T theta = acos(cosTheta) * t;

            return lhs*cos(theta) + rel*sin(theta);
        }
    }
    
    
    template <class FloatType, class IntType = int>
    Vec3<IntType> floorToInt(const Vec3<FloatType>& value)
    {
        return { floorToInt(value.x), floorToInt(value.y), floorToInt(value.z) };
    }
}
