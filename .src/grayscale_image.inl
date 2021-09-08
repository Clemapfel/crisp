// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    inline ColorImage GrayScaleImage::convert_to_color() const
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