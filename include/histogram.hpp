// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>

#include <unordered_map>

namespace crisp
{
    /// @brief histogram that quantizes range of values into n bins and records them
    /// @param N_Bins: number of bins, 256 by default
    template<size_t N_Bins = 256>
    class Histogram
    {
        public:
            /// @brief default ctor
            Histogram();

            /// @brief create from 1d image
            /// @param image
            template<typename Inner_t>
            Histogram(const Image<Inner_t, 1>&);

            /// @brief create from 1d image
            /// @param image
            template<typename Inner_t>
            void create_from(const Image<Inner_t, 1>&);

            /// @brief access number of elements
            /// @param bin_index: index in [0, N_Bins]
            size_t at(size_t bin_index) const;

            /// @brief get mean intensity
            /// @returns mean as double
            double mean() const;

            /// @brief access begin of data
            /// @returns std::unordered_map<size_t, size_t>::iterator where .first is the bin index, .second is the number of elements
            auto begin();

            /// @brief const-access begin of data
            /// @returns std::unordered_map<size_t, size_t>::iterator where .first is the bin index, .second is the number of elements
            auto begin() const;

            /// @brief access end of data
            /// @returns std::unordered_map<size_t, size_t>::iterator where .first is the bin index, .second is the number of elements
            auto end();

            /// @brief const-access end of data
            /// @returns std::unordered_map<size_t, size_t>::iterator where .first is the bin index, .second is the number of elements
            auto end() const;

            /// @brief transform to N_Bins x N_Bins image
            /// @return grayscale image
            ColorImage to_image() const;

        private:
            double _mean;
            std::unordered_map<size_t, size_t> _data;
    };
}

#include ".src/histogram.inl"