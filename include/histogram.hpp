// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>

#include <unordered_map>

namespace crisp
{
    template<size_t N_Bins>
    class Histogram
    {
        public:
            Histogram();

            template<typename Inner_t>
            Histogram(const Image<Inner_t, 1>&);

            template<typename Inner_t>
            void create_from(const Image<Inner_t, 1>&);

            size_t at(size_t bin_index) const;

            double mean() const;

            auto begin();
            auto begin() const;

            auto end();
            auto end() const;

        private:
            double _mean;
            std::unordered_map<size_t, size_t> _data;
    };
}

#include ".src/histogram.inl"