#pragma once

#include "Fwd.h"

namespace ls
{
    namespace util
    {
        template <class T, int Dim>
        struct VectorType
        {
        };

        template <class T>
        struct VectorType<T, 1>
        {
            using type = T;
        };
        template <class T>
        struct VectorType<T, 2>
        {
            using type = Vec2<T>;
        };
        template <class T>
        struct VectorType<T, 3>
        {
            using type = Vec3<T>;
        };
        template <class T>
        struct VectorType<T, 4>
        {
            using type = Vec4<T>;
        };
    }
}
