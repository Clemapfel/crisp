// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <image/binary_image.hpp>

namespace crisp
{
    /*
    inline BinaryImage BinaryImage::operator!() const
    {
        BinaryImage out;
        out.create(get_size().x(), get_size().y());

        for (size_t x = 0; x < get_size().x(); ++x)
            for (size_t y = 0; y < get_size().y(); ++y)
                out(x, y) = not operator()(x, y);

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

    inline BinaryImage& BinaryImage::invert()
    {
        for (bool& b : (*this))
            b = not b;

        return *this;
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
     */
}