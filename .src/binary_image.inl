// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    inline GrayScaleImage BinaryImage::convert_to_grayscale() const
    {
        GrayScaleImage out;
        out.create(get_size().x(), get_size().y());

        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                out.at(x, y) = at(x, y) ? 1.f : 0.f;

        return out;
    }

    inline ColorImage BinaryImage::convert_to_color() const
    {
        ColorImage out;
        out.create(get_size().x(), get_size().y());

        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                out(x, y) = at(x, y) ? RGB(1, 1, 1) : RGB(0, 0, 0);

        return out;
    }

    inline BinaryImage BinaryImage::operator!() const
    {
        BinaryImage out;
        out.create(get_size().x(), get_size().y());

        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                out(x, y) = not bool(operator()(x, y));

        return out;
    }

    inline BinaryImage BinaryImage::operator&(const BinaryImage& other) const
    {
        BinaryImage out;
        out.create(get_size().x(), get_size().y());

        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                out(x, y) = (*this)(x, y) & other(x, y);

        return out;
    }

    inline BinaryImage BinaryImage::operator|(const BinaryImage& other) const
    {
        BinaryImage out;
        out.create(get_size().x(), get_size().y());

        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                out(x, y) = (*this)(x, y) | other(x, y);

        return out;
    }

    inline BinaryImage BinaryImage::operator^(const BinaryImage& other) const
    {
        BinaryImage out;
        out.create(get_size().x(), get_size().y());

        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                out(x, y) = (*this)(x, y) ^ other(x, y);

        return out;
    }

    inline void BinaryImage::invert()
    {
        /*
        for (long y = 0; y < _data.cols(); ++y)
            for (long x = 0; x < _data.rows(); ++x)
                operator()(x, y) = not bool(at(x, y);*/
    }

    inline BinaryImage& BinaryImage::operator&=(const BinaryImage& other)
    {
        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                (*this)(x, y) = (*this)(x, y) & other(x, y);

        return *this;
    }

    inline BinaryImage& BinaryImage::operator|=(const BinaryImage& other)
    {
        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                (*this)(x, y) = (*this)(x, y) | other(x, y);

        return *this;
    }

    inline BinaryImage& BinaryImage::operator^=(const BinaryImage& other)
    {
        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                (*this)(x, y) = (*this)(x, y) ^ other(x, y);

        return *this;
    }
}