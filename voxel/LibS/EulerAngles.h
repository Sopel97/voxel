#pragma once

#include "Shapes/Angle2.h"

namespace ls
{
    template <class T>
    class EulerAngles
    {
    public:
        Angle2<T> pitch;
        Angle2<T> yaw;
        Angle2<T> roll;

        EulerAngles() noexcept = default;
        EulerAngles(const Angle2<T>& pitch, const Angle2<T>& yaw, const Angle2<T>& roll) :
            pitch(pitch),
            yaw(yaw),
            roll(roll)
        {

        }
        EulerAngles(const EulerAngles<T>&) = default;
        EulerAngles(EulerAngles<T>&&) noexcept = default;
        EulerAngles<T>& operator=(const EulerAngles<T>&) = default;
        EulerAngles<T>& operator=(EulerAngles<T>&&) noexcept = default;
    };

    using EulerAnglesD = EulerAngles<double>;
    using EulerAnglesF = EulerAngles<float>;
}
