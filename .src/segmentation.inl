// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#include <segmentation.hpp>
#include <vector.hpp>

#include <unordered_map>
#include <deque>

namespace crisp
{
    template<typename Image_t>
    std::vector<ImageSegment> decompose_into_segments(const Image_t& image, size_t min_segment_size)
    {
        using Value_t = typename Image_t::Value_t;

        std::vector<ImageSegment> segments;
        std::unordered_map<size_t, size_t> value_to_index;

        for (size_t y = 1; y < image.get_size().y(); ++y)
            for (size_t x = 1; x < image.get_size().x(); ++x)
            {
                auto value = image(x, y);
                if (value_to_index.find(value.to_hash()) == value_to_index.end())
                {
                    segments.emplace_back();
                    value_to_index.emplace(value.to_hash(), segments.size() - 1);
                }

                segments.at(value_to_index.at(value.to_hash())).insert(Vector2ui{x, y});
            }

        std::vector<ImageSegment> out;
        for (auto& s : segments)
            if (s.size() > min_segment_size)
                out.push_back(std::move(s));

        return out;
    }

    template<typename Image_t>
    std::vector<ImageSegment> decompose_into_connected_segments(const Image_t& image, size_t min_segment_size)
    {
        using Value_t = typename Image_t::Value_t;

        std::vector<ImageSegment> segments;

        BinaryImage seen;
        seen.create(image.get_size().x(), image.get_size().y());
        Value_t color;

        for (size_t y = 0; y < seen.get_size().y(); ++y)
        {
            for (size_t x = 0; x < seen.get_size().x(); ++x)
            {
                if (seen(x, y))
                    continue;

                if (not bool(seen(x, y)))
                {
                    color = image(x, y);
                    segments.emplace_back();

                    std::deque<Vector2ui> to_add;
                    to_add.push_front(Vector2ui{x, y});

                    while (not to_add.empty())
                    {
                        auto current = to_add.front();
                        for (int i : {-1, 1, 0, 0})
                        {
                            for (int j : {0, 0, -1, 1})
                            {
                                if (current.x() + i < 0 or current.x() + i >= seen.get_size().x() or
                                    current.y() + j < 0 or current.y() + j >= seen.get_size().y() or
                                    seen(current.x() + i, current.y() + j))
                                    continue;

                                if (image(current.x() + i, current.y() + j) == color)
                                {
                                    to_add.push_back(Vector2ui{current.x() + i, current.y() + j});
                                    seen(current.x() + i, current.y() + j) = true;
                                }
                            }
                        }

                        segments.back().insert(to_add.front());
                        to_add.pop_front();
                    }

                    if (segments.back().size() < min_segment_size)
                        segments.erase(segments.end() - 1);

                    color = false;
                }
            }
        }

        return segments;
    }

}