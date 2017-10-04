#pragma once

#include "../Array2.h"
#include "../Shapes/Box2.h"

namespace ls
{
    template <class Rule> //class representing a rule
    class CellularAutomaton2
    {
    public:
        using RuleType = Rule;
        using State = typename Rule::State;

        enum class DefaultState
        {
            White,
            Black
        };
        enum class GridTopology
        {
            Finite,
            Toroidal //for toroidal space it is still required that passed coordinates are inside the bounds
        };


        CellularAutomaton2(const Rule& rule, size_t width, size_t height, GridTopology topology = GridTopology::Finite);
        CellularAutomaton2(const Rule& rule, size_t width, size_t height, State fillState, GridTopology topology = GridTopology::Finite);

        State at(size_t x, size_t y) const;

        void setState(size_t x, size_t y, State state) &;

        void fill(State state) &;
        template <class FillFunction>
        void fill(FillFunction fillingFunction) &; //fill function must take x, y as coordinates and output valid State

        void iterate(size_t numberOfIterations = 1) &;

        size_t occurencesIn3x3(State state, size_t x, size_t y) const; //x,y determine center of the 3x3 region
        size_t occurencesIn5x5(State state, size_t x, size_t y) const; //x,y determine center of the 5x5 region
        size_t occurencesInRadius(State state, size_t x, size_t y, size_t radius) const; //x,y determine center of the region, radius is a maximal chessboard distance
        size_t occurencesInRect(State state, const Box2<int>& rect) const;
        size_t occurencesInMooreNeighbourhood(State state, size_t x, size_t y) const; //only sides
        size_t occurencesInNeighbourhood(State state, size_t x, size_t y) const; //sides and diagonals

    protected:
        Rule m_rule;
        GridTopology m_topology;
        Array2<State> m_grid;
    };

    template <class SetOfStates> //enum representing all possible states
    class QuantityRule3x3
    {
    public:
        using State = SetOfStates;

        QuantityRule3x3(State countedState, const std::array<State, 10u>& outputs) :
            m_countedState(countedState),
            m_outputs(outputs)
        {

        }

        State operator()(const CellularAutomaton2<QuantityRule3x3>& automaton, size_t x, size_t y)
        {
            return m_outputs[automaton.occurencesIn3x3(m_countedState, x, y)];
        }

        void setOutputForQuantity(State outputState, size_t quantity)
        {
            m_outputs[quantity] = outputState;
        }

    protected:
        State m_countedState;
        std::array<State, 10u> m_outputs; //the quantity of state in 3x3 region is the index
    };

    class ConwaysGameOfLifeRule
    {
    public:
        enum class State
        {
            Dead,
            Live
        };

        ConwaysGameOfLifeRule() {}

        State operator()(const CellularAutomaton2<ConwaysGameOfLifeRule>& automaton, size_t x, size_t y)
        {
            const size_t numberOfLivingNeighbours = automaton.occurencesInNeighbourhood(State::Live, x, y);
            const auto& currentState = automaton.at(x, y);

            if (currentState == State::Live)
            {
                if (numberOfLivingNeighbours < 2) return State::Dead;
                else if (numberOfLivingNeighbours > 3) return State::Dead;
            }
            else //Dead
            {
                if (numberOfLivingNeighbours == 3) return State::Live;
            }

            return currentState;
        }
    };
    class WireworldRule
    {
    public:
        enum class State
        {
            Empty,
            ElectronHead,
            ElectronTail,
            Conductor
        };

        WireworldRule() {}

        State operator()(const CellularAutomaton2<WireworldRule>& automaton, size_t x, size_t y)
        {
            State currentState = automaton.at(x, y);

            if (currentState == State::Empty) return State::Empty;
            else if (currentState == State::ElectronHead) return State::ElectronTail;
            else if (currentState == State::ElectronTail) return State::Conductor;
            else //(currentState == State::Conductor)
            {
                const size_t numberOfElectronHeadsInNeighbourhood = automaton.occurencesInMooreNeighbourhood(State::ElectronHead, x, y);
                if (numberOfElectronHeadsInNeighbourhood == 1 || numberOfElectronHeadsInNeighbourhood == 2)
                    return State::ElectronHead;
            }

            return currentState;
        }
    };



    template <class Rule>
    CellularAutomaton2<Rule>::CellularAutomaton2(const Rule& rule, size_t width, size_t height, GridTopology topology) :
        m_rule(rule),
        m_topology(topology),
        m_grid(width, height)
    {

    }

    template <class Rule>
    CellularAutomaton2<Rule>::CellularAutomaton2(const Rule& rule, size_t width, size_t height, State fillState, GridTopology topology) :
        m_rule(rule),
        m_topology(topology),
        m_grid(width, height, fillState)
    {

    }

