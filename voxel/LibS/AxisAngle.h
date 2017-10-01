#pragma once

#include "Shapes/Angle2.h"
#include "Shapes/Vec3.h"

namespace ls
{
    template <class T>
    class AxisAngle
    {
    public:
        AxisAngle() noexcept = default;
        AxisAngle(const Vec3<T>& axis, const Angle2<T>& angle) :
            m_axis(axis.normalized()),
            m_angle(angle)
        {

        }
        AxisAngle(const AxisAngle<T>&) = default;
        AxisAngle(AxisAngle<T>&&) noexcept = default;
        AxisAngle<T>& operator=(const AxisAngle<T>&) = default;
        AxisAngle<T>& operator=(AxisAngle<T>&&) noexcept = default;

        const Vec3<T>& axis() const
        {
            return m_axis;
        }

        const Angle2<T>& angle() const
        {
            return m_angle;
        }

    private:
        Vec3<T> m_axis;
        Angle2<T> m_angle;
    };

    using AxisAngleD = AxisAngle<double>;
    using AxisAngleF = AxisAngle<float>;
}
