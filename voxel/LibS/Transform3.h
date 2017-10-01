#pragma once

#include "Quat.h"
#include "Shapes/Vec3.h"

namespace ls
{
    // QST(A) = Q(A)*S(A)*P*-Q(A) + T(A)
    template <class T>
    class Transform3
    {
    public:
        static Transform3<T> rotation(const Quat<T>& q)
        {
            return Transform3<T>(q, Vec3<T>::one(), Vec3<T>::zero());
        }
        static Transform3<T> rotation(const Quat<T>& q, const Vec3<T>& origin)
        {
            return translation(origin) * rotation(q) * translation(-origin);
        }
        static Transform3<T> scale(const T& s)
        {
            return Transform3<T>(Quat<T>::identity(), Vec3<T>(s, s, s), Vec3<T>::zero());
        }
        static Transform3<T> scale(const Vec3<T>& s)
        {
            return Transform3<T>(Quat<T>::identity(), s, Vec3<T>::zero());
        }
        static Transform3<T> translation(const Vec3<T>& v)
        {
            return Transform3<T>(Quat<T>::identity(), Vec3<T>::one(), v);
        }

        Transform3() noexcept :
            m_rotation(Quat<T>::identity()),
            m_scale(Vec3<T>::one()),
            m_translation(Vec3<T>::zero())
        {

        }
        Transform3(const Quat<T>& q, const Vec3<T>& s, const Vec3<T>& v) :
            m_rotation(q),
            m_scale(s),
            m_translation(v)
        {
        }
        Transform3(const Transform3<T>&) = default;
        Transform3(Transform3<T>&&) noexcept = default;
        Transform3& operator=(const Transform3<T>&) = default;
        Transform3& operator=(Transform3<T>&&) noexcept = default;

        const Quat<T>& rotation() const
        {
            return m_rotation;
        }

        const Vec3<T>& scale() const
        {
            return m_scale;
        }

        const Vec3<T>& translation() const
        {
            return m_translation;
        }

        Transform3<T> inverse() const
        {
            const Quat<T> invRotation = m_rotation.inverse();
            const Vec3<T> invScale = T(1) / m_scale;
            const Vec3<T> invTranslation = invRotation.apply(invScale * -m_translation);

            return Transform3(invRotation, invScale, invTranslation);
        }

        // rhs before this
        Transform3<T>& operator*=(const Transform3<T>& rhs)
        {
            //	When Q = quaternion, S = single scalar scale, and T = translation
            //	QST(A) = Q(A), S(A), T(A), and QST(B) = Q(B), S(B), T(B)

            //	QST (AB) 

            // QST(A) = Q(A)*S(A)*P*-Q(A) + T(A)
            // QST(AB) = Q(A)*S(A)*QST(B)*-Q(A) + T(A)
            // QST(AB) = Q(A)*S(A)*[Q(B)*S(B)*P*-Q(B) + T(B)]*-Q(A) + T(A)
            // QST(AB) = Q(A)*S(A)*Q(B)*S(B)*P*-Q(A)*-Q(A) + Q(A)*S(A)*T(B)*-Q(A) + T(A)
            // QST(AB) = [Q(A)*Q(B)]*[S(A)*S(B)]*P*-[Q(A)*Q(B)] + Q(A)*S(A)*T(B)*-Q(A) + T(A)

            //	Q(AB) = Q(A)*Q(B)
            //	S(AB) = S(B)*S(A)
            //	T(AB) = Q(A)*S(A)*T(B)*-Q(A) + T(A)

            // does not handle negative or zero scale

            const Quat<T> rotation = m_rotation * rhs.m_rotation;
            const Vec3<T> scale = rhs.m_scale * m_scale;
            const Vec3<T> translation = m_rotation.apply(m_scale * rhs.m_translation) + m_translation;

            m_rotation = rotation;
            m_scale = scale;
            m_translation = translation;

            return *this;
        }


        Vec3<T> apply(const Vec3<T>& v) const
        {
            return m_rotation.apply(v * m_scale) + m_translation;
        }

    private:

        Quat<T> m_rotation;
        Vec3<T> m_scale;
        Vec3<T> m_translation;
    };

    using Transform3D = Transform3<double>;
    using Transform3F = Transform3<float>;

    // rhs before lhs
    template <class T>
    Transform3<T> operator*(const Transform3<T>& lhs, const Transform3<T>& rhs)
    {
        Transform3<T> result(lhs);
        return result *= rhs;
    }

    template <class T>
    Vec3<T> operator*(const Vec3<T>& lhs, const Transform3<T>& rhs)
    {
        return rhs.apply(lhs);
    }
}
