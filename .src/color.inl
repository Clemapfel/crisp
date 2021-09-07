// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <color.hpp>

namespace crisp
{
    template<size_t N>
    ColorRepresentation<N>::ColorRepresentation<size_t N>()
        : Vector<float, N>()
    {}

    inline RGB::RGB(float r, float g, float b)
    {
        x() = r;
        y() = g;
        z() = b;
    }

    inline float & RGB::red()
    {
        return x();
    }

    inline float RGB::red() const
    {
        return x();
    }

    inline float & RGB::green()
    {
        return y();
    }

    inline float RGB::green() const
    {
        return y();
    }

    inline float & RGB::blue()
    {
        return z();
    }

    inline float RGB::blue() const
    {
        return z();
    }

    inline HSV::HSV(float h, float s, float v)
    {
        x() = h;
        y() = s;
        z() = v;
    }

    inline float& HSV::hue()
    {
        return x();
    }

    inline float HSV::hue() const
    {
        return x();
    }

    inline float & HSV::saturation()
    {
        return y();
    }

    inline float HSV::saturation() const
    {
        return y();
    }

    inline float & HSV::value()
    {
        return z();
    }

    inline float HSV::value() const
    {
        return z();
    }

    inline HSL::HSL(float h, float s, float l)
    {
        x() = h;
        y() = s;
        z() = l;
    }

    inline float & HSL::hue()
    {
        return x();
    }

    inline float HSL::hue() const
    {
        return x();
    }

    inline float & HSL::saturation()
    {
        return y();
    }

    inline float HSL::saturation() const
    {
        return y();
    }

    inline GrayScale::GrayScale(float i)
    {
        x() = i;
    }

    inline float & GrayScale::intensity()
    {
        return x();
    }

    inline float GrayScale::intensity() const
    {
        return x();
    }
}