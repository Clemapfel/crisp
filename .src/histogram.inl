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
    template<typename Range_t>
    Histogram<N>::Histogram(const Range_t& image)
        : Histogram()
    {
        create_from<Range_t>(image);
    }

    template<size_t N>
    template<typename Range_t>
    void Histogram<N>::create_from(const Range_t& image)
    {
        for (auto& pair : _data)
            pair.second = 0;

        size_t n = 0;
        float sum = 0;
        for (const auto& it : image)
        {
            float value = float(it);
            value = clamp<float>(0, 1, value);
            sum += value;

            value *= N;
            value = floor(value);
            _data.at(size_t(value)) += 1;

            n += 1;
        }

        _mean = double(sum) / double(n);
        _n_sum = n;
    }

    template<size_t N>
    double Histogram<N>::mean() const
    {
        return _mean;
    }

    template<size_t N>
    size_t Histogram<N>::at(size_t bin_index) const
    {
        return _data.at(bin_index);
    }

    template<size_t N>
    size_t Histogram<N>::get_n_occurrences(float intensity) const
    {
        return _data.at(floor(intensity * N));
    }

    template<size_t N>
    size_t Histogram<N>::get_n_total() const
    {
        return _n_sum;
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
    ColorImage Histogram<N>::as_image() const
    {
        ColorImage out;
        out.create(N+1, N+1);

        size_t max = 0;
        for (const auto& pair : _data)
            max = std::max(max, pair.second);

        float step = (0.85f * N) / float(max);

        for (size_t x = 0; x <= N; ++x)
        {
            if (x == floor(_mean * N))
                for (size_t y = 0; y < out.get_size().y(); ++y)
                    out(x, y) = RGB{0.5, 0, 0};

            for (size_t y = 1; y < _data.at(x) * step; ++y)
            {
                out(x, out.get_size().y() - y) = RGB{1, 1, 1};
            }
        }

        return out;
    }
}