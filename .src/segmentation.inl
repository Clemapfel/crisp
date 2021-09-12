// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#include <segmentation.hpp>
#include <vector.hpp>

#include <unordered_map>

namespace crisp
{
    template<typename Image_t>
    std::vector<ImageSegment> decompose_into_segments(const Image_t& image, size_t min_region_size)
    {
        using Value_t = typename Image_t::Value_t;

        std::vector<ImageSegment> out;
        std::unordered_map<size_t, size_t> value_to_index;

        for (size_t y = 1; y < image.get_size().y(); ++y)
            for (size_t x = 1; x < image.get_size().x(); ++x)
            {
                auto value = image(x, y);
                if (value_to_index.find(value.to_hash()) == value_to_index.end())
                {
                    out.emplace_back();
                    value_to_index.emplace(value.to_hash(), out.size() - 1);
                }

                out.at(value_to_index.at(value.to_hash())).insert(Vector2ui{x, y});
            }

        return out;
    }
}