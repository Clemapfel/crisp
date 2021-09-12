// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#include <segmentation.hpp>
#include <vector.hpp>
#include <histogram.hpp>

#include <unordered_map>
#include <deque>
#include <list>

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
    
    template<typename Inner_t>
    BinaryImage manual_threshold(const Image<Inner_t, 1>& image, Inner_t threshold)
    {
        BinaryImage out;
        out.create(image.get_size().x(), image.get_size().y());
        
        for (size_t x = 0; x < image.get_size().x(); ++x)
            for (size_t y = 0; y < image.get_size().y(); ++y)
                if (image(x, y) > threshold)
                    out(x, y) = true;
                else
                    out(x, y) = false;
                
        return out;
    }
    
    template<typename Inner_t>
    BinaryImage basic_threshold(const Image<Inner_t, 1>& image)
    {
        const float convergence_treshold = 1 / 255.f;

        auto histogram = Histogram<256>();
        histogram.create_from(image);

        uint8_t old_threshold = histogram.mean() * 255;
        uint8_t new_threshold = 0;
        float total_sum = 0,
                left_sum = 0,
                right_sum = 0;

        size_t total_n = 0,
                left_n = 0,
                right_n = 0;

        float left_mean, right_mean;

        for (uint8_t i = 0; i < uint8_t(255); ++i)
        {
            float value = histogram.at(i) * (i / 255.f);
            if (i < old_threshold)
            {
                left_n += histogram.at(i);
                left_sum += value;
            }
            else
            {
                right_n += histogram.at(i);
                right_sum += value;
            }
        }

        left_mean = left_sum / left_n;
        right_mean = right_sum / right_n;
        new_threshold = (left_mean + 0.5 * (abs(right_mean - left_mean))) * 255;

        while (std::abs<float>((old_threshold / 255.f) - (new_threshold / 255.f)) > convergence_treshold)
        {
            old_threshold = new_threshold;
            left_n = 0;
            left_sum = 0;
            right_n = 0;
            right_sum = 0;

            for (uint8_t i = 0; i < uint8_t(255); ++i)
            {
                float value = histogram.at(i) * (i / 255.f);
                if (i < old_threshold)
                {
                    left_n += histogram.at(i);
                    left_sum += value;
                }
                else
                {
                    right_n += histogram.at(i);
                    right_sum += value;
                }
            }

            left_mean = left_sum / left_n;
            right_mean = right_sum / right_n;
            new_threshold = (left_mean + 0.5 * (abs(right_mean - left_mean))) * 255;
        }

        auto out = BinaryImage();
        out.create(image.get_size().x(), image.get_size().y());

        for (size_t x = 0; x < image.get_size().x(); ++x)
            for (size_t y = 0; y < image.get_size().y(); ++y)
                out(x, y) = image(x, y) > (new_threshold / 255.f);

        return out;
    }
    
    template<typename Inner_t>
    BinaryImage otsu_threshold(const Image<Inner_t, 1>& image)
    {
        auto histogram = Histogram<256>();
        histogram.create_from(image);

        std::map<uint8_t, std::pair<float, float>> threshold_to_sums;   // k: {cumulative_sum, intensity_sum}
        float mn = image.get_size().x() * image.get_size().y();
        float global_mean = 0;

        for (uint8_t k = 0; k < 255; ++k)
        {
            float cumulative_sum = 0;
            float intensity_sum = 0;
            size_t n = 0;
            for (uint8_t i = 0; i <= k; ++i)
            {
                float p_i = histogram.at(i) / mn;
                cumulative_sum += p_i;
                intensity_sum += p_i * i;
                n += histogram.at(i);

                if (k == 254)
                    global_mean += p_i * i;
            }
            threshold_to_sums.emplace(k, std::make_pair(cumulative_sum, intensity_sum));
        }

        uint8_t max_k = 0;
        float max_sigma = 0;
        for (auto& pair : threshold_to_sums)
        {
            auto p_i = pair.second.first;
            auto local_mean = pair.second.second;
            auto sigma = pow(global_mean * p_i - local_mean, 2) / (p_i * (1 - p_i));

            if (sigma > max_sigma)
            {
                max_sigma = sigma;
                max_k = pair.first;
            }
        }

        float result = max_k / 255.f;

        auto out = BinaryImage();
        out.create(image.get_size().x(), image.get_size().y());

        for (size_t x = 0; x < image.get_size().x(); ++x)
            for (size_t y = 0; y < image.get_size().y(); ++y)
                out(x, y) = image(x, y) > result;

        return out;
    }
    
    template<typename Inner_t>
    BinaryImage variable_threshold(const Image<Inner_t, 1>& image, float tail_length_factor)
    {
        BinaryImage out;
        out.create(image.get_size().x(), image.get_size().y());

        std::list<float> tail;
        float current_sum = 0;

        size_t tail_length = (image.get_size().x() + image.get_size().y()) * tail_length_factor;
        size_t k = 0;

        auto update = [&](size_t x, size_t y, size_t i) 
        {
            out(x, y) = image(x, y) > (current_sum / tail.size());

            if (i > tail_length)
            {
                current_sum -= tail.front();
                tail.erase(tail.begin());
            }

            tail.emplace_back(image(x, y));
            current_sum += tail.back();
        };

        int top = 0,
            bottom = image.get_size().x() - 1,
            left = 0,
            right = image.get_size().y() - 1;

        int direction = 1;

        while (top <= bottom and left <= right)
        {
            if (direction == 1)
            {
                for (int i = left; i <= right; ++i, k++)
                    update(top, i, k);

                ++top;
                direction = 2;
            }

            else if (direction == 2)
            {
                for (int i = top; i <= bottom; ++i, k++)
                    update(i, right, k);

                --right;
                direction = 3;
            }

            else if (direction == 3)
            {
                for (int i = right; i >= left; --i, k++)
                    update(bottom, i, k);

                --bottom;
                direction = 4;
            }
            else if (direction == 4)
            {
                for (int i = bottom; i >= top; --i)
                    update(i, left, k);

                ++left;
                direction = 1;
            }
        }

        return out;
    }
    
    template<typename Inner_t>
    BinaryImage neighborhood_threshold(const Image<Inner_t, 1>& image, size_t neighborhood_size)
    {
        auto out = BinaryImage();
        out.create(image.get_size().x(), image.get_size().y());

        const int spread = 7;
        const int half_spread = spread * 0.5;

        for (size_t x = 0; x < image.get_size().x(); ++x)
        {
            for (size_t y = 0; y < image.get_size().y(); ++y)
            {
                float mean = 0;
                size_t count = 0;
                for (size_t i = 1; i <= spread * neighborhood_size; i += spread)
                {
                    mean += image(x - i, y);
                    mean += image(x + i, y);
                    mean += image(x, y + i);
                    mean += image(x, y - i);

                    mean += image(x - i - half_spread, y - i - half_spread);
                    mean += image(x + i - half_spread, y - i - half_spread);
                    mean += image(x - i - half_spread, y + i - half_spread);
                    mean += image(x + i - half_spread, y + i - half_spread);
                }
                mean /= (8 * neighborhood_size);
                out(x, y) = float(image(x, y)) > mean;
            }
        }

        return out;
    }

    template<typename Image_t>
    Image_t superpixel_clustering(const Image_t& in, size_t n_superpixels)
    {
        const size_t n_pixels = image.get_size().x() * image.get_size().y();
        auto spacing = sqrt(n_pixels / double(n_superpixel));

        using Value_t = typename Image_t::Value_t;
        auto value_distance = [&](const Value_t& a, const Value_t& b) -> float
        {
            float distance = 0;
            for (size_t i = 0; i < Value_t::size(); ++i)
                distance += fabs(a.at(i) - b.at(i));

            distance /= Value_t::size();
        };

        auto xy_distance = [&](const Vector2ui& a, const Vector2ui& b) -> float
        {
            float distance = 0;
            distance += abs<size_t>(a.x() - b.x());
            distance += abs<size_t>(a.y() - b.y());

            return distance / (2 * spacing * spacing);
        };

        struct Cluster
        {
            Vector2f center;
            float color_sum;
            Vector2f xy_sum;
            size_t n;

            float final_color; // unused until post-processing
        };

        std::map<size_t, Cluster> clusters;

        // initialize
        for (size_t i = 0, index = 0; i * spacing < image.get_size().x(); i += spacing)
            for (size_t j = 0; j * spacing < image.get_size().y(); j += spacing, index++)
                clusters.emplace(index, Cluster{i + spacing * 0})

    }
}