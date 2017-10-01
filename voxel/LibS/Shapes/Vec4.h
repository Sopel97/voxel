#pragma once

#include <type_traits>

namespace ls
{
    namespace detail
    {
        template <class...>
        struct vec4_align
        {
            static constexpr size_t value = 0;
        };

        template <>
        struct vec4_align<float>
        {
            static constexpr size_t value = 16;
        };
    }

    /**
    * Class representing 4 dimensional vector
    * If the underlying type is float then it has specified align of 16 bytes
    */
    template <class T>
    class alignas(detail::vec4_align<T>::value) Vec4
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;
        using VectorType = Vec4<T>;

        T x, y, z, w;

        /**
        * Default constructor
        *
        * Does not initialize the components
        */
        constexpr Vec4() noexcept = default;

        /**
        * Constructor
        *
        * @param xyzw Value to be assigned to all components
        */
        constexpr explicit Vec4(const T& xyzw) noexcept(std::is_nothrow_copy_constructible<T>::value);

        /**
        * Constructor
        *
        * @param x Value to be assigned to the x component
        * @param y Value to be assigned to the y component
        * @param z Value to be assigned to the z component
        * @param w Value to be assigned to the w component
        */
        constexpr Vec4(const T& x, const T& y, const T& z, const T& w) noexcept(std::is_nothrow_copy_constructible<T>::value);

        /**
        * Named constructor that returns a zero vector
        *
        * @return A zero vector
        */
        constexpr static const Vec4<T>& zero() noexcept;

        /**
        * Named constructor that returns a (1, 1, 1, 1) vector
        *
        * @return A (1, 1, 1, 1) vector
        */
        constexpr static const Vec4<T>& one() noexcept;

        /**
        * Default copy constructor
        */
        constexpr Vec4(const Vec4<T>&) = default;

        /**
        * Default move constructor
        */
        constexpr Vec4(Vec4<T>&&) noexcept = default;

        /**
        * Default copy assign
        */
        Vec4<T>& operator=(const Vec4<T>&) = default;

        /**
        * Default move assign
        */
        Vec4<T>& operator=(Vec4<T>&&) noexcept = default;

        /**
        * Adds a vector to this
        *
        * @param rhs Vector to be added to this
        * @return reference to this after operation
        */
        Vec4<T>& operator+=(const Vec4<T>& rhs);

        /**
        * Subtracts a vector from this
        *
        * @param rhs Vector to be subtracted from this
        * @return reference to this after operation
        */
        Vec4<T>& operator-=(const Vec4<T>& rhs);

        /**
        * Multiplies this vector in place by a scalar
        *
        * @param rhs Scalar to multiply by
        * @return reference to this after operation
        */
        Vec4<T>& operator*=(const T& rhs);

        /**
        * Multiplies this vector in place by a another one
        * This is component-wise multiplication
        *
        * @param rhs Vector to multiply by
        * @return reference to this after operation
        */
        Vec4<T>& operator*=(const Vec4<T>& rhs);

        /**
        * Divides this vector in place by a scalar
        *
        * @param rhs Scalar to divide by
        * @return reference to this after operation
        */
        Vec4<T>& operator/=(const T& rhs);

        /**
        * Divides this vector in place by a another one
        * This is component-wise division
        *
        * @param rhs Vector to divide by
        * @return reference to this after operation
        */
        Vec4<T>& operator/=(const Vec4<T>& rhs);

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
        * @return Const refence to the component
        */
        T& operator[](int i);

        /**
        * Converts to a Vec4 with different underlying type
        */
        template <class T2>
        explicit operator Vec4<T2>() const;

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
        constexpr T distance(const Vec4<T>& other) const;

        /**
        * Computes the squared distance between to vectors
        *
        * @param other Vector to compute squared distance to
        * @return Squared distance between vectors
        */
        constexpr T distanceSquared(const Vec4<T>& other) const;

        /**
        * Normalizes this vector in place (divides itself by the length)
        */
        void normalize() &;

        /**
        * Returns a normalized copy of this
        *
        * @return A normalized copy of this
        */
        Vec4<T> normalized() const;

        /**
        * Performs a dot product on this and a given vector
        *
        * @param rhs Vector to dot with
        * @return The dot product of the two vectors
        */
        T dot(const Vec4<T>& rhs) const;

        /**
        * Named constructor returning unit vector on OX axis
        *
        * @return Vector (1, 0, 0, 0)
        */
        static constexpr const Vec4<T>& unitX() noexcept;

        /**
        * Named constructor returning unit vector on OY axis
        *
        * @return Vector (0, 1, 0, 0)
        */
        static constexpr const Vec4<T>& unitY() noexcept;

        /**
        * Named constructor returning unit vector on OZ axis
        *
        * @return Vector (0, 0, 1, 0)
        */
        static constexpr const Vec4<T>& unitZ() noexcept;

        /**
        * Named constructor returning unit vector on OW axis
        *
        * @return Vector (0, 0, 0, 1)
        */
        static constexpr const Vec4<T>& unitW() noexcept;
    };

    using Vec4D = Vec4<double>;
    using Vec4F = Vec4<float>;
    using Vec4I = Vec4<int>;

    /**
    * Compares vectors for equality
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether vector are exactly equal
    */
    template <class T>
    constexpr bool operator==(const Vec4<T>& lhs, const Vec4<T>& rhs);

    /**
    * Compares vectors for inequality
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether vector are not exactly equal
    */
    template <class T>
    constexpr bool operator!=(const Vec4<T>& lhs, const Vec4<T>& rhs);

    /**
    * Orders vectors (total order)
    * First compares x, if it is equal then compares y, and so on
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is less than rhs
    */
    template <class T>
    constexpr bool operator<(const Vec4<T>& lhs, const Vec4<T>& rhs);

    /**
    * Orders vectors (total order)
    * First compares x, if it is equal then compares y, and so on
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is greater than rhs
    */
    template <class T>
    constexpr bool operator>(const Vec4<T>& lhs, const Vec4<T>& rhs);

    /**
    * Orders vectors
    * First compares x, if it is equal then compares y, and so on
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is less or equal than rhs
    */
    template <class T>
    constexpr bool operator<=(const Vec4<T>& lhs, const Vec4<T>& rhs);

    /**
    * Orders vectors
    * First compares x, if it is equal then compares y, and so on
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is greater or equal than rhs
    */
    template <class T>
    constexpr bool operator>=(const Vec4<T>& lhs, const Vec4<T>& rhs);

    /**
    * Returns negation of a vector
    *
    * @param vector Vector to negate
    * @return Negated copy of a vector
    */
    template <class T>
    constexpr Vec4<T> operator-(const Vec4<T>& vector);

    /**
    * Adds two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Sum of the two vectors
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() + std::declval<T2>())>
    constexpr Vec4<R> operator+(const Vec4<T1>& lhs, const Vec4<T2>& rhs);

    /**
    * Subtracts two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs - rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() - std::declval<T2>())>
    constexpr Vec4<R> operator-(const Vec4<T1>& lhs, const Vec4<T2>& rhs);

    /**
    * Multiplies vector by scalar
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs*rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec4<R> operator*(const Vec4<T1>& lhs, const T2& rhs);

    /**
    * Divides vector by scalar
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs/rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec4<R> operator/(const Vec4<T1>& lhs, const T2& rhs);

    /**
    * Multiplies vector by a scalar (takes them in inverted order)
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs*rhs
    */
    template <class T1, class T2, class R = std::enable_if_t<std::is_arithmetic_v<T1>, decltype(std::declval<T1>() * std::declval<T2>())>>
    constexpr Vec4<R> operator*(const T1& lhs, const Vec4<T2>& rhs);

    /**
    * Divides scalar (converted to a vector) by a vector
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Vec2(lhs, lhs)/rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec4<R> operator/(const T1& lhs, const Vec4<T2>& rhs);

    /**
    * Component-wise multiplies two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Component-wise lhs*rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec4<R> operator*(const Vec4<T1>& lhs, const Vec4<T2>& rhs);

    /**
    * Component-wise divides two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Component-wise lhs/rhs
    */
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec4<R> operator/(const Vec4<T1>& lhs, const Vec4<T2>& rhs);



    template <class T>
    constexpr Vec4<T>::Vec4(const T& _xyzw) noexcept(std::is_nothrow_copy_constructible<T>::value) :
        x(_xyzw),
        y(_xyzw),
        z(_xyzw),
        w(_xyzw)
    {
    }
    template <class T>
    constexpr Vec4<T>::Vec4(const T& _x, const T& _y, const T& _z, const T& _w) noexcept(std::is_nothrow_copy_constructible<T>::value) :
        x(_x),
        y(_y),
        z(_z),
        w(_w)
    {
    }
    template <class T>
    constexpr const Vec4<T>& Vec4<T>::zero() noexcept
    {
        constexpr static Vec4<T> vec(T(0), T(0), T(0), T(0));
        return vec;
    }
    template <class T>
    constexpr const Vec4<T>& Vec4<T>::one() noexcept
    {
        constexpr static Vec4<T> vec(T(1), T(1), T(1), T(1));
        return vec;
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator+(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z,
            lhs.w + rhs.w
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator-(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x - rhs.x,
            lhs.y - rhs.y,
            lhs.z - rhs.z,
            lhs.w - rhs.w
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator*(const Vec4<T1>& lhs, const T2& rhs)
    {
        return Vec4<R>(
            lhs.x * rhs,
            lhs.y * rhs,
            lhs.z * rhs,
            lhs.w * rhs
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator/(const Vec4<T1>& lhs, const T2& rhs)
    {
        return Vec4<R>(
            lhs.x / rhs,
            lhs.y / rhs,
            lhs.z / rhs,
            lhs.w / rhs
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator*(const T1& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs * rhs.x,
            lhs * rhs.y,
            lhs * rhs.z,
            lhs * rhs.w
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator/(const T1& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs / rhs.x,
            lhs / rhs.y,
            lhs / rhs.z,
            lhs / rhs.w
            );
    }

    template <class T1, class T2, class R>
    constexpr Vec4<R> operator*(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x * rhs.x,
            lhs.y * rhs.y,
            lhs.z * rhs.z,
            lhs.w * rhs.w
            );
    }

    template <class T1, class T2, class R>
    constexpr Vec4<R> operator/(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x / rhs.x,
            lhs.y / rhs.y,
            lhs.z / rhs.z,
            lhs.w / rhs.w
            );
    }

    template <class T>
    Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }
    template <class T>
    Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }
    template <class T>
    Vec4<T>& Vec4<T>::operator*=(const T& rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }
    template <class T>
    Vec4<T>& Vec4<T>::operator*=(const Vec4<T>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }
    template <class T>
    Vec4<T>& Vec4<T>::operator/=(const T& rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }
    template <class T>
    Vec4<T>& Vec4<T>::operator/=(const Vec4<T>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        w /= rhs.w;
        return *this;
    }

    template <class T>
    const T& Vec4<T>::operator[](int i) const
    {
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: return w;
        }
    }
    template <class T>
    T& Vec4<T>::operator[](int i)
    {
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: return w;
        }
    }

    template <class T>
    template <class T2>
    Vec4<T>::operator Vec4<T2>() const
    {
        return Vec4<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z), static_cast<T2>(w));
    }

    template <class T>
    constexpr T Vec4<T>::length() const
    {
        using std::sqrt;

        return static_cast<T>(sqrt(lengthSquared()));
    }
    template <class T>
    constexpr T Vec4<T>::lengthSquared() const
    {
        return static_cast<T>((x*x) + (y*y) + (z*z) + (w*w));
    }
    template <class T>
    constexpr T Vec4<T>::distance(const Vec4<T>& other) const
    {
        return (other - *this).length();
    }
    template <class T>
    constexpr T Vec4<T>::distanceSquared(const Vec4<T>& other) const
    {
        return (other - *this).lengthSquared();
    }
    template <class T>
    void Vec4<T>::normalize() &
    {
        const T invMag = T(1) / length();
        x *= invMag;
        y *= invMag;
        z *= invMag;
        w *= invMag;
    }
    template <class T>
    Vec4<T> Vec4<T>::normalized() const
    {
        Vec4<T> result(*this);
        result.normalize();
        return result;
    }
    template <class T>
    T Vec4<T>::dot(const Vec4<T>& rhs) const
    {
        return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;
    }

    template <class T>
    constexpr const Vec4<T>& Vec4<T>::unitX() noexcept
    {
        constexpr static Vec4<T> vec(T(1), T(0), T(0), T(0));
        return vec;
    }
    template <class T>
    constexpr const Vec4<T>& Vec4<T>::unitY() noexcept
    {
        constexpr static Vec4<T> vec(T(0), T(1), T(0), T(0));
        return vec;
    }
    template <class T>
    constexpr const Vec4<T>& Vec4<T>::unitZ() noexcept
    {
        constexpr static Vec4<T> vec(T(0), T(0), T(1), T(0));
        return vec;
    }
    template <class T>
    constexpr const Vec4<T>& Vec4<T>::unitW() noexcept
    {
        constexpr static Vec4<T> vec(T(0), T(0), T(0), T(1));
        return vec;
    }

    template <class T>
    constexpr bool operator==(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z, lhs.w) == std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
    }
    template <class T>
    constexpr bool operator!=(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    constexpr bool operator<(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z, lhs.w) < std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
    }
    template <class T>
    constexpr bool operator>(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    constexpr bool operator<=(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    constexpr bool operator>=(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return !(lhs < rhs);
    }
    template <class T>
    constexpr Vec4<T> operator-(const Vec4<T>& vector)
    {
        return Vec4<T>(-vector.x, -vector.y, -vector.z, -vector.w);
    }


    template <class T>
    constexpr bool almostZero(const Vec4<T>& val, const T& tolerance)
    {
        using std::abs;

        return abs(val.x) <= tolerance && abs(val.y) <= tolerance && abs(val.z) <= tolerance && abs(val.w) <= tolerance;
    }
    template <class T>
    constexpr bool almostEqual(const Vec4<T>& lhs, const Vec4<T>& rhs, const T& tolerance)
    {
        using std::abs;

        return abs(lhs.x - rhs.x) <= tolerance && abs(lhs.y - rhs.y) <= tolerance && abs(lhs.z - rhs.z) <= tolerance && abs(lhs.w - rhs.w) <= tolerance;
    }
    template <class FloatType, class IntType = int>
    Vec4<IntType> floorToInt(const Vec4<FloatType>& value)
    {
        return { floorToInt(value.x), floorToInt(value.y), floorToInt(value.z), floorToInt(value.w) };
    }
}
