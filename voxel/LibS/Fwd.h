#pragma once

// Noise
namespace ls
{
    template <class T, int Dim>
    class NoiseSampler;

    template <int N, class IntType>
    class PermTable;

    class OriginalPerlinPermTable;

    template <class T, int N>
    struct ValueDerivativePair;

    template <class T, class Hasher = OriginalPerlinPermTable>
    class PerlinNoise;
    using PerlinNoiseD = PerlinNoise<double>;
    using PerlinNoiseF = PerlinNoise<float>;

    template <class T, class Hasher = OriginalPerlinPermTable>
    class SimplexNoise;
    using SimplexNoiseD = SimplexNoise<double>;
    using SimplexNoiseF = SimplexNoise<float>;
}

// Memory
namespace ls
{
    namespace mem
    {
        template <class T>
        class HomogeneousMemoryPool;

        template <class T>
        class StaticEmbeddedHomogeneousMemoryPool;
    }
}

// Json
namespace ls
{
    namespace json
    {
        class Value;
        class Document;
        class Writer;
        class DocumentParser;

        template <class...>
        struct Reader;
    }
}

// Collisions
namespace ls
{
    template <class T>
    class ContinuousCollision2;

    template <class ShapeT, class VectorT = typename ShapeT::VectorType>
    class Accelerating;

    template <class ShapeT, class VectorT = typename ShapeT::VectorType>
    class Moving;

    template <class T>
    struct PointNormalPair2;
}

// Cellular Automata
namespace ls
{
    template <class Rule> //class representing a rule
    class CellularAutomaton2;

    template <class SetOfStates> //enum representing all possible states
    class QuantityRule3x3;

    class ConwaysGameOfLifeRule;

    class WireworldRule;
}

// Shapes
namespace ls
{
    template <class T>
    class Angle2;
    using Angle2F = Angle2<float>;
    using Angle2D = Angle2<double>;

    template <class T>
    class BezierCurve2;
    using BezierCurve2F = BezierCurve2<float>;
    using BezierCurve2D = BezierCurve2<double>;

    template <class T>
    class BezierPath2;
    using BezierPath2F = BezierPath2<float>;
    using BezierPath2D = BezierPath2<double>;

    template <class T>
    class Box2;
    using Box2F = Box2<float>;
    using Box2D = Box2<double>;
    using Box2I = Box2<int>;

    template <class T>
    class Box3;
    using Box3F = Box3<float>;
    using Box3D = Box3<double>;
    using Box3I = Box3<int>;

    template <class T>
    class Capsule3;
    using Capsule3F = Capsule3<float>;
    using Capsule3D = Capsule3<double>;

    template <class T>
    class Circle2;
    using Circle2F = Circle2<float>;
    using Circle2D = Circle2<double>;

    template <class T>
    class ConvexPolygon2;
    using ConvexPolygon2F = ConvexPolygon2<float>;
    using ConvexPolygon2D = ConvexPolygon2<double>;

    template <class T>
    class Cylinder3;
    using Cylinder3F = Cylinder3<float>;
    using Cylinder3D = Cylinder3<double>;

    template <class T>
    class Edge2;
    using Edge2F = Edge2<float>;
    using Edge2D = Edge2<double>;

    template <class T>
    class Edge3;
    using Edge3F = Edge3<float>;
    using Edge3D = Edge3<double>;

    template <class T>
    class Polyline2;
    using Polyline2F = Polyline2<float>;
    using Polyline2D = Polyline2<double>;

    template <class T>
    class Ray2;
    using Ray2F = Ray2<float>;
    using Ray2D = Ray2<double>;

    template <class T>
    class Ray3;
    using Ray3F = Ray3<float>;
    using Ray3D = Ray3<double>;

    template <class T>
    class Sphere3;
    using Sphere3F = Sphere3<float>;
    using Sphere3D = Sphere3<double>;

    template <class T>
    class Triangle2;
    using Triangle2F = Triangle2<float>;
    using Triangle2D = Triangle2<double>;

    template <class T>
    class Triangle3;
    using Triangle3F = Triangle3<float>;
    using Triangle3D = Triangle3<double>;

    template <class T>
    class Vec2;
    using Vec2F = Vec2<float>;
    using Vec2D = Vec2<double>;
    using Vec2I = Vec2<int>;

    template <class T>
    class Vec3;
    using Vec3F = Vec3<float>;
    using Vec3D = Vec3<double>;
    using Vec3I = Vec3<int>;

    template <class T>
    class Vec4;
    using Vec4F = Vec4<float>;
    using Vec4D = Vec4<double>;
    using Vec4I = Vec4<int>;
}

// Others
namespace ls
{
    template <class T>
    class Array2;

    template <class T>
    class AxisAngle;
    using AxisAngleF = AxisAngle<float>;
    using AxisAngleD = AxisAngle<double>;

    template <class T>
    class EulerAngles;
    using EulerAnglesF = EulerAngles<float>;
    using EulerAnglesD = EulerAngles<double>;

    template <class T, int R, int C>
    class Matrix;
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

    template <class T>
    class Quat;
    using QuatF = Quat<float>;
    using QuatD = Quat<double>;

    template <class T>
    class Transform3;
    using Transform3D = Transform3<double>;
    using Transform3F = Transform3<float>;
}
