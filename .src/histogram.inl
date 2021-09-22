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
        for (size_t i = 0; i <= N; ++i)
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
            value = floor(value);
            _data.at(size_t(value)) += 1;

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

    template<size_t N>
    ColorImage Histogram<N>::to_image() const
    {
        ColorImage out;
        out.create(N+1, N+1);

        float n = 0;
        float mean = 0;
        size_t max = 0;
        for (const auto& pair : _data)
        {
            max = std::max(max, pair.second);
            mean += pair.first * pair.second;
            n += pair.second;
        }

        mean /= n;

        size_t step = ceil(float(max) / (0.85 * N));

        for (size_t x = 0; x <= N; ++x)
        {
            if (x == mean)
                for (size_t y = 0; y < out.get_size().y(); ++y)
                    out(x, y) = RGB{1, 0, 0};

            for (size_t y = 1; y * step < _data.at(x); ++y)
            {
                out(x, out.get_size().y() - y) = RGB{1, 1, 1};
            }
        }

        return out;
    }
}