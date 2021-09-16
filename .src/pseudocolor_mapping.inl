// 
// Copyright 2021 Clemens Cords
// Created on 12.08.21 by clem (mail@clemens-cords.com)
//

#include ".src/common.inl"

namespace crisp
{
    ColorImage PseudoColorMapping::transform(const GrayScaleImage& grayscale)
    {
        ColorImage out;
        out.create(grayscale.get_size().x(), grayscale.get_size().y(), RGB(0,0,0));

        for (long x = 0; x < grayscale.get_size().x(); ++x)
            for (long y = 0; y < grayscale.get_size().y(); ++y)
                out(x, y) = _function(grayscale(x, y));

        return out;
    }

    void PseudoColorMapping::set_function(std::function<RGB(float)>&& mapping)
    {
        _function = mapping;
    }

    auto && PseudoColorMapping::identity()
    {
        static auto f = [](float x) -> RGB {return RGB(x);};
        return std::move(f);
    }

    auto && PseudoColorMapping::value_range_to_hue_range(float min_gray, float max_gray, float min_hue, float max_hue)
    {
        auto f = [min_gray, max_gray, min_hue, max_hue](float x) -> RGB {

            assert(min_gray <= max_gray and min_hue <= max_hue);

            if (x < min_gray or x > max_gray)
                return RGB(x);
            else
            {
                float hue = x;
                hue -= min_gray;
                hue /= abs(max_gray - min_gray);

                hue = project(min_hue, max_hue, hue);

                return HSV{hue, 1, 1}.to_rgb();
            }
        };

        return std::move(f);
    }

    auto && PseudoColorMapping::value_range_to_inverse_hue_range(float min_gray, float max_gray, float min_hue, float max_hue)
    {
        auto f = [min_gray, max_gray, min_hue, max_hue](float x) -> RGB {

            assert(min_gray <= max_gray);

            if (x < min_gray or x > max_gray)
                return RGB(x);
            else
            {
                float hue = x;
                hue -= min_gray;
                hue /= abs(max_gray - min_gray);

                hue = project(min_hue, max_hue, hue);
                hue = max_hue - (hue - min_hue);

                return HSV{hue, 1, 1}.to_rgb();
            }
        };

        return std::move(f);
    }

    auto&& PseudoColorMapping::value_ranges_to_hue_ranges(PseudoColorMapping::RangeMapping& mapping)
    {
        static auto f = [mapping](float x) -> RGB {

            for (auto& map : mapping._gray_to_hue)
            {
                float min_gray = map.first.first;
                float max_gray = map.first.second;
                float min_hue = map.second.first;
                float max_hue = map.second.second;

                if (x >= min_gray and x <= max_gray)
                {
                    float hue = x;
                    hue -= min_gray;
                    hue /= abs(max_gray - min_gray);

                    hue = project(min_hue, max_hue, hue);

                    return HSV{hue, 1, 1}.to_rgb();
                }
            }

            for (auto& map : mapping._gray_to_inverse_hue)
            {
                float min_gray = map.first.first;
                float max_gray = map.first.second;
                float min_hue = map.second.first;
                float max_hue = map.second.second;

                if (x >= min_gray and x <= max_gray)
                {
                    float hue = x;
                    hue -= min_gray;
                    hue /= abs(max_gray - min_gray);

                    hue = project(min_hue, max_hue, hue);
                    hue = max_hue - (hue - min_hue);

                    return HSV{hue, 1, 1}.to_rgb();
                }
            }

            return RGB(x);
        };

        return std::move(f);
    }
    
    auto && PseudoColorMapping::value_to_hue(float gray, float hue)
    {
        return value_range_to_hue_range(gray, gray, hue, hue);
    }

    auto && PseudoColorMapping::value_range_to_hue(float min_gray, float max_gray, float hue)
    {
        return value_range_to_hue_range(min_gray, max_gray, hue, hue);
    }

    void PseudoColorMapping::RangeMapping::add_value_to_hue(float gray, float hue)
    {
        _gray_to_hue.push_back({{gray, gray}, {hue, hue}});
    }

    void PseudoColorMapping::RangeMapping::add_value_range_to_hue(float min_gray, float max_gray, float hue)
    {
        _gray_to_hue.push_back({{min_gray, max_gray}, {hue, hue}});
    }

    void PseudoColorMapping::RangeMapping::add_value_range_to_hue_range(float min_gray, float max_gray, float min_hue,
                                                                        float max_hue)
    {
        _gray_to_hue.push_back({{min_gray, max_gray}, {min_hue, max_hue}});
    }

    void PseudoColorMapping::RangeMapping::add_value_range_to_inverse_hue_range(float min_gray, float max_gray,
                                                                                float min_hue, float max_hue)
    {
        _gray_to_inverse_hue.push_back({{min_gray, max_gray}, {min_hue, max_hue}});
    }
}