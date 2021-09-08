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
    struct ColorRepresentation
    {
        ColorRepresentation();

        virtual RGB to_rgb() const = 0;
        virtual HSV to_hsv() const = 0;
        virtual HSL to_hsl() const = 0;
        virtual GrayScale to_grayscale() const = 0;
    };

    struct RGB : public ColorRepresentation<3>, public Vector<float, 3>
    {
        RGB() = default;
        RGB(float, float, float);

        RGB(Vector<float, 3>);
        RGB& operator=(Vector<float, 3>);

        float& red();
        float red() const;

        float& green();
        float green() const;

        float& blue();
        float blue() const;

        RGB to_rgb() const override;
        HSV to_hsv() const override;
        HSL to_hsl() const override;
        GrayScale to_grayscale() const override;
    };

    struct HSV : public ColorRepresentation<3>, public Vector<float, 3>
    {
        HSV() = default;
        HSV(float, float, float);

        HSV(Vector<float, 3>);
        HSV& operator=(Vector<float, 3>);
        operator Vector<float, 3>() const;

        float& hue();
        float hue() const;

        float& saturation();
        float saturation() const;

        float& value();
        float value() const;

        RGB to_rgb() const override;
        HSV to_hsv() const override;
        HSL to_hsl() const override;
        GrayScale to_grayscale() const override;
    };

    struct HSL : public ColorRepresentation<3>, public Vector<float, 3>
    {
        HSL() = default;
        HSL(float, float, float);

        HSL(Vector<float, 3>);
        HSL& operator=(Vector<float, 3>);
        operator Vector<float, 3>() const;

        float& hue();
        float hue() const;

        float& saturation();
        float saturation() const;

        float& lightness();
        float lightness() const;

        RGB to_rgb() const override;
        HSV to_hsv() const override;
        HSL to_hsl() const override;
        GrayScale to_grayscale() const override;
    };

    struct GrayScale : public ColorRepresentation<1>, public Vector<float, 1>
    {
        GrayScale() = default;
        GrayScale(float);
        GrayScale(Vector<float, 1>);

        GrayScale& operator=(Vector<float, 1>);
        GrayScale& operator=(float);

        operator Vector<float, 1>() const;
        operator float() const;

        float& intensity();
        float intensity() const;

        RGB to_rgb() const override;
        HSV to_hsv() const override;
        HSL to_hsl() const override;
        GrayScale to_grayscale() const override;
    };
}

#include ".src/color.inl"