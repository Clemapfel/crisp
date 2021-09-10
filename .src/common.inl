// 
// Copyright 2021 Clemens Cords
// Created on 12.08.21 by clem (mail@clemens-cords.com)
//

#pragma once

namespace crisp
{
    template<typename T>
    T project(T lower_bound, T upper_bound, T value)
    {
        return value * fabs(lower_bound - upper_bound) + std::min(lower_bound, upper_bound);
    }

    template<typename T>
    T  sign(T in)
    {
        if (in < T(0))
            return -1;
        else if (in > T(0))
            return 1;
        else
            return 0;
    }
}