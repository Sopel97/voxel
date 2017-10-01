#pragma once

#include "VectorUtil.h"
#include "Shapes/Vec2.h"
#include "Shapes/Vec3.h"
#include "Shapes/Vec4.h"
#include "Quat.h"
#include "Transform3.h"
#include "Shapes/Angle2.h"

namespace ls
{
    namespace detail
    {
        template <class...>
        struct matrix_align
        {
            static constexpr size_t value = 0;
        };

        template <>
        struct matrix_align<float>
        {
            static constexpr size_t value = 16;
        };
    }
    // TODO: determinant, inverse

    // row major layout
    template <class T, int R, int C>
    class alignas(detail::matrix_align<T>::value) Matrix
    {
        static_assert(std::is_arithmetic<T>::value, "T must be of arithmetic type");
        static_assert(R >= 2 && R <= 4 && C >= 2 && C <= 4, "Matrix must not be smaller than 2x2 nor bigger than 4x4.");

        using RowStorageType = T[C];
    public:
        using ValueType = T;
        static constexpr int numRows = R;
        static constexpr int numColumns = C;

        using ColVecType = typename util::VectorType<T, R>::type;
        using RowVecType = typename util::VectorType<T, C>::type;

        static const Matrix<T, R, C>& zero() noexcept;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == Cols>::type>
        static const Matrix<T, R, C>& identity() noexcept;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<(Rows == 3 && Cols == 3) || (Rows == 4 && Cols == 4)>::type>
        static Matrix<T, R, C> rotation(const Quat<T>& q) noexcept;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == 4 && Cols == 4>::type>
        static Matrix<T, R, C> rotation(const Quat<T>& q, const Vec3<T>& origin) noexcept;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<(Rows == 3 && Cols == 3) || (Rows == 4 && Cols == 4)>::type>
        static Matrix<T, R, C> scale(const T& s) noexcept;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<(Rows == 3 && Cols == 3) || (Rows == 4 && Cols == 4)>::type>
        static Matrix<T, R, C> scale(const Vec3<T>& s) noexcept;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == 4 && Cols == 4>::type>
        static Matrix<T, R, C> translation(const Vec3<T>& v) noexcept;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == 4 && Cols == 4>::type>
        static Matrix<T, R, C> transform(const Transform3<T>& t) noexcept;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == 4 && Cols == 4>::type>
        static Matrix<T, R, C> perspective(const Angle2<T>& fovY, const T& aspect, const T& nearZ, const T& farZ) noexcept;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == 4 && Cols == 4>::type>
        static Matrix<T, R, C> lookAt(const Vec3<T>& cameraPos, const Vec3<T>& cameraTarget, const Vec3<T>& upVector) noexcept;

        constexpr Matrix() noexcept = default;
        explicit Matrix(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value);

        constexpr Matrix(const Matrix<T, R, C>&) = default;
        constexpr Matrix(Matrix<T, R, C>&&) noexcept = default;

        Matrix<T, R, C>& operator=(const Matrix<T, R, C>&) = default;
        Matrix<T, R, C>& operator=(Matrix<T, R, C> &&) noexcept = default;

        constexpr const RowStorageType& operator[](int row) const;
        RowStorageType& operator[](int row);
        T* data();
        const T* data() const;

        Matrix<T, R, C>& operator+=(const Matrix<T, R, C>& rhs);
        Matrix<T, R, C>& operator+=(const T& rhs);
        Matrix<T, R, C>& operator-=(const Matrix<T, R, C>& rhs);
        Matrix<T, R, C>& operator-=(const T& rhs);
        Matrix<T, R, C>& operator*=(const T& rhs);
        Matrix<T, R, C>& operator/=(const T& rhs);

        T sum() const;
        T product() const;
        T mean() const;
        T min() const;
        T max() const;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == Cols, T>::type>
        T trace() const;

        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == Cols, T>::type>
        T determinant() const;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == Cols, T>::type>
        Matrix<T, R, C> inverse() const;

        template <class E = std::enable_if<R == C, T>>
        typename E::type diagonal() const;