    template <class Rule>
    typename CellularAutomaton2<Rule>::State CellularAutomaton2<Rule>::at(size_t x, size_t y) const
    {
        return m_grid(x, y);
    }

    template <class Rule>
    void CellularAutomaton2<Rule>::fill(State state) &
    {
        for (auto& cell : m_grid)
        {
            cell = state;
        }
    }
    template <class Rule>
    template <class FillFunction>
    void CellularAutomaton2<Rule>::fill(FillFunction fillingFunction) &
    {
        const size_t w = m_grid.sizeX();
        const size_t h = m_grid.sizeY();

        for (size_t x = 0; x < w; ++x)
        {
            for (size_t y = 0; y < h; ++y)
            {
                m_grid(x, y) = fillingFunction(x, y);
            }
        }
    }

    template <class Rule>
    void CellularAutomaton2<Rule>::setState(size_t x, size_t y, State state) &
    {
        m_grid(x, y) = state;
    }

    template <class Rule>
    void CellularAutomaton2<Rule>::iterate(size_t iterations) &
    {
        const size_t w = m_grid.width();
        const size_t h = m_grid.height();

        Array2<State> nextGrid(w, h);
        while (iterations--)
        {
            for (size_t x = 0; x < w; ++x)
            {
                for (size_t y = 0; y < h; ++y)
                {
                    nextGrid(x, y) = m_rule(*this, x, y);
                }
            }

            m_grid.swap(nextGrid);
        }
    }

    template <class Rule>
    size_t CellularAutomaton2<Rule>::occurencesIn3x3(State state, size_t x, size_t y) const
    {
        return occurencesInRadius(state, x, y, 1);
    }
    template <class Rule>
    size_t CellularAutomaton2<Rule>::occurencesIn5x5(State state, size_t x, size_t y) const
    {
        return occurencesInRadius(state, x, y, 2);
    }
    template <class Rule>
    size_t CellularAutomaton2<Rule>::occurencesInRadius(State state, size_t x, size_t y, size_t radius) const
    {
        const Vec2<int> diagonalVector(static_cast<int>(radius), static_cast<int>(radius));
        const Vec2<int> centerVector(static_cast<int>(x), static_cast<int>(y));
        return occurencesInRect(state, Box2<int>(centerVector - diagonalVector, centerVector + diagonalVector));
    }
    template <class Rule>
    size_t CellularAutomaton2<Rule>::occurencesInRect(State state, const Box2<int>& rect) const
    {
        const size_t width = m_grid.width();
        const size_t height = m_grid.height();

        size_t quantity = 0;

        if (m_topology == GridTopology::Finite)
        {
            size_t xmin = static_cast<size_t>(std::max(rect.min.x, 0));
            size_t ymin = static_cast<size_t>(std::max(rect.min.y, 0));
            size_t xmax = static_cast<size_t>(std::min(rect.max.x, static_cast<int>(width - 1)));
            size_t ymax = static_cast<size_t>(std::min(rect.max.y, static_cast<int>(height - 1)));

            for (size_t xx = xmin; xx <= xmax; ++xx)
            {
                for (size_t yy = ymin; yy <= ymax; ++yy)
                {
                    if (m_grid(xx, yy) == state) ++quantity;
                }
            }
        }
        else if (m_topology == GridTopology::Toroidal)
        {
            for (int xx = rect.min.x; xx <= rect.max.x; ++xx)
            {
                for (int yy = rect.min.y; yy <= rect.max.y; ++yy)
                {
                    if (m_grid((xx + width) % width, (yy + height) % height) == state) ++quantity;
                }
            }
        }

        return quantity;
    }

    template <class Rule>
    size_t CellularAutomaton2<Rule>::occurencesInMooreNeighbourhood(State state, size_t x, size_t y) const
    {
        const size_t width = m_grid.width();
        const size_t height = m_grid.height();

        size_t quantity = 0;

        if (m_topology == GridTopology::Finite)
        {
            if (x > 0 && m_grid(x - 1, y) == state) ++quantity;
            if (x < width - 1 && m_grid(x + 1, y) == state) ++quantity;
            if (y > 0 && m_grid(x, y - 1) == state) ++quantity;
            if (y < height - 1 && m_grid(x, y + 1) == state) ++quantity;
        }
        else if (m_topology == GridTopology::Toroidal)
        {
            if (m_grid((x + width - 1) % width, y) == state) ++quantity;
            if (m_grid((x + 1) % width, y) == state) ++quantity;
            if (m_grid(x, (y + height - 1) % height) == state) ++quantity;
            if (m_grid(x, (y + 1) % height) == state) ++quantity;
        }

        return quantity;
    }

    template <class Rule>
    size_t CellularAutomaton2<Rule>::occurencesInNeighbourhood(State state, size_t x, size_t y) const
    {
        return occurencesIn3x3(state, x, y) - (m_grid(x, y) == state);
    }
}
