// 
// Copyright 2021 Clemens Cords
// Created on 28.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SFML/System/Clock.hpp>
#include <functional>

namespace crisp
{
    using Clock = sf::Clock;
    using Duration = sf::Time;

    /// @brief simple benchmark object used for profiling
    template<typename Function_t>
    class Benchmark
    {
        public:
            /// @brief create from lambda
            /// @param lambda
            Benchmark(Function_t&& lambda);

            /// @brief measure average cycle duration
            /// @param n_cycles: number of executes
            /// @param args: arguments used for bound lambda
            /// @return time per cycle, in microseconds
            template<typename... Args_t>
            float execute(size_t n_cycles, Args_t... args);

            /// @brief measure duration of a single cycles
            /// @param args: arguments used for bound lambda
            /// @return time per cycle, in microseconds
            template<typename... Args_t>
            float execute(Args_t... args);

        private:
            Function_t _function;
    };
}

#include ".src/benchmark.inl"