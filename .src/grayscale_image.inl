// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    GrayScaleImage::GrayScaleImage()
        : Image<float, 1>()
    {}

    GrayScaleImage::GrayScaleImage(size_t x, size_t y)
        : Image<float, 1>(x, y)
    {}

    template<typename T>
    GrayScaleImage::GrayScaleImage(const Image<T, 1>& other)
        : Image<float, 1>(other.get_size().x(), other.get_size().y())
    {
        for (size_t x = 0; x < get_size().x(); ++x)
        {
            for (size_t y = 0; y < get_size().y(); ++y)
            {
                at(x, y) = static_cast<float>(other(x, y));
            }
        }
    }
    
    GrayScaleImage::GrayScaleImage(const Eigen::MatrixXf& m)
        : Image<float, 1>(m.rows(), m.cols())
    {
        for (size_t x = 0; x < get_size().x(); ++x)
        {
            for (size_t y = 0; y < get_size().y(); ++y)
            {
                at(x, y) = m(x, y);
            }
        }
    }

    ColorImage GrayScaleImage::convert_to_color() const
    {
        ColorImage out;
        out.create(get_size().x(), get_size().y());

        for (size_t x = 0; x < get_size().x(); ++x)
        {
            for (size_t y = 0; y < get_size().y(); ++y)
            {
                float value = at(x, y);
                out(x, y) = RGB(value, value, value);
            }
        }

        return out;
    }
}