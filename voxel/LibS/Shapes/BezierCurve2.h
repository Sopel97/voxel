#pragma once

#include "Vec2.h"
#include "../Array2.h"
#include "../BinomialCoefficients.h"

#include <vector>
#include <type_traits>

namespace ls
{
    template <class T>
    class BezierCurve2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        std::vector<Vec2<T>> controlPoints;

        BezierCurve2() = default;
        BezierCurve2(const std::vector<Vec2<T>>& controlPoints);
        BezierCurve2(std::vector<Vec2<T>>&& controlPoints) noexcept;
        template<class InputIt>
        BezierCurve2(InputIt begin, InputIt end);
        BezierCurve2(const BezierCurve2<T>&) = default;
        BezierCurve2(BezierCurve2<T>&&) = default;

        BezierCurve2<T>& operator=(const BezierCurve2<T>&) = default;
        BezierCurve2<T>& operator=(BezierCurve2<T>&&) = default;

        Vec2<T> evaluate(const T& t) const;
        std::vector<Vec2<T>> evaluateAll(const int numberOfSamples) const;
        std::vector<Vec2<T>> evaluateRange(const T& rangeMin, const T& rangeMax, const int numberOfSamples) const;
        std::vector<Vec2<T>> evaluateAll(const T& step) const;
        std::vector<Vec2<T>> evaluateRange(const T& rangeMin, const T& rangeMax, const T& step) const;
    };

    using BezierCurve2D = BezierCurve2<double>;
    using BezierCurve2F = BezierCurve2<float>;



    template <class T>
    BezierCurve2<T>::BezierCurve2(const std::vector<Vec2<T>>& points) :
        controlPoints(points)
    {

    }
    template <class T>
    BezierCurve2<T>::BezierCurve2(std::vector<Vec2<T>>&& points) noexcept :
        controlPoints(std::move(points))
    {

    }
    template <class T>
    template<class InputIt>
    BezierCurve2<T>::BezierCurve2(InputIt begin, InputIt end) :
        controlPoints(begin, end)
    {

    }

    template <class T>
    Vec2<T> BezierCurve2<T>::evaluate(const T& t) const
    {
        Vec2<T> point(T(0), T(0));
        const int numberOfControlPoints = controlPoints.size();
        for (int i = 0; i < numberOfControlPoints; ++i)
        {
            point += controlPoints[i] * static_cast<T>(BinomialCoefficients64::value(numberOfControlPoints - 1, i)) * static_cast<T>(std::pow(T(1) - t, numberOfControlPoints - 1 - i)) * static_cast<T>(std::pow(t, i));
        }
        return point;
    }

    template <class T>
    std::vector<Vec2<T>> BezierCurve2<T>::evaluateAll(const int numberOfSamples) const
    {
        return evaluateRange(T(0), T(1), numberOfSamples);
    }

    template <class T>
    std::vector<Vec2<T>> BezierCurve2<T>::evaluateAll(const T& step) const
    {
        return evaluateRange(T(0), T(1), step);
    }

    template <class T>
    std::vector<Vec2<T>> BezierCurve2<T>::evaluateRange(const T& rangeMin, const T& rangeMax, const int numberOfSamples) const
    {
        std::vector<Vec2<T>> result;
        result.reserve(numberOfSamples);

        const T step = (rangeMax - rangeMin) / static_cast<T>(numberOfSamples - 1);

        T t = rangeMin;
        int i = 0;
        while (i < numberOfSamples)
        {
            result.emplace_back(evaluate(t));

            t += step;
            ++i;
        }

        return result;
    }

    template <class T>
    std::vector<Vec2<T>> BezierCurve2<T>::evaluateRange(const T& rangeMin, const T& rangeMax, const T& step) const
    {
        std::vector<Vec2<T>> result;
        result.reserve(static_cast<int>((rangeMax - rangeMin) / step) + 1);

        T t = rangeMin;
        while (t <= rangeMax)
        {
            result.emplace_back(evaluate(t));
            t += step;
        }

        return result;
    }
}
