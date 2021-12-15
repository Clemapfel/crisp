// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#include <vector.hpp>
#include <histogram.hpp>
#include <whole_image_transform.hpp>

#include <map>
#include <iostream>
#include <deque>
#include <list>

namespace crisp::Segmentation
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
                out.push_back(s);

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
                out(x, y) = image(x, y) > threshold;

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
                out(x, y) = float(image(x, y)) < mean;
            }
        }

        return out;
    }

    template<typename T, size_t N>
    Texture<T, N> threshold(const Texture<T, N>& texture, size_t neighborhood_size, size_t correction)
    {
        assert(neighborhood_size > 0);

        auto out = texture;

        auto shader = gl::State::register_shader("local_threshold.glsl");
        auto program = gl::State::register_program(shader);
        gl::State::bind_shader_program(program);
        gl::State::free_shader(shader);

        gl::State::set_uint(program, "_neighborhood_size", neighborhood_size);
        gl::State::set_uint(program, "_correction", correction);

        auto& workspace = out.get_workspace();
        workspace.display();
        workspace.yield();

        return out;
    }

    template<typename Image_t>
    Image_t superpixel_clustering(const Image_t& image, size_t n_superpixels, size_t max_n_iterations)
    {
        const size_t n_pixels = image.get_size().x() * image.get_size().y();
        auto spacing = sqrt(n_pixels / double(n_superpixels));

        using Value_t = typename Image_t::Value_t;
        auto value_distance = [&](const Value_t& a, const Value_t& b) -> float
        {
            float distance = 0;
            for (size_t i = 0; i < Value_t::size(); ++i)
                distance += fabs(a.at(i) - b.at(i));

            return distance;
        };

        auto xy_distance = [&](const auto& a, const auto& b) -> float
        {
            float distance = 0;
            distance += fabs(a.x() - b.x());
            distance += fabs(a.y() - b.y());

            return distance / (2 * spacing * spacing);
        };

        struct Cluster
        {
            const Vector2f initial_center;
            Vector2f center;
            Value_t color_sum;
            Vector2f xy_sum;
            int n;
        };

        std::unordered_map<size_t, Cluster> clusters;

        auto distance = [&](const Vector2i& pos, size_t cluster_i)
        {
            if (cluster_i == -1)
                return std::numeric_limits<float>::max();

            auto& cluster = clusters.at(cluster_i);
            auto cluster_xy = cluster.xy_sum;
            cluster_xy = cluster_xy / float(cluster.n);

            auto cluster_v = cluster.color_sum;
            cluster_v = cluster_v / float(cluster.n);

            return xy_distance(pos, cluster_xy) + 2 * value_distance(image(pos.x(), pos.y()), cluster_v);
        };

        auto out = image;
        auto gradient_img = compute_gradient_magnitude(image);

        float per_row = ceil(sqrt(n_superpixels));
        spacing = std::max(image.get_size().x() / per_row, image.get_size().y() / per_row);
        spacing = ceil(spacing);

        // initialize
        size_t n_total = 0;
        for (size_t ii = 0, index = 0; ii < per_row; ii++)
        {
            for (size_t jj = 0; jj < per_row; jj++, index++)
            {
                auto i = ii * spacing;
                auto j = jj * spacing;

                auto center = Vector2f{float(i + spacing * 0.5), float(j + spacing * 0.5)};
                auto cluster = Cluster{center, center, image(center.x(), center.y()), center, 1};

                size_t min_x = cluster.center.x(), min_y = cluster.center.y();
                float min_gradient_value = std::numeric_limits<float>::max();

                for (size_t x = i; x < i + spacing; ++x)
                {
                    for (size_t y = j; y < j + spacing; ++y)
                    {
                        if (x < 0 or x >= image.get_size().x() or y < 0 or y >= image.get_size().y())
                            continue;

                        out(x, y).at(0) = index;
                        cluster.xy_sum += Vector2f{float(x), float(y)};
                        cluster.color_sum += image(x, y);
                        cluster.n += 1;
                        n_total += 1;

                        float gradient = gradient_img(x, y);
                        if (gradient < min_gradient_value)
                        {
                            min_x = x;
                            min_y = y;

                            min_gradient_value = gradient;
                        }
                    }
                }

                cluster.center = Vector2f{float(min_x), float(min_y)};
                clusters.insert(std::make_pair(index, std::move(cluster)));
            }
        }

        assert(n_total == n_pixels);

        size_t n_changed = n_pixels;
        size_t n_iterations = 0;
        while (n_changed > 0 and n_iterations < max_n_iterations)
        {
            n_changed = 0;
            for (auto& pair : clusters)
            {
                auto& cluster = pair.second;
                auto cluster_i = pair.first;

                const float offset = 0.75 * spacing;
                int x_range_min = std::min(cluster.initial_center.x(), cluster.center.x()) - offset;
                int x_range_max = std::max(cluster.initial_center.x(), cluster.center.x()) + offset;
                int y_range_min = std::min(cluster.initial_center.y(), cluster.center.y()) - offset;
                int y_range_max = std::max(cluster.initial_center.y(), cluster.center.y()) + offset;

                for (int y = y_range_min; y < y_range_max; ++y)
                {
                    for (int x = x_range_min; x < x_range_max; ++x)
                    {
                        if (x < 0 or x >= image.get_size().x() or y < 0 or y >= image.get_size().y())
                            continue;

                        size_t old_i = out(x, y).at(0);

                        if (old_i == cluster_i)
                            continue;

                        float old_dist = distance(Vector2i{x, y}, old_i);
                        float new_dist = distance(Vector2i{x, y}, cluster_i);

                        if (new_dist < old_dist)
                        {
                            n_changed += 1;
                            out(x, y).at(0) = float(cluster_i);

                            cluster.color_sum += image(x, y);
                            cluster.xy_sum += Vector2f{float(x), float(y)};
                            cluster.n += 1;

                            auto& old_cluster = clusters.at(old_i);
                            if (old_cluster.n > 0)
                            {
                                old_cluster.color_sum -= image(x, y);
                                old_cluster.xy_sum -= Vector2f{float(x), float(y)};
                                old_cluster.n -= 1;
                            }
                        }
                    }
                }
            }

            n_iterations++;
        }

        // post process
        std::vector<Value_t> final_colors;
        final_colors.resize(clusters.size());

        for (auto& c : clusters)
        {
            if (c.second.n < 0)
                c.second.n = 1;

            auto color = c.second.color_sum;
            color = color / float(c.second.n);
            final_colors.at(c.first) = color;
        }

        // merge
        size_t n_merged = 0;
        for (size_t i = 0; i < final_colors.size(); ++i)
        {
            for (size_t j = 0; j < final_colors.size(); ++j)
            {
                if (i == j)
                    continue;

                if (final_colors.at(i) == final_colors.at(j))
                    continue;

                if (value_distance(final_colors.at(i), final_colors.at(j)) < 0.01)
                {
                    n_merged++;
                    Value_t new_color = final_colors.at(i);
                    new_color += final_colors.at(j);

                    new_color /= 2.f;
                    final_colors.at(i) = new_color;
                    final_colors.at(j) = new_color;
                }
            }
        }

        for (auto& px : out)
            px = final_colors.at(px.at(0));

        return out;
    }

    template<typename Image_t>
    Image_t k_means_clustering(const Image_t& image, size_t n_clusters, size_t max_n_iterations)
    {
        assert(false && "k-means clusterin in n dimensions is not currently supportly. Please convert you image to color or grayscale");
    }

    template<>
    GrayScaleImage k_means_clustering<GrayScaleImage>(const GrayScaleImage& image, size_t n_clusters, size_t max_n_iterations)
    {
        struct Cluster
        {
           float color_sum;
           size_t n;
           float mean_color;
        };

        std::vector<Cluster> clusters;
        std::map<int, size_t> intensity_histogram;

        for (long x = 0; x < image.get_size().x(); ++x)
        {
            for (long y = 0; y < image.get_size().y(); ++y)
            {
                int value = int(image(x, y) * 255.f);
                if (intensity_histogram.find(value) == intensity_histogram.end())
                    intensity_histogram.emplace(value, 1);
                else
                    intensity_histogram.at(value) += 1;
            }
        }

        std::vector<std::pair<int, size_t>> as_vector;
        for (auto& pair : intensity_histogram)
            as_vector.emplace_back(pair.first, pair.second);

        int interval = int(as_vector.size() / float(n_clusters));
        for (size_t i = 0; i < n_clusters; ++i)
        {
            int max_intensity = i * interval;
            size_t max_intensity_n = 0;

            for (int j = i * interval; j < as_vector.size() and j < (i + 1) * interval; ++j)
            {
                if (as_vector.at(j).second > max_intensity_n)
                {
                    max_intensity = as_vector.at(j).first;
                    max_intensity_n = as_vector.at(j).second;
                }
            }

            clusters.emplace_back(Cluster{max_intensity / 255.f, 1, max_intensity / 255.f});
        }

        auto dist = [](float a, float b) -> int
        {
            return abs(int(a * 255) - int(b * 255));
        };

        GrayScaleImage out;
        out.create(image.get_size().x(), image.get_size().y());

        size_t n_changed = 1;
        size_t n_iterations = 0;
        while (n_changed != 0)
        {
            n_changed = 0;
            for (long x = 0; x < image.get_size().x(); ++x)
            {
                for (long y = 0; y < image.get_size().y(); ++y)
                {
                    float min_distance = std::numeric_limits<float>::infinity();
                    size_t min_cluster_i = -1;

                    for (size_t i = 0; i < clusters.size(); ++i)
                    {
                        auto current_distance = dist(image(x, y), clusters.at(i).mean_color);
                        if (current_distance < min_distance)
                        {
                            min_distance = current_distance;
                            min_cluster_i = i;
                        }
                    }

                    int old_i = int(out(x, y));

                    if (old_i < 0 or old_i >= clusters.size())
                    {
                        out(x, y) = min_cluster_i;
                        clusters.at(min_cluster_i).n += 1;
                        clusters.at(min_cluster_i).color_sum += image(x, y);

                        if (n_iterations > 0)
                        {
                        clusters.at(min_cluster_i).mean_color = clusters.at(min_cluster_i).color_sum;
                        clusters.at(min_cluster_i).mean_color /= clusters.at(min_cluster_i).n;
                        }

                        n_changed++;
                    }
                    else if (out(x, y) != float(min_cluster_i))
                    {
                        auto before_dist = dist(clusters.at(old_i).mean_color, clusters.at(min_cluster_i).mean_color);

                        clusters.at(old_i).n -= 1;
                        clusters.at(old_i).color_sum -= image(x, y);
                        clusters.at(old_i).mean_color = clusters.at(old_i).color_sum;
                        clusters.at(old_i).mean_color /= clusters.at(old_i).n;

                        out(x, y) = min_cluster_i;

                        clusters.at(min_cluster_i).n += 1;
                        clusters.at(min_cluster_i).color_sum += image(x, y);

                        if (n_iterations > 0)
                        {
                            clusters.at(min_cluster_i).mean_color = clusters.at(min_cluster_i).color_sum;
                            clusters.at(min_cluster_i).mean_color /= clusters.at(min_cluster_i).n;
                        }

                        auto after_dist = dist(clusters.at(old_i).mean_color, clusters.at(min_cluster_i).mean_color);

                        if (abs(after_dist - before_dist) > 0)
                            n_changed++;
                    }
                }
            }

            if (n_iterations == 0)
            {
                for (auto& cluster : clusters)
                    cluster.mean_color = cluster.color_sum / cluster.n;
            }

            n_iterations++;
        }

        for (size_t i = 0; i < clusters.size(); ++i)
        {
            for (size_t j = 0; j < clusters.size(); ++j)
            {
                if (i == j)
                    continue;

                // pseudo merge
                if (dist(clusters.at(i).mean_color, clusters.at(j).mean_color) < 1)
                {
                    size_t new_n = clusters.at(i).n + clusters.at(j).n;
                    float new_sum = clusters.at(i).color_sum + clusters.at(j).color_sum;
                    clusters.at(i).color_sum = new_sum;
                    clusters.at(i).n = new_n;
                    clusters.at(i).mean_color = new_sum;
                    clusters.at(i).mean_color /= clusters.at(i).n;

                    clusters.at(j).color_sum = new_sum;
                    clusters.at(j).n = new_n;
                    clusters.at(j).mean_color = new_sum;
                    clusters.at(j).mean_color /= clusters.at(j).n;
                }
            }
        }

        for (auto& px : out)
        {
            px = clusters.at(px).mean_color;
        }

        return out;
    }

    template<>
    ColorImage k_means_clustering<ColorImage>(const ColorImage& image, size_t n_clusters, size_t max_n_iterations)
    {
         // initial clusters
        struct Cluster
        {
           RGB color_sum;
           size_t n;

           RGB mean_color;
        };

        std::vector<Cluster> clusters;

        struct HueElement
        {
            size_t n;
            float saturation_sum;
            float value_sum;
        };

        // heuristic to pick initial cluster centers
        std::vector<HueElement> hue_histogram;
        std::vector<size_t> gray_histogram;

        for (size_t i = 0; i < 256; ++i)
        {
            hue_histogram.emplace_back(HueElement{0, 0});
            gray_histogram.emplace_back(0);
        }

        for (long x = 0; x < image.get_size().x(); ++x)
        for (long y = 0; y < image.get_size().y(); ++y)
        {
            auto hsv = image(x, y).to_hsv();

            if (hsv.saturation() < 0.33)
                gray_histogram.at(int(hsv.value() * 255)) += 1;
            else
            {
                auto& element = hue_histogram.at(int(hsv.hue() * 255));
                element.n += 1;
                element.saturation_sum += hsv.saturation();
                element.value_sum += hsv.value();
            }
        }

        // try to find centrum in hue band
        int interval = int(255 / (n_clusters));
        std::vector<size_t> gray_clusters;

        for (size_t i = 0; i < n_clusters; ++i)
        {
            int max_hue = i * interval;
            size_t max_hue_n = 0;
            float s_sum = 0;
            float n_sum = 0;
            float v_sum = 0;

            for (size_t hue = i * interval; hue < hue_histogram.size() and hue < (i+1) * interval; ++hue)
            {
                if (hue_histogram.at(hue).n > max_hue_n)
                {
                    max_hue = hue;
                    max_hue_n = hue_histogram.at(hue).n;
                }

                s_sum += hue_histogram.at(hue).saturation_sum;
                v_sum += hue_histogram.at(hue).value_sum;
                n_sum += hue_histogram.at(hue).n;
            }

            if (n_sum == 0 or s_sum / n_sum < 0.33 or v_sum / n_sum < 0.25)
                gray_clusters.push_back(i);
            else
            {
                auto hsv = HSV{max_hue / 255.f, s_sum / n_sum, v_sum / n_sum};
                auto color = hsv.to_rgb();
                clusters.emplace_back(Cluster{color, 1, color});
            }
        }

        if (gray_clusters.size() > 0)
        {
            interval = int(255 / gray_clusters.size());
            for (size_t i = 0; i < gray_clusters.size(); ++i)
            {
                int max_gray = 0;
                size_t max_gray_n = 0;

                for (size_t gray = i * interval; gray < gray_histogram.size() and gray < (i + 1) * interval; ++gray)
                {
                    if (gray_histogram.at(gray) > max_gray_n)
                    {
                        max_gray = gray;
                        max_gray_n = gray_histogram.at(gray);
                    }
                }

                auto color = RGB(max_gray / 255.f, max_gray / 255.f, max_gray / 255.f);
                clusters.emplace_back(Cluster{color, 1, color});
            }
        }

        auto dist = [](RGB a, RGB b) -> int
        {
            auto a_hsv = a.to_hsv(), b_hsv = b.to_hsv();

            int score = abs(int(a.red() * 255) - int(b.red() * 255)) +
                        abs(int(a.green() * 255) - int(b.green() * 255)) +
                        abs(int(a.blue() * 255) - int(b.blue() * 255));

            return score;
        };

        ColorImage out; // .r is cluster index
        out.create(image.get_size().x(), image.get_size().y(), RGB(-1, 0, 0));

        size_t n_changed = 1;
        size_t n_iterations = 0;
        while (n_changed != 0 and n_iterations < max_n_iterations)
        {
            n_changed = 0;
            for (long x = 0; x < image.get_size().x(); ++x)
            {
                for (long y = 0; y < image.get_size().y(); ++y)
                {
                    int min_distance = std::numeric_limits<int>::max();
                    size_t min_cluster_i = -1;

                    for (size_t i = 0; i < clusters.size(); ++i)
                    {
                        auto current_distance = dist(image(x, y), clusters.at(i).mean_color);
                        if (current_distance < min_distance)
                        {
                            min_distance = current_distance;
                            min_cluster_i = i;
                        }
                    }

                    int old_i = int(out(x, y).x());

                    if (old_i < 0 or old_i >= clusters.size())
                    {
                        out(x, y) = RGB(min_cluster_i);
                        clusters.at(min_cluster_i).n += 1;
                        clusters.at(min_cluster_i).color_sum += image(x, y);

                        if (n_iterations > 0)
                        {
                            clusters.at(min_cluster_i).mean_color = clusters.at(min_cluster_i).color_sum;
                            clusters.at(min_cluster_i).mean_color /= clusters.at(min_cluster_i).n;
                        }

                        n_changed++;
                    }
                    else if (out(x, y).x() != min_cluster_i)
                    {
                        auto before_dist = dist(clusters.at(old_i).mean_color, clusters.at(min_cluster_i).mean_color);

                        clusters.at(old_i).n -= 1;
                        clusters.at(old_i).color_sum -= image(x, y);

                        if (n_iterations >= 0)
                        {
                            clusters.at(old_i).mean_color = clusters.at(old_i).color_sum;
                            clusters.at(old_i).mean_color /= clusters.at(old_i).n;
                        }

                        out(x, y) = RGB(min_cluster_i);

                        clusters.at(min_cluster_i).n += 1;
                        clusters.at(min_cluster_i).color_sum += image(x, y);

                        if (n_iterations >= 0)
                        {
                            clusters.at(min_cluster_i).mean_color = clusters.at(min_cluster_i).color_sum;
                            clusters.at(min_cluster_i).mean_color /= clusters.at(min_cluster_i).n;
                        }

                        auto after_dist = dist(clusters.at(old_i).mean_color, clusters.at(min_cluster_i).mean_color);

                        if (abs(after_dist - before_dist) > 0)
                            n_changed++;
                    }
                }
            }

            if (n_iterations == 0)
            {
                for (auto& cluster : clusters)
                {
                    cluster.mean_color = cluster.color_sum;
                    cluster.mean_color /= cluster.n;
                }
            }

            n_iterations++;
        }

        for (size_t i = 0; i < clusters.size(); ++i)
        {
            for (size_t j = 0; j < clusters.size(); ++j)
            {
                if (i == j)
                    continue;

                // pseudo merge
                if (dist(clusters.at(i).mean_color, clusters.at(j).mean_color) < 1)
                {
                    size_t new_n = clusters.at(i).n + clusters.at(j).n;
                    RGB new_sum = clusters.at(i).color_sum;
                    new_sum += clusters.at(j).color_sum;

                    clusters.at(i).color_sum = new_sum;
                    clusters.at(i).n = new_n;
                    clusters.at(i).mean_color = new_sum;
                    clusters.at(i).mean_color /= clusters.at(i).n;

                    clusters.at(j).color_sum = new_sum;
                    clusters.at(j).n = new_n;
                    clusters.at(j).mean_color = new_sum;
                    clusters.at(j).mean_color /= clusters.at(j).n;
                }
            }
        }

        for (auto& px : out)
        {
            px = clusters.at(px.x()).mean_color;
        }

        return out;
    }
}