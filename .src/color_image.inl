// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    GrayScaleImage ColorImage::get_red_plane()
    {
        return get_nths_plane<0>();
    }

    GrayScaleImage ColorImage::get_blue_plane()
    {
        return get_nths_plane<1>();
    }

    GrayScaleImage ColorImage::get_green_plane()
    {
        return get_nths_plane<2>();
    }

    GrayScaleImage ColorImage::get_hue_plane()
    {
        GrayScaleImage out;
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

    GrayScaleImage ColorImage::get_saturation_plane()
    {
        GrayScaleImage out;
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

    GrayScaleImage ColorImage::get_value_plane()
    {
        GrayScaleImage out;
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

    GrayScaleImage ColorImage::get_lightness_plane()
    {
        GrayScaleImage out;
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
}