        void fill(const T& value) &;

    private:

        RowStorageType m_values[R]; //as array of rows
    };

    //TODO: predefined types for every matrix from 2x2 to 4x4 and for types int, float, double

    using Matrix2x2D = Matrix<double, 2, 2>;
    using Matrix2x2F = Matrix<float, 2, 2>;
    using Matrix2x2I = Matrix<int, 2, 2>;

    using Matrix2x3D = Matrix<double, 2, 3>;
    using Matrix2x3F = Matrix<float, 2, 3>;
    using Matrix2x3I = Matrix<int, 2, 3>;

    using Matrix2x4D = Matrix<double, 2, 4>;
    using Matrix2x4F = Matrix<float, 2, 4>;
    using Matrix2x4I = Matrix<int, 2, 4>;

    using Matrix3x2D = Matrix<double, 3, 2>;
    using Matrix3x2F = Matrix<float, 3, 2>;
    using Matrix3x2I = Matrix<int, 3, 2>;

    using Matrix3x3D = Matrix<double, 3, 3>;
    using Matrix3x3F = Matrix<float, 3, 3>;
    using Matrix3x3I = Matrix<int, 3, 3>;

    using Matrix3x4D = Matrix<double, 3, 4>;
    using Matrix3x4F = Matrix<float, 3, 4>;
    using Matrix3x4I = Matrix<int, 3, 4>;

    using Matrix4x2D = Matrix<double, 4, 2>;
    using Matrix4x2F = Matrix<float, 4, 2>;
    using Matrix4x2I = Matrix<int, 4, 2>;

    using Matrix4x3D = Matrix<double, 4, 3>;
    using Matrix4x3F = Matrix<float, 4, 3>;
    using Matrix4x3I = Matrix<int, 4, 3>;

    using Matrix4x4D = Matrix<double, 4, 4>;
    using Matrix4x4F = Matrix<float, 4, 4>;
    using Matrix4x4I = Matrix<int, 4, 4>;


