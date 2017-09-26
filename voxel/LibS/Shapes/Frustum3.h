#pragma once

#include "Plane3.h"
#include "../Matrix.h"

#include <array>

namespace ls
{
    template <class T>
    class Frustum3
    {
    public:
        enum class Side
        {
            Left,
            Right,
            Bottom,
            Top,
            Near,
            Far
        };

        std::array<Plane3<T>, 6> planes;

        Frustum3(
            const Plane3<T>& left,
            const Plane3<T>& right,
            const Plane3<T>& bottom,
            const Plane3<T>& top,
            const Plane3<T>& near,
            const Plane3<T>& far
        ) :
            planes{ left, right, bottom, top, near, far }
        {

        }

        const Plane3<T>& plane(Side side) const
        {
            return planes[static_cast<int>(side)];
        }
        Plane3<T>& plane(Side side)
        {
            return planes[static_cast<int>(side)];
        }

        static Frustum3 fromMatrix(const Matrix<T, 4, 4>& m)
        {
            Frustum3 frustum(
                Plane3<T>(m[3][0] + m[0][0], m[3][1] + m[0][1], m[3][2] + m[0][2], m[3][3] + m[0][3]),
                Plane3<T>(m[3][0] - m[0][0], m[3][1] - m[0][1], m[3][2] - m[0][2], m[3][3] - m[0][3]),
                Plane3<T>(m[3][0] + m[1][0], m[3][1] + m[1][1], m[3][2] + m[1][2], m[3][3] + m[1][3]),
                Plane3<T>(m[3][0] - m[1][0], m[3][1] - m[1][1], m[3][2] - m[1][2], m[3][3] - m[1][3]),
                Plane3<T>(m[3][0] + m[2][0], m[3][1] + m[2][1], m[3][2] + m[2][2], m[3][3] + m[2][3]),
                Plane3<T>(m[3][0] - m[2][0], m[3][1] - m[2][1], m[3][2] - m[2][2], m[3][3] - m[2][3])
            );

            return frustum;
        }
    };

    using Frustum3F = Frustum3<float>;
    using Frustum3D = Frustum3<double>;
}
