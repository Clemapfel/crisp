// 
// Copyright 2021 Clemens Cords
// Created on 12.08.21 by clem (mail@clemens-cords.com)
//

#include ".src/common.inl"

namespace crisp
{
    ColorImage PseudoColor::identity(const GrayScaleImage& image)
    {
        return image.convert_to_color();
    }

    ColorImage PseudoColor::value_range_to_hue_range(float min_gray, float max_gray, float min_hue, float max_hue, const GrayScaleImage& in)
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
        
        auto out = ColorImage();
        out.create(in.get_size().x(), in.get_size().y());
        
        for (long x = 0; x < out.get_size().x(); ++x)
            for (long y = 0; y < out.get_size().y(); ++y)
                out(x, y) = f(in.at(x, y));
            
        return out;
    }

    ColorImage PseudoColor::value_range_to_inverse_hue_range(float min_gray, float max_gray, float min_hue, float max_hue, const GrayScaleImage& in)
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
        
        auto out = ColorImage();
        out.create(in.get_size().x(), in.get_size().y());
        
        for (long x = 0; x < out.get_size().x(); ++x)
            for (long y = 0; y < out.get_size().y(); ++y)
                out(x, y) = f(in.at(x, y));
            
        return out;
    }

    ColorImage PseudoColor::value_ranges_to_hue_ranges(RangeMapping& mapping, const GrayScaleImage& in)
    {
        auto f = [mapping](float x) -> RGB {

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
        
        auto out = ColorImage();
        out.create(in.get_size().x(), in.get_size().y());
        
        for (long x = 0; x < out.get_size().x(); ++x)
            for (long y = 0; y < out.get_size().y(); ++y)
                out(x, y) = f(in.at(x, y));
            
        return out;
    }
    
    ColorImage PseudoColor::value_to_hue(float gray, float hue, const GrayScaleImage& in)
    {
        return value_range_to_hue_range(gray, gray, hue, hue, in);
    }

    ColorImage PseudoColor::value_range_to_hue(float min_gray, float max_gray, float hue, const GrayScaleImage& in)
    {
        return value_range_to_hue_range(min_gray, max_gray, hue, hue, in);
    }

    void PseudoColor::RangeMapping::add_value_to_hue(float gray, float hue)
    {
        _gray_to_hue.push_back({{gray, gray}, {hue, hue}});
    }

    void PseudoColor::RangeMapping::add_value_range_to_hue(float min_gray, float max_gray, float hue)
    {
        _gray_to_hue.push_back({{min_gray, max_gray}, {hue, hue}});
    }

    void PseudoColor::RangeMapping::add_value_range_to_hue_range(
            float min_gray, 
            float max_gray, 
            float min_hue,
            float max_hue)
    {
        _gray_to_hue.push_back({{min_gray, max_gray}, {min_hue, max_hue}});
    }

    void PseudoColor::RangeMapping::add_value_range_to_inverse_hue_range(
            float min_gray, 
            float max_gray,
            float min_hue, 
            float max_hue)
    {
        _gray_to_inverse_hue.push_back({{min_gray, max_gray}, {min_hue, max_hue}});
    }
}