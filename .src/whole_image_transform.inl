// 
// Copyright 2021 Clemens Cords
// Created on 13.09.21 by clem (mail@clemens-cords.com)
//

#include <whole_image_transform.hpp>
#include <spatial_filter.hpp>

namespace crisp
{
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

}