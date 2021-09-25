// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    Image<float, 1> ColorImage::get_red_plane()
    {
        return get_nths_plane(0);
    }

    void ColorImage::set_red_plane(const Image<float, 1>& plane)
    {
        assert(plane.get_size().x() == get_size().x() and plane.get_size().y() == get_size().y());
        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                operator()(x, y).red() = plane(x, y);
    }

    Image<float, 1> ColorImage::get_blue_plane()
    {
        return get_nths_plane(1);
    }

    void ColorImage::set_blue_plane(const Image<float, 1>& plane)
    {
        assert(plane.get_size().x() == get_size().x() and plane.get_size().y() == get_size().y());
        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                operator()(x, y).blue() = plane(x, y);
    }

    Image<float, 1> ColorImage::get_green_plane()
    {
        return get_nths_plane(2);
    }

    void ColorImage::set_green_plane(const Image<float, 1>& plane)
    {
        assert(plane.get_size().x() == get_size().x() and plane.get_size().y() == get_size().y());
        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                operator()(x, y).green() = plane(x, y);
    }

    Image<float, 1> ColorImage::get_hue_plane()
    {
        Image<float, 1> out;
        out.create(_data.rows(), _data.cols());

        for (long y = 0; y < _data.cols(); ++y)
        {
            for (long x = 0; x < _data.cols(); ++x)
            {
                RGB as_rgb = _data(x, y);
                out(x, y) = as_rgb.to_hsv().hue();
            }
        }

        return out;
    }

    void ColorImage::set_hue_plane(const Image<float, 1>& plane)
    {
        assert(plane.get_size().x() == get_size().x() and plane.get_size().y() == get_size().y());
        for (size_t x = 0; x < get_size().x(); ++x)
        {
            for (size_t y = 0; y < get_size().y(); ++y)
            {
                auto value = operator()(x, y).to_hsv();
                value.hue() = plane(x, y);
                operator()(x, y) = value.to_rgb();
            }
        }
    }

    Image<float, 1> ColorImage::get_saturation_plane()
    {
        Image<float, 1> out;
        out.create(_data.rows(), _data.cols());

        for (long y = 0; y < _data.cols(); ++y)
        {
            for (long x = 0; x < _data.cols(); ++x)
            {
                RGB as_rgb = _data(x, y);
                out(x, y) = as_rgb.to_hsv().saturation();
            }
        }

        return out;
    }

    void ColorImage::set_saturation_plane(const Image<float, 1>& plane)
    {
        assert(plane.get_size().x() == get_size().x() and plane.get_size().y() == get_size().y());
        for (size_t x = 0; x < get_size().x(); ++x)
        {
            for (size_t y = 0; y < get_size().y(); ++y)
            {
                HSV value = operator()(x, y).to_hsv();
                value.saturation() = plane(x, y);
                operator()(x, y) = value.to_rgb();
            }
        }
    }

    Image<float, 1> ColorImage::get_value_plane()
    {
        Image<float, 1> out;
        out.create(_data.rows(), _data.cols());

        for (long y = 0; y < _data.cols(); ++y)
        {
            for (long x = 0; x < _data.cols(); ++x)
            {
                RGB as_rgb = _data(x, y);
                out(x, y) = as_rgb.to_hsv().value();
            }
        }

        return out;
    }

    void ColorImage::set_value_plane(const Image<float, 1>& plane)
    {
        assert(plane.get_size().x() == get_size().x() and plane.get_size().y() == get_size().y());
        for (size_t x = 0; x < get_size().x(); ++x)
        {
            for (size_t y = 0; y < get_size().y(); ++y)
            {
                HSV value = operator()(x, y).to_hsv();
                value.value() = plane(x, y);
                operator()(x, y) = value.to_rgb();
            }
        }
    }

    Image<float, 1> ColorImage::get_lightness_plane()
    {
        Image<float, 1> out;
        out.create(_data.rows(), _data.cols());

        for (long y = 0; y < _data.cols(); ++y)
        {
            for (long x = 0; x < _data.cols(); ++x)
            {
                RGB as_rgb = _data(x, y);
                out(x, y) = as_rgb.to_hsl().lightness();
            }
        }

        return out;
    }

    void ColorImage::set_lightness_plane(const Image<float, 1>& plane)
    {
        assert(plane.get_size().x() == get_size().x() and plane.get_size().y() == get_size().y());
        for (size_t x = 0; x < get_size().x(); ++x)
        {
            for (size_t y = 0; y < get_size().y(); ++y)
            {
                HSL value = operator()(x, y).to_hsl();
                value.lightness() = plane(x, y);
                operator()(x, y) = value.to_rgb();
            }
        }
    }
}