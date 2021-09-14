// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#include <.src/common.inl>

namespace crisp
{
    template<size_t N>
    Histogram<N>::Histogram()
    {
        for (size_t i = 0; i < N; ++i)
            _data.emplace(i, 0);
    }

    template<size_t N>
    template<typename Inner_t>
    Histogram<N>::Histogram(const Image<Inner_t, 1>& image)
        : Histogram()
    {
        create_from(image);
    }

    template<size_t N>
    template<typename Inner_t>
    void Histogram<N>::create_from(const Image<Inner_t, 1>& image)
    {
        for (auto& pair : _data)
            pair.second = 0;

        size_t n = 0;
        Inner_t sum = 0;
        for (const auto& it : image)
        {
            float value = float(it);
            value = clamp<float>(0, 1, value);

            value *= N;
            _data.at(size_t(floor(value))) += 1;

            sum += value;
            n += 1;
        }

        _mean = double(sum) / double(n);
    }

    template<size_t N>
    double Histogram<N>::mean() const
    {
        return _mean;
    }

    template<size_t N>
    size_t Histogram<N>::at(size_t i) const
    {
        return _data.at(i);
    }

    template<size_t N>
    auto Histogram<N>::begin()
    {
        return _data.begin();
    }

    template<size_t N>
    auto Histogram<N>::begin() const
    {
        return _data.begin();
    }

    template<size_t N>
    auto Histogram<N>::end()
    {
        return _data.end();
    }

    template<size_t N>
    auto Histogram<N>::end() const
    {
        return _data.end();
    }
}