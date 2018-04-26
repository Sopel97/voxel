#pragma once

#include "../LibS/Fwd.h"

#include <cstdint>
#include <array>

class CubeSide;

template <class T>
class PerCubeSideData
{
public:

    constexpr PerCubeSideData() = default;

    template <typename... InitT>
    PerCubeSideData(InitT&&... values) :
        m_values{ std::forward<InitT>(values)... }
    {
    }

    const T & operator[](CubeSide side) const
    {
        return m_values[side.ordinal()];
    }
    T & operator[](CubeSide side)
    {
        return m_values[side.ordinal()];
    }

private:
    std::array<T, 6> m_values;
};

template <>
class PerCubeSideData<bool>
{
public:
    struct Reference
    {
        Reference(uint8_t& value, int offset) :
            m_valuePtr(&value),
            m_offset(offset)
        {
        }

        Reference& operator=(bool bit)
        {
            const uint8_t mask = ~(1 << m_offset);
            *m_valuePtr = (mask & *m_valuePtr) | (bit << m_offset);
            return *this;
        }

        operator bool() const
        {
            return (*m_valuePtr >> m_offset) & 1;
        }

    private:
        uint8_t* m_valuePtr;
        int m_offset;
    };

    constexpr PerCubeSideData() :
        m_values(0)
    {

    }

    constexpr PerCubeSideData(bool east, bool west, bool top, bool bottom, bool south, bool north) :
        m_values(
            east
            + (west << 1)
            + (top << 2)
            + (bottom << 3)
            + (south << 4)
            + (north << 5)
        )
    {

    }

    static constexpr PerCubeSideData<bool> none()
    {
        return { 0 };
    }
    static constexpr PerCubeSideData<bool> all()
    {
        return { 0b00111111 };
    }

    static PerCubeSideData<bool> fromJson(const ls::json::Value& config);

    bool operator[](CubeSide side) const;
    Reference operator[](CubeSide side);

private:
    constexpr PerCubeSideData(uint8_t val) :
        m_values(val)
    {

    }

    uint8_t m_values;
};
