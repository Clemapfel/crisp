// 
// Copyright 2021 Clemens Cords
// Created on 14.12.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    template<typename WindowFunction_t, typename T>
    std::vector<T> apply_window(WindowFunction_t window_function, const std::vector<T>& signal)
    {
        std::vector<T> out;
        out.reserve(signal.size());

        for (size_t i = 0; i < signal.size(); ++i)
            out.push_back(signal.at(i) * window_function(i / float(signal.size())));
    }

    template<typename WindowFunction_t, typename T>
    void apply_window(WindowFunction_t window_function, std::vector<T>&)
    {
        for (size_t i = 0; i < signal.size(); ++i)
            signal.at(i) *= window_function(i / float(signal.size()));
    }

    template<typename WindowFunction_t, typename T>
    void apply_window(WindowFunction_t window_function, T* ptr, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
            *(ptr + i) *= window_function(i / float(n));
    }
}
