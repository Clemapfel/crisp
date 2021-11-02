// 
// Copyright 2021 Clemens Cords
// Created on 28.10.21 by clem (mail@clemens-cords.com)
//

#include <benchmark.hpp>

namespace crisp
{
    template<typename Function_t>
    Benchmark<Function_t>::Benchmark(Function_t&& lambda)
        : _function(lambda)
    {}

    template<typename Function_t>
    template<typename... Args_t>
    float Benchmark<Function_t>::execute(size_t n_cycles, Args_t... args)
    {
        sf::Clock clock;

        Duration current_sum = sf::seconds(0);
        size_t current_n = 0;
        for (size_t i = 0; i < n_cycles; ++i)
        {
            clock.restart();
            _function(args...);
            current_sum += clock.restart();
            current_n += 1;
        }

        return float(current_sum.asMicroseconds()) / float(current_n);
    }
}

