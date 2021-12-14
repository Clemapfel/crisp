// 
// Copyright 2021 Clemens Cords
// Created on 14.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector>

namespace crisp
{
    /// @brief apply windowing function to vector out-of-place
    /// @param window_function: weight function for range [0, 1]
    /// @param signal
    /// @returns weighted signal as new vector
    template<typename WindowFunction_t, typename T>
    std::vector<T> apply_window(WindowFunction_t window_function, const std::vector<T>&);

    /// @brief apply windowing function to vector in-place
    /// @param window_function: weight function for range [0, 1]
    /// @param signal
    template<typename WindowFunction_t, typename T>
    void apply_window(WindowFunction_t window_function, std::vector<T>&);

    /// @brief apply windowing function to number of elements
    /// @param window_function: weight function for range [0, 1]
    /// @param signal_ptr: pointer to first element
    /// @param n: number of elements
    template<typename WindowFunction_t, typename T>
    void apply_window(WindowFunction_t window_function, T*, size_t n);
    
    namespace WindowFunction
    {
        /// @brief hann-window
        /// @param x: range [0, 1]
        static inline auto hann = [](float x)
        {
            assert(x >= 0 and x <= 1);
            return pow(cos(M_PI * x - (M_PI / 2)), 2);
        };

        /// @brief gauss window, fourier-transform of this function is identical to spatial domain function
        /// @param x: range [0, 1]
        static inline auto gauss = [](float x)
        {
            assert(x >= 0 and x <= 1);
            return exp(-4 * pow(2 * x - 1, 2));
        };

        /// @brief triangle window, linear ramp from y = 0 to y = 1 from x = [0, 0.5], then from y = 1 to y = 0 from x = [0.5, 1]
        /// @param x: range [0, 1]
        static inline auto triangle = [](float x)
        {
            assert(x >= 0 and x <= 1);
            return -2 * abs(x - 0.5) + 1;
        };

        /// @brief identity function, 1 at all points
        /// @param x: range [0, 1]
        static inline auto identity = [](float x)
        {
            assert(x >= 0 and x <= 1);
            return 1;
        };
    }
}

#include ".src/window_function.inl"