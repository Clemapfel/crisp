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















        GrayScaleImage x_gradient;
        x_gradient.create(image.get_size().x(), image.get_size().y());

        using Value_t = typename Image_t::Value_t;

        for (size_t x = 0; x < image.get_size().x(); ++x)
            for (size_t y = 0; y < image.get_size().y(); ++y)
            {
                float sum = 0;
                for (size_t i = 0; i < Value_t::size(); ++i)
                    sum += image(x, y).at(i);

                x_gradient(x, y) = sum / float(Value_t::size());
            }

        auto y_gradient = x_gradient;

        SpatialFilter filter;

        Kernel x_right;
        x_right.resize(3, 1);
        x_right << 1, 0, -1;
        filter.set_kernel(x_right);
        filter.apply_to(x_gradient);

        Kernel x_left;
        x_left.resize(1, 3);
        x_left << 1, 2, 1;
        filter.set_kernel(x_left);
        filter.apply_to(x_gradient);

        Kernel y_right;
        y_right.resize(3, 1);
        y_right << 1, 2, 3;
        filter.set_kernel(y_right);
        filter.apply_to(y_gradient);

        Kernel y_left;
        y_left.resize(1, 3);
        y_left << 1, 0, -1;
        filter.set_kernel(y_left);
        filter.apply_to(y_gradient);

        float min = std::numeric_limits<float>::max(), max = -1;

        for (size_t x = 0; x < image.get_size().x(); ++x)
            for (size_t y = 0; y < image.get_size().y(); ++y)
            {
                float x_val = x_gradient(x, y) / 4.f;   // normalizes
                float y_val = y_gradient(x, y) / 4.f;
                x_gradient(x, y) = float(sqrt(x_val * x_val + y_val * y_val) / sqrt(2));

                min = std::min<float>(x_gradient(x, y), min);
                max = std::max<float>(x_gradient(x, y), max);
            }

        //for (auto& px : x_gradient)
          //  px = px / max;

        std::cout << "min: " << min << "  max: " << max << std::endl;

        return x_gradient;
    }

}