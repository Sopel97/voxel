#pragma once

#include "Shapes.h"

#include "MathConstants.h"

#include <random>

namespace ls
{
    template <class T, class RNG>
    Vec2<T> sampleUniformPointOnPerimeter(const Circle2<T>& circle, RNG&& rng)
    {
        static const std::uniform_real_distribution<T> angleDistribution(T(0), T(2) * pi<T>);

        const Angle2<T> angle = Angle2<T>::radians(angleDistribution(std::forward<RNG>(rng)));
        return circle.origin + Vec2<T>(angle.cos(), angle.sin());
    }
    template <class T, class RNG>
    Vec2<T> sampleUniformPointOnArea(const Circle2<T>& circle, RNG&& rng)
    {
        using std::sqrt;

        static const std::uniform_real_distribution<T> radiusSquaredDistribution(T(0), T(1));
        static const std::uniform_real_distribution<T> angleDistribution(T(0), T(2) * pi<T>);

        const T r = sqrt(radiusSquaredDistribution(rng)) * circle.radius;
        const Angle2<T> angle = Angle2<T>::radians(angleDistribution(std::forward<RNG>(rng)));
        return circle.origin + Vec2<T>(r * angle.cos(), r * angle.sin());
    }

    template <class T, class RNG>
    Vec2<T> sampleUniformPoint(const Edge2<T>& edge, RNG&& rng)
    {
        static const std::uniform_real_distribution<T> scalarDistribution(T(0), T(1));

        const T t = scalarDistribution(std::forward<RNG>(rng));
        return edge.vertices[0] + (edge.vertices[0] - edge.vertices[1]) * t;
    }

    template <class T, class RNG>
    Vec2<T> sampleUniformPointOnPerimeter(const Box2<T>& box, RNG&& rng)
    {
        static const std::uniform_real_distribution<T> scalarDistribution(T(0), T(1));

        const T width = box.width();
        const T height = box.height();
        std::uniform_real_distribution<T> lengthAlongPerimeterDistribution(T(0), width + height);
        std::bernoulli_distribution doOppositeEdgeDistribution(0.5);

        Vec2<T> point = box.min;

        const bool doOppositEdge = doOppositeEdgeDistribution(rng);
        if (lengthAlongPerimeterDistribution(rng) > width) //horizontally
        {
            point.x += scalarDistribution(std::forward<RNG>(rng)) * width;
            if (doOppositEdge) point.y += height;
        }
        else //vertically
        {
            point.y += scalarDistribution(std::forward<RNG>(rng)) * height;
            if (doOppositEdge) point.x += width;
        }

        return point;
    }
    template <class T, class RNG>
    Vec2<T> sampleUniformPointOnArea(const Box2<T>& box, RNG&& rng)
    {
        static const std::uniform_real_distribution<T> scalarDistribution(T(0), T(1));

        const T t1 = scalarDistribution(std::forward<RNG>(rng));
        const T t2 = scalarDistribution(std::forward<RNG>(rng));
        Vec2<T> displacement = box.max - box.min;
        displacement.x *= t1;
        displacement.y *= t2;
        return box.min + displacement;
    }

    template <class T, class RNG>
    Vec2<T> sampleUniformPointOnPerimeter(const Triangle2<T>& triangle, RNG&& rng)
    {
        static const std::uniform_real_distribution<T> scalarDistribution(T(0), T(1));

        const T lengths[3] = {
            triangle.vertices[0].distance(triangle.vertices[1]),
            triangle.vertices[1].distance(triangle.vertices[2]),
            triangle.vertices[2].distance(triangle.vertices[0])
        };

        T lengthSums[3];
        lengthSums[0] = lengths[0];
        lengthSums[1] = lengthSums[0] + lengths[1];
        lengthSums[2] = lengthSums[1] + lengths[2];

        const T perimeterLength = lengths[0] + lengths[1] + lengths[2];

        std::uniform_real_distribution<T> lengthAlongPerimeterDistribution(T(0), perimeterLength);
        const T lengthAlongPerimeter = lengthAlongPerimeterDistribution(rng);
        int edgeIndex = 0;
        if (lengthAlongPerimeter > lengthSums[1]) edgeIndex = 2;
        else if (lengthAlongPerimeter > lengthSums[0]) edgeIndex = 1;

        const Vec2<T> begin = triangle.vertices[edgeIndex];
        const Vec2<T> end = triangle.vertices[(edgeIndex + 1) % 3];
        const T t = scalarDistribution(std::forward<RNG>(rng));
        return begin + (end - begin) * t;
    }
    template <class T, class RNG>
    Vec2<T> sampleUniformPointOnArea(const Triangle2<T>& triangle, RNG&& rng)
    {
        static const std::uniform_real_distribution<T> scalarDistribution(T(0), T(1));

        const T t1 = scalarDistribution(rng);
        const T t2 = scalarDistribution(std::forward<RNG>(rng));
        Vec2<T> point(triangle.vertices[0] + (triangle.vertices[2] - triangle.vertices[0]) * t1 + (triangle.vertices[1] - triangle.vertices[0]) * t2);
        const Vec2<T> e12 = triangle.vertices[2] - triangle.vertices[1];
        const Vec2<T> e10 = triangle.vertices[0] - triangle.vertices[1];
        const Vec2<T> e1p = point - triangle.vertices[1];
        if (e12.cross(e1p) * e12.cross(e10) < T(0)) //points vertices[0] and point are on the opposite sides of the diagonal of the parallelogram
        {
            const Vec2<T> edgeCenter = triangle.vertices[1] + (e12 * T(0.5));
            point = edgeCenter * T(2) - point;
        }
        return point;
    }
}
