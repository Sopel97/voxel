#pragma once

#include "Readers.h"

#include "../Shapes/Vec2.h"
#include "../Shapes/Vec3.h"
#include "../Shapes/Vec4.h"

namespace ls
{
    namespace json
    {
        template <class T>
        struct Reader<Vec2<T>>
        {
            static Vec2<T> fromJson(const Value& val)
            {
                return Vec2<T>(
                    json::fromJson<T>(val[0]),
                    json::fromJson<T>(val[1])
                );
            }
        };

        template <class T>
        struct Reader<Vec3<T>>
        {
            static Vec3<T> fromJson(const Value& val)
            {
                return Vec3<T>(
                    json::fromJson<T>(val[0]),
                    json::fromJson<T>(val[1]),
                    json::fromJson<T>(val[2])
                );
            }
        };

        template <class T>
        struct Reader<Vec4<T>>
        {
            static Vec4<T> fromJson(const Value& val)
            {
                return Vec4<T>(
                    json::fromJson<T>(val[0]),
                    json::fromJson<T>(val[1]),
                    json::fromJson<T>(val[2]),
                    json::fromJson<T>(val[3])
                );
            }
        };
    }
}
