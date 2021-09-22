// 
// Copyright 2021 Clemens Cords
// Created on 13.09.21 by clem (mail@clemens-cords.com)
//

#include <spatial_filter.hpp>

namespace crisp
{
    template<typename Image_t>
    void normalize(Image_t& image)
    {
        using Value_t = typename Image_t::Value_t;
        using Inner_t = typename Image_t::Value_t::Value_t;

        float min = std::numeric_limits<Inner_t>::max();
        float max = std::numeric_limits<Inner_t>::min();

        for (const auto& px : image)
        {
            for (size_t i = 0; i < Value_t::size(); ++i)
            {
                min = std::min<Inner_t>(min, px.at(i));
                max = std::max<Inner_t>(max, px.at(i));
            }
        }

        for (auto& px : image)
        {
            for (size_t i = 0; i < Value_t::size(); ++i)
                px.at(i) = (px.at(i) - min) / (max - min);
        }
    }

    template<typename Image_t>
    GrayScaleImage compute_gradient_magnitude(const Image_t& image)
    {
        using Value_t = typename Image_t::Value_t;

        GrayScaleImage as_grayscale;
        as_grayscale.create(image.get_size().x(), image.get_size().y());

        for (size_t y = 0; y < image.get_size().y(); ++y)
            for (size_t x = 0; x < image.get_size().x(); ++x)
            {
                float image_value = 0;
                for (size_t i = 0; i < Value_t::size(); ++i)
                    image_value += float(image(x, y).at(i));

                as_grayscale(x, y) = image_value / float(Value_t::size());
            }

        GrayScaleImage out;
        out.create(image.get_size().x(), image.get_size().y());

        Kernel x_gradient = SpatialFilter::sobel_gradient_x();
        Kernel y_gradient = SpatialFilter::sobel_gradient_y();

        for (size_t y = 0; y < image.get_size().y(); ++y)
            for (size_t x = 0; x < image.get_size().x(); ++x)
            {
                float gx = 0;
                float gy = 0;

                for (size_t i = 0; i < 3; ++i)
                    for (size_t j = 0; j < 3; ++j)
                    {
                        gx += x_gradient(i, j) * float(as_grayscale(x - (i - 1), y - (j - 1)));
                        gy += y_gradient(i, j) * float(as_grayscale(x - (i - 1), y - (j - 1)));
                    }

                gx /= 4;
                gy /= 4;

                out(x, y) = sqrtf(gx * gx + gy * gy) / sqrtf(2);
            }

        return out;
    }

    template<typename T>
    std::array<Image<bool, 1>, 8> bitplane_decompose(const Image<T, 1>& image)
    {
        std::array<Image<bool, 1>, 8> out;

        for (size_t i = 0; i < 8; ++i)
            out.at(i).create(image.get_size().x(), image.get_size().y());

        for (size_t x = 0; x < image.get_size().x(); ++x)
        {
            for (size_t y = 0; y < image.get_size().y(); ++y)
            {
                auto as_char = static_cast<uint8_t>(clamp(0.f, 1.f, static_cast<float>(image(x, y))) * 255);

                for (size_t i = 0; i < 8; ++i)
                    out.at(i)(x, y) = static_cast<bool>((as_char >> i) & 1);
            }
        }

        return out;
    }

    template<typename T>
    Image<T, 1> bitplane_assemble(const std::array<Image<bool, 1>, 8>& in)
    {
        Image<T, 1> out;
        out.create(in.at(0).get_size().x(), in.at(0).get_size().y());

        for (size_t x = 0; x < out.get_size().x(); ++x)
        {
            for (size_t y = 0; y < out.get_size().y(); ++y)
            {
                auto as_char = 0;

                for (size_t i = 0; i < 8; ++i)
                    as_char |= uint8_t(1) << i;

                T as_t = static_cast<T>(as_char) / T(255);

                out(x, y) = as_t;
            }
        }

        return out;
    }

    template<typename Image_t>
    void histogram_equalize(Image_t& in)
    {
        using Value_t = typename Image_t::Value_t;
        for (size_t i = 0; i < Value_t::size(); ++i)
        {
            auto histogram = Histogram<255>(in.get_nths_plane(i));


        }
    }

}