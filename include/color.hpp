// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector.hpp>

namespace crisp
{
    struct RGB;
    struct HSV;
    struct HSL;
    struct GrayScale;

    template<size_t N>
    struct ColorRepresentation : protected Vector<float, N>
    {
        ColorRepresentation();

        /*
        virtual RGB to_rgb() const = 0;
        virtual HSV to_hsv() const = 0;
        virtual HSL to_hsl() const = 0;
        virtual GrayScale to_grayscale() const = 0;*/
    };

    struct RGB : public ColorRepresentation<3>
    {
        RGB() = default;
        RGB(float, float, float);

        float& red();
        float red() const;

        float& green();
        float green() const;

        float& blue();
        float blue() const;
    };

    struct HSV : public ColorRepresentation<3>
    {
        HSV();
        HSV(float, float, float);

        float& hue();
        float hue() const;

        float& saturation();
        float saturation() const;

        float& value();
        float value() const;
    };

    struct HSL : public ColorRepresentation<3>
    {
        HSL();
        HSL(float, float, float);

        float& hue();
        float hue() const;

        float& saturation();
        float saturation() const;

        float& lightness();
        float lightness() const;
    };

    struct GrayScale : public ColorRepresentation<1>
    {
        GrayScale();
        GrayScale(float);

        float& intensity();
        float intensity() const;
    };
}