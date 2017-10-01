#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include <stack>
#include <tuple>
#include <functional>

namespace ls
{
    template <class, int...>
    class Array3;

    template <class T>
    class Array3<T>
    {
    private:

    public:
        using ValueType = T;

        using iterator = T*;
        using const_iterator = const T*;

        Array3() noexcept :
            m_data(nullptr),
            m_width(0),
            m_height(0),
            m_depth(0)
        {

        }

        Array3(int width, int height, int depth) :
            m_width(width),
            m_height(height),
            m_depth(depth)
        {
            const int totalSize = width * height * depth;
            m_data = std::make_unique<T[]>(totalSize);
        }

        Array3(int width, int height, int depth, const T& initValue) :
            m_width(width),
            m_height(height),
            m_depth(depth)
        {
            const int totalSize = width * height * depth;
            m_data = std::make_unique<T[]>(totalSize);
            for (int i = 0; i < totalSize; ++i)
            {
                m_data[i] = initValue;
            }
        }

        Array3(const Array3<T>& other) :
            m_width(other.m_width),
            m_height(other.m_height),
            m_depth(other.m_depth)
        {
            const int totalSize = m_width * m_height * m_depth;
            m_data = std::make_unique<T[]>(totalSize);
            for (int i = 0; i < totalSize; ++i)
            {
                m_data[i] = other.m_data[i];
            }
        }

        Array3(Array3<T>&& other) noexcept :
            m_data(std::move(other.m_data)),
            m_width(std::move(other.m_width)),
            m_height(std::move(other.m_height)),
            m_depth(std::move(other.m_depth))
        {
            other.m_data = nullptr;
        }

        const T& operator() (int x, int y, int z) const
        {
            return m_data[index(x, y, z)];
        }
        T& operator() (int x, int y, int z)
        {
            return m_data[index(x, y, z)];
        }
        const T& at(int x, int y, int z) const
        {
            return m_data[index(x, y, z)];
        }
        T& at(int x, int y, int z)
        {
            return m_data[index(x, y, z)];
        }

        Array3& operator= (Array3<T> && other) noexcept
        {
            m_data = std::move(other.m_data);
            m_width = other.m_width;
            m_height = other.m_height;
            m_depth = other.m_depth;

            other.m_data = nullptr;

            return *this;
        }

        Array3& operator= (const Array3<T>& other)
        {
            return operator=(Array3(other));
        }

        void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }

        void swap(Array3<T>& other) noexcept
        {
            Array3<T> temp(std::move(*this));
            *this = std::move(other);
            other = std::move(temp);
        }

        const T* data() const
        {
            return m_data.get();
        }

        int width() const
        {
            return m_width;
        }

        int height() const
        {
            return m_height;
        }

        int depth() const
        {
            return m_depth;
        }

        bool isEmpty() const
        {
            return m_data == nullptr;
        }

        iterator begin()
        {
            return m_data.get();
        }

        iterator end()
        {
            const int totalSize = m_width * m_height * m_depth;
            return m_data.get() + totalSize;
        }
        const_iterator begin() const
        {
            return m_data.get();
        }

        const_iterator end() const
        {
            const int totalSize = m_width * m_height * m_depth;
            return m_data.get() + totalSize;
        }
        const_iterator cbegin() const
        {
            return m_data.get();
        }

        const_iterator cend() const
        {
            const int totalSize = m_width * m_height * m_depth;
            return m_data.get() + totalSize;
        }

    private:
        std::unique_ptr<T[]> m_data;
        int m_width;
        int m_height;
        int m_depth;

        int index(int x, int y, int z) const
        {
            return (x * m_height + y) * m_depth + z;
        }
    };

    template <class T>
    void swap(Array3<T>& lhs, Array3<T>& rhs) noexcept
    {
        lhs.swap(rhs);
    }



    template <class T, int Width, int Height, int Depth>
    class Array3<T, Width, Height, Depth>
    {
    public:
        using ValueType = T;
        using SelfType = Array3<T, Width, Height, Depth>;

        using iterator = T*;
        using const_iterator = const T*;

        Array3() :
            m_data(std::make_unique<T[]>(bufferSize()))
        {

        }

        Array3(const T& initValue) :
            Array3()
        {
            fill(initValue);
        }

        Array3(const SelfType& other) :
            Array3()
        {
            for (int i = 0; i < bufferSize(); ++i)
            {
                m_data[i] = other.m_data[i];
            }
        }

        Array3(SelfType&& other) noexcept :
            m_data(std::move(other.m_data))
        {
        }

        const T& operator() (int x, int y, int z) const
        {
            return m_data[index(x, y, z)];
        }
        T& operator() (int x, int y, int z)
        {
            return m_data[index(x, y, z)];
        }
        const T& at(int x, int y, int z) const
        {
            return m_data[index(x, y, z)];
        }
        T& at(int x, int y, int z)
        {
            return m_data[index(x, y, z)];
        }

        Array3& operator= (SelfType&& other) noexcept
        {
            m_data = std::move(other.m_data);

            return *this;
        }

        Array3& operator= (const SelfType& other)
        {
            return operator=(Array3(other));
        }

        void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }

        void swap(SelfType& other) noexcept
        {
            SelfType temp(std::move(*this));
            *this = std::move(other);
            other = std::move(temp);
        }

        const T* data() const
        {
            return m_data.get();
        }

        static constexpr int width()
        {
            return Width;
        }

        static constexpr int height()
        {
            return Height;
        }

        static constexpr int depth()
        {
            return Depth;
        }

        bool isEmpty() const
        {
            return m_data == nullptr;
        }

        iterator begin()
        {
            return m_data.get();
        }

        iterator end()
        {
            return m_data.get() + bufferSize();
        }
        const_iterator begin() const
        {
            return cbegin();
        }

        const_iterator end() const
        {
            return cend();
        }
        const_iterator cbegin() const
        {
            return m_data.get();
        }

        const_iterator cend() const
        {
            return m_data.get() + bufferSize();
        }

    private:
        std::unique_ptr<T[]> m_data;

        int index(int x, int y, int z) const
        {
            return x * (Depth * Height) + y * Depth + z;
        }

        static constexpr int bufferSize()
        {
            return Width * Height * Depth;
        }
    };

    template <class T, int W, int H, int D>
    void swap(Array3<T, W, H, D>& lhs, Array3<T, W, H, D>& rhs) noexcept
    {
        lhs.swap(rhs);
    }
}