    template <class T, int R, int C>
    bool operator ==(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs)
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                if (lhs[r][c] != rhs[r][c]) return false;
            }
        }
        return true;
    }

    template <class T, int R, int C>
    bool operator !=(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class T, int R, int C>
    Matrix<T, R, C> operator+(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs)
    {
        Matrix<T, R, C> result(lhs);
        return (result += rhs);
    }
    template <class T, int R, int C>
    Matrix<T, R, C> operator+(const Matrix<T, R, C>& lhs, const T& rhs)
    {
        Matrix<T, R, C> result(lhs);
        return (result += rhs);
    }
    template <class T, int R, int C>
    Matrix<T, R, C> operator-(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs)
    {
        Matrix<T, R, C> result(lhs);
        return (result -= rhs);
    }
    template <class T, int R, int C>
    Matrix<T, R, C> operator-(const Matrix<T, R, C>& lhs, const T& rhs)
    {
        Matrix<T, R, C> result(lhs);
        return (result -= rhs);
    }
    template <class T, int R, int C>
    Matrix<T, R, C> operator-(const Matrix<T, R, C>& m)
    {
        Matrix<T, R, C> result;
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                result[r][c] = -m[r][c];
            }
        }
        return result;
    }
    template <class T, int R, int C, int C2>
    Matrix<T, R, C2> operator*(const Matrix<T, R, C>& lhs, const Matrix<T, C2, C>& rhs)
    {
        Matrix<T, R, C> result;
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                T resultElement = T(0);
                for (int i = 0; i < C2; ++i)
                {
                    resultElement += lhs[r][i] * rhs[i][c];
                }
                result[r][c] = resultElement;
            }
        }
        return result;
    }
    template <class T, int R, int C>
    typename Matrix<T, R, C>::ColVecType operator*(const Matrix<T, R, C>& lhs, const typename Matrix<T, R, C>::RowVecType& rhs)
    {
        typename Matrix<T, R, C>::ColVecType result;
        for (int r = 0; r < R; ++r)
        {
            T resultElement = 0;
            for (int i = 0; i < C; ++i)
            {
                resultElement += lhs[r][i] * rhs[i];
            }
            result[r] = resultElement;
        }
        return result;
    }
    template <class T>
    Vec3<T> operator*(const Matrix<T, 4, 4>& lhs, const Vec3<T>& rhs)
    {
        Vec3<T> result;
        for (int r = 0; r < 3; ++r)
        {
            T resultElement = 0;
            for (int i = 0; i < 3; ++i)
            {
                resultElement += lhs[r][i] * rhs[i];
            }
            resultElement += lhs[r][i];
            result[r] = resultElement;
        }
        return result;
    }
    template <class T, int R, int C>
    Matrix<T, R, C> operator*(const Matrix<T, R, C>& lhs, const T& rhs)
    {
        Matrix<T, R, C> result(lhs);
        return (result *= rhs);
    }
    template <class T, int R, int C>
    Matrix<T, R, C> operator/(const Matrix<T, R, C>& lhs, const T& rhs)
    {
        Matrix<T, R, C> result(lhs);
        return (result /= rhs);
    }

    template <class T, int R, int C>
    const Matrix<T, R, C>& Matrix<T, R, C>::zero() noexcept
    {
        static const Matrix<T, R, C> matrix(T(0));
        return matrix;
    }
    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    const Matrix<T, R, C>& Matrix<T, R, C>::identity() noexcept
    {
        static const Matrix<T, R, C> matrix = []()
        {
            Matrix<T, R, C> matrix(T(0));

            for (int i = 0; i < R; ++i)
                matrix.m_values[i][i] = T(1);

            return matrix;
        }();

        return matrix;
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    Matrix<T, R, C> Matrix<T, R, C>::rotation(const Quat<T>& q) noexcept
    {
        static constexpr bool is4 = R == 4 && C == 4;

        Matrix<T, R, C> result;

        const T x2 = q.x*q.x;
        const T y2 = q.y*q.y;
        const T z2 = q.z*q.z;

        // row 0
        result[0][0] = T(1) - T(2) * (y2 + z2);
        result[0][1] = T(2) * (q.x*q.y - q.z*q.w);
        result[0][2] = T(2) * (q.x*q.z + q.y*q.w);
        if constexpr(is4)
            result[0][3] = T(0);

        // row 1
        result[1][0] = T(2) * (q.x*q.y + q.z*q.w);
        result[1][1] = T(1) - T(2) * (x2 + z2);
        result[1][2] = T(2) * (q.y*q.z - q.x*q.w);
        if constexpr(is4)
            result[1][3] = T(0);

        // row 2
        result[2][0] = T(2) * (q.x*q.z - q.y*q.w);
        result[2][1] = T(2) * (q.y*q.z + q.x*q.w);
        result[2][2] = T(1) - T(2) * (x2 + y2);
        if constexpr(is4)
            result[2][3] = T(0);

        // row 3
        if constexpr(is4)
        {
            result[3][0] = T(0);
            result[3][1] = T(0);
            result[3][2] = T(0);
            result[3][3] = T(1);
        }

        return result;
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    Matrix<T, R, C> Matrix<T, R, C>::rotation(const Quat<T>& q, const Vec3<T>& origin) noexcept
    {
        // TODO: optimize
        return translation(origin) * rotation(q) * translation(-origin);
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    Matrix<T, R, C> Matrix<T, R, C>::scale(const T& s) noexcept
    {
        return Matrix<T, R, C>::scale(Vec3<T>(s, s, s));
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    Matrix<T, R, C> Matrix<T, R, C>::scale(const Vec3<T>& s) noexcept
    {
        static constexpr bool is4 = R == 4 && C == 4;

        Matrix<T, R, C> result;

        const T x2 = x*x;
        const T y2 = y*y;
        const T z2 = z*z;

        // row 0
        result[0][0] = s.x;
        result[0][1] = T(0);
        result[0][2] = T(0);
        if constexpr(is4)
            result[0][3] = T(0);

        // row 1
        result[1][0] = T(0);
        result[1][1] = s.y;
        result[1][2] = T(0);
        if constexpr(is4)
            result[1][3] = T(0);

        // row 2
        result[2][0] = T(0);
        result[2][1] = T(0);
        result[2][2] = s.z;
        if constexpr(is4)
            result[2][3] = T(0);

        // row 3
        if constexpr(is4)
        {
            result[3][0] = T(0);
            result[3][1] = T(0);
            result[3][2] = T(0);
            result[3][3] = T(1);
        }

        return result;
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    Matrix<T, R, C> Matrix<T, R, C>::translation(const Vec3<T>& v) noexcept
    {
        Matrix<T, R, C> result;

        // row 0
        result[0][0] = T(1);
        result[0][1] = T(0);
        result[0][2] = T(0);
        result[0][3] = v.x;

        // row 1
        result[1][0] = T(0);
        result[1][1] = T(1);
        result[1][2] = T(0);
        result[1][3] = v.y;

        // row 2
        result[2][0] = T(0);
        result[2][1] = T(0);
        result[2][2] = T(1);
        result[2][3] = v.z;

        // row 3
        result[3][0] = T(0);
        result[3][1] = T(0);
        result[3][2] = T(0);
        result[3][3] = T(1);

        return result;
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    Matrix<T, R, C> Matrix<T, R, C>::transform(const Transform3<T>& t) noexcept
    {
        Matrix<T, R, C> result;

        const auto& rotation = t.rotation();
        const auto& translation = t.translation();
        const auto& scale = t.scale();

        result[0][3] = translation.x;
        result[1][3] = translation.y;
        result[2][3] = translation.z;

        const T x2 = rotation.x + rotation.x;
        const T y2 = rotation.y + rotation.y;
        const T z2 = rotation.z + rotation.z;
        {
            const T xx2 = rotation.x * x2;
            const T yy2 = rotation.y * y2;
            const T zz2 = rotation.z * z2;

            result[0][0] = (T(1) - (yy2 + zz2)) * scale.x;
            result[1][1] = (T(1) - (xx2 + zz2)) * scale.y;
            result[2][2] = (T(1) - (xx2 + yy2)) * scale.z;
        }
        {
            const T yz2 = rotation.y * z2;
            const T wx2 = rotation.w * x2;

            result[1][2] = (yz2 - wx2) * scale.z;
            result[2][1] = (yz2 + wx2) * scale.y;
        }
        {
            const T xy2 = rotation.x * y2;
            const T wz2 = rotation.w * z2;

            result[0][1] = (xy2 - wz2) * scale.y;
            result[1][0] = (xy2 + wz2) * scale.x;
        }
        {
            const T xz2 = rotation.x * z2;
            const T wy2 = rotation.w * y2;

            result[0][2] = (xz2 + wy2) * scale.z;
            result[2][0] = (xz2 - wy2) * scale.x;
        }

        result[3][0] = 0.0f;
        result[3][1] = 0.0f;
        result[3][2] = 0.0f;
        result[3][3] = 1.0f;

        return result;
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    Matrix<T, R, C> Matrix<T, R, C>::perspective(const Angle2<T>& fovY, const T& aspect, const T& nearZ, const T& farZ) noexcept
    {
        Matrix<T, 4, 4> result(T(0));

        const T tanHalfFovY = (fovY / T(2)).tan();

        result[0][0] = T(1) / (aspect * tanHalfFovY);
        result[1][1] = T(1) / (tanHalfFovY);
        result[2][2] = -(farZ + nearZ) / (farZ - nearZ);
        result[2][3] = (T(-2) * farZ * nearZ) / (farZ - nearZ);
        result[3][2] = T(-1);

        return result;
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    static Matrix<T, R, C> Matrix<T, R, C>::lookAt(const Vec3<T>& cameraPos, const Vec3<T>& cameraTarget, const Vec3<T>& upVector) noexcept
    {
        const Vec3<T> fwd = (cameraTarget - cameraPos).normalized();
        const Vec3<T> side = fwd.cross(upVector).normalized();
        const Vec3<T> up = side.cross(fwd);

        Matrix<T, 4, 4> result;

        result[0][0] = side.x;
        result[0][1] = side.y;
        result[0][2] = side.z;
        result[0][3] = -side.dot(cameraPos);

        result[1][0] = up.x;
        result[1][1] = up.y;
        result[1][2] = up.z;
        result[1][3] = -up.dot(cameraPos);

        result[2][0] = -fwd.x;
        result[2][1] = -fwd.y;
        result[2][2] = -fwd.z;
        result[2][3] = fwd.dot(cameraPos);

        result[3][0] = T(0);
        result[3][1] = T(0);
        result[3][2] = T(0);
        result[3][3] = T(1);

        return result;
    }

    template <class T, int R, int C>
    Matrix<T, R, C>::Matrix(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value)
    {
        fill(value);
    }

    template <class T, int R, int C>
    constexpr const typename Matrix<T, R, C>::RowStorageType& Matrix<T, R, C>::operator[](int row) const
    {
        return m_values[row];
    }
    template <class T, int R, int C>
    typename Matrix<T, R, C>::RowStorageType& Matrix<T, R, C>::operator[](int row)
    {
        return m_values[row];
    }
    template <class T, int R, int C>
    T* Matrix<T, R, C>::data()
    {
        return &(m_values[0][0]);
    }
    template <class T, int R, int C>
    const T* Matrix<T, R, C>::data() const
    {
        return &(m_values[0][0]);
    }

    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator+=(const Matrix<T, R, C>& rhs)
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                m_values[r][c] += rhs.m_values[r][c];
            }
        }
        return *this;
    }
    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator+=(const T& rhs)
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                m_values[r][c] += rhs;
            }
        }
        return *this;
    }
    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator-=(const Matrix<T, R, C>& rhs)
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                m_values[r][c] -= rhs.m_values[r][c];
            }
        }
        return *this;
    }
    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator-=(const T& rhs)
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                m_values[r][c] -= rhs;
            }
        }
        return *this;
    }
    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator*=(const T& rhs)
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                m_values[r][c] *= rhs;
            }
        }
        return *this;

    }
    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator/=(const T& rhs)
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                m_values[r][c] /= rhs;
            }
        }
        return *this;
    }

    template <class T, int R, int C>
    T Matrix<T, R, C>::sum() const
    {
        T sumOfElements = 0;
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                sumOfElements += m_values[r][c];
            }
        }
        return sumOfElements;
    }
    template <class T, int R, int C>
    T Matrix<T, R, C>::product() const
    {
        T productOfElements = 1;
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                productOfElements *= m_values[r][c];
            }
        }
        return productOfElements;
    }
    template <class T, int R, int C>
    T Matrix<T, R, C>::mean() const
    {
        return sum() / (R * C);
    }
    template <class T, int R, int C>
    T Matrix<T, R, C>::min() const
    {
        T minElement = m_values[0][0];
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                if (m_values[r][c] < minElement) minElement = m_values[r][c];
            }
        }
        return minElement;
    }
    template <class T, int R, int C>
    T Matrix<T, R, C>::max() const
    {
        T maxElement = m_values[0][0];
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                if (m_values[r][c] > maxElement) maxElement = m_values[r][c];
            }
        }
        return maxElement;
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    T Matrix<T, R, C>::trace() const
    {
        T sumOfDiagonal = 0;
        for (int i = 0; i < R; ++i) sumOfDiagonal += m_values[i][i];
        return sumOfDiagonal;
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    T Matrix<T, R, C>::determinant() const
    {
        if constexpr(R == 2)
        {
            return m_values[0][0] * m_values[1][1] - m_values[0][1] * m_values[1][0];
        }
        else if constexpr (R == 3)
        {
            return
                +m_values[0][0] * (m_values[1][1] * m_values[2][2] - m_values[1][2] * m_values[2][1])
                + m_values[0][1] * (m_values[1][2] * m_values[2][0] - m_values[1][0] * m_values[2][2])
                + m_values[0][2] * (m_values[1][0] * m_values[2][1] - m_values[1][1] * m_values[2][0]);
        }
        else // R == 4
        {
            const T sub0 = m_values[2][2] * m_values[3][3] - m_values[3][2] * m_values[2][3];
            const T sub1 = m_values[2][1] * m_values[3][3] - m_values[3][1] * m_values[2][3];
            const T sub2 = m_values[2][1] * m_values[3][2] - m_values[3][1] * m_values[2][2];
            const T sub3 = m_values[2][0] * m_values[3][3] - m_values[3][0] * m_values[2][3];
            const T sub4 = m_values[2][0] * m_values[3][2] - m_values[3][0] * m_values[2][2];
            const T sub5 = m_values[2][0] * m_values[3][1] - m_values[3][0] * m_values[2][1];

            const T cof0 = m_values[1][1] * sub0 - m_values[1][2] * sub1 + m_values[1][3] * sub2;
            const T cof1 = m_values[1][2] * sub3 - m_values[1][0] * sub0 - m_values[1][3] * sub4;
            const T cof2 = m_values[1][0] * sub1 - m_values[1][1] * sub3 + m_values[1][3] * sub5;
            const T cof3 = m_values[1][1] * sub4 - m_values[1][0] * sub2 - m_values[1][2] * sub5;


            return
                m_values[0][0] * cof0 + m_values[0][1] * cof1 +
                m_values[0][2] * cof2 + m_values[0][3] * cof3;
        }
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    Matrix<T, R, C> Matrix<T, R, C>::inverse() const
    {
        if constexpr(R == 2)
        {
            Matrix<T, 2, 2> inv;
            inv[0][0] = m_values[1][1];
            inv[0][1] = -m_values[0][1];
            inv[1][0] = -m_values[1][0];
            inv[1][1] = m_values[0][0];
            inv /= determinant();

            return inv;
        }
        else if constexpr (R == 3)
        {

            Matrix<T, 3, 3> inv;
            inv[0][0] = +(m_values[1][1] * m_values[2][2] - m_values[1][2] * m_values[2][1]);
            inv[0][1] = -(m_values[0][1] * m_values[2][2] - m_values[0][2] * m_values[2][1]);
            inv[0][2] = +(m_values[0][1] * m_values[1][2] - m_values[0][2] * m_values[1][1]);
            inv[1][0] = -(m_values[1][0] * m_values[2][2] - m_values[1][2] * m_values[2][0]);
            inv[1][1] = +(m_values[0][0] * m_values[2][2] - m_values[0][2] * m_values[2][0]);
            inv[1][2] = -(m_values[0][0] * m_values[1][2] - m_values[0][2] * m_values[1][0]);
            inv[2][0] = +(m_values[1][0] * m_values[2][1] - m_values[1][1] * m_values[2][0]);
            inv[2][1] = -(m_values[0][0] * m_values[2][1] - m_values[0][1] * m_values[2][0]);
            inv[2][2] = +(m_values[0][0] * m_values[1][1] - m_values[0][1] * m_values[1][0]);
            inv /= determinant();

            return inv;
        }
        else if constexpr (R == 4)
        {
            const T sub00 = m_values[2][2] * m_values[3][3] - m_values[2][3] * m_values[3][2];
            const T sub01 = m_values[1][2] * m_values[3][3] - m_values[1][3] * m_values[3][2];
            const T sub02 = m_values[1][2] * m_values[2][3] - m_values[1][3] * m_values[2][2];
            const T sub03 = m_values[0][2] * m_values[3][3] - m_values[0][3] * m_values[3][2];
            const T sub04 = m_values[0][2] * m_values[2][3] - m_values[0][3] * m_values[2][2];
            const T sub05 = m_values[0][2] * m_values[1][3] - m_values[0][3] * m_values[1][2];
            const T sub06 = m_values[2][1] * m_values[3][3] - m_values[2][3] * m_values[3][1];
            const T sub07 = m_values[1][1] * m_values[3][3] - m_values[1][3] * m_values[3][1];
            const T sub08 = m_values[1][1] * m_values[2][3] - m_values[1][3] * m_values[2][1];
            const T sub09 = m_values[0][1] * m_values[3][3] - m_values[0][3] * m_values[3][1];
            const T sub10 = m_values[0][1] * m_values[2][3] - m_values[0][3] * m_values[2][1];
            const T sub11 = m_values[1][1] * m_values[3][3] - m_values[1][3] * m_values[3][1];
            const T sub12 = m_values[0][1] * m_values[1][3] - m_values[0][3] * m_values[1][1];
            const T sub13 = m_values[2][1] * m_values[3][2] - m_values[2][2] * m_values[3][1];
            const T sub14 = m_values[1][1] * m_values[3][2] - m_values[1][2] * m_values[3][1];
            const T sub15 = m_values[1][1] * m_values[2][2] - m_values[1][2] * m_values[2][1];
            const T sub16 = m_values[0][1] * m_values[3][2] - m_values[0][2] * m_values[3][1];
            const T sub17 = m_values[0][1] * m_values[2][2] - m_values[0][2] * m_values[2][1];
            const T sub18 = m_values[0][1] * m_values[1][2] - m_values[0][2] * m_values[1][1];

            Matrix<T, 4, 4> inv;
            inv[0][0] = +(m_values[1][1] * sub00 - m_values[2][1] * sub01 + m_values[3][1] * sub02);
            inv[0][1] = -(m_values[0][1] * sub00 - m_values[2][1] * sub03 + m_values[3][1] * sub04);
            inv[0][2] = +(m_values[0][1] * sub01 - m_values[1][1] * sub03 + m_values[3][1] * sub05);
            inv[0][3] = -(m_values[0][1] * sub02 - m_values[1][1] * sub04 + m_values[2][1] * sub05);

            inv[1][0] = -(m_values[1][0] * sub00 - m_values[2][0] * sub01 + m_values[3][0] * sub02);
            inv[1][1] = +(m_values[0][0] * sub00 - m_values[2][0] * sub03 + m_values[3][0] * sub04);
            inv[1][2] = -(m_values[0][0] * sub01 - m_values[1][0] * sub03 + m_values[3][0] * sub05);
            inv[1][3] = +(m_values[0][0] * sub02 - m_values[1][0] * sub04 + m_values[2][0] * sub05);

            inv[2][0] = +(m_values[1][0] * sub06 - m_values[2][0] * sub07 + m_values[3][0] * sub08);
            inv[2][1] = -(m_values[0][0] * sub06 - m_values[2][0] * sub09 + m_values[3][0] * sub10);
            inv[2][2] = +(m_values[0][0] * sub11 - m_values[1][0] * sub09 + m_values[3][0] * sub12);
            inv[2][3] = -(m_values[0][0] * sub08 - m_values[1][0] * sub10 + m_values[2][0] * sub12);

            inv[3][0] = -(m_values[1][0] * sub13 - m_values[2][0] * sub14 + m_values[3][0] * sub15);
            inv[3][1] = +(m_values[0][0] * sub13 - m_values[2][0] * sub16 + m_values[3][0] * sub17);
            inv[3][2] = -(m_values[0][0] * sub14 - m_values[1][0] * sub16 + m_values[3][0] * sub18);
            inv[3][3] = +(m_values[0][0] * sub15 - m_values[1][0] * sub17 + m_values[2][0] * sub18);

            const T det =
                +m_values[0][0] * inv[0][0]
                + m_values[1][0] * inv[0][1]
                + m_values[2][0] * inv[0][2]
                + m_values[3][0] * inv[0][3];

            inv /= det;

            return inv;
        }
    }

    template <class T, int R, int C>
    template <class E>
    typename E::type Matrix<T, R, C>::diagonal() const
    {
        Matrix<T, R, C>::RowVecType diagonalVector;
        for (int i = 0; i < R; ++i) diagonalVector[i] = m_values[i][i];
        return diagonalVector;
    }

    template <class T, int R, int C>
    void Matrix<T, R, C>::fill(const T& value) &
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                m_values[r][c] = value;
            }
        }
    }
}
