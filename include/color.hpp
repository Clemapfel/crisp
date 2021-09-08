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

    // colors as n-dimension vector of floats
    template<size_t N>
    struct ColorRepresentation : public Vector<float, N>
    {
        // @brief default ctor
        ColorRepresentation();

        // @brief convert to RGB format
        virtual RGB to_rgb() const = 0;

        // @brief convert to HSV format
        virtual HSV to_hsv() const = 0;

        // @brief convert to HSL format
        virtual HSL to_hsl() const = 0;

        // @brief convert to grayscale
        virtual GrayScale to_grayscale() const = 0;
    };

    // color as RGB
    struct RGB : public ColorRepresentation<3>
    {
        RGB() = default;

        // @brief ctor
        RGB(float red, float green, float blue);

        // @brief ctor from vector
        RGB(Vector<float, 3>);

        // @brief assign from vector
        RGB& operator=(Vector<float, 3>);

        // @brief expose red (x) component
        float& red();
        float red() const;

        // @brief expose green (y) component
        float& green();
        float green() const;

        // @brief expose blue (z) component
        float& blue();
        float blue() const;

        // @override c.f. ColorRepresentation
        RGB to_rgb() const override;

        // @override c.f. ColorRepresentation
        HSV to_hsv() const override;

        // @override c.f. ColorRepresentation
        HSL to_hsl() const override;

        // @override c.f. ColorRepresentation
        GrayScale to_grayscale() const override;
    };

    // color as HSV
    struct HSV : public ColorRepresentation<3>
    {
        HSV() = default;

        // @brief ctor
        // @param hue: range [0, 1]
        // @param saturation: range [0, 1]
        // @param value: also called "brightness", range [0, 1]
        HSV(float hue, float saturation, float value);

        // @brief ctor from vector
        HSV(Vector<float, 3>);

        // @brief assign from vector
        HSV& operator=(Vector<float, 3>);

        // @brief expose hue (x) component
        float& hue();
        float hue() const;

        // @brief expose saturation (y) component
        float& saturation();
        float saturation() const;

        // @brief expose value (z) component
        float& value();
        float value() const;

        // @override c.f. ColorRepresentation
        RGB to_rgb() const override;

        // @override c.f. ColorRepresentation
        HSV to_hsv() const override;

        // @override c.f. ColorRepresentation
        HSL to_hsl() const override;

        // @override c.f. ColorRepresentation
        GrayScale to_grayscale() const override;
    };

    // color as HSL
    struct HSL : public ColorRepresentation<3>
    {
        HSL() = default;

        // @brief ctor
        // @param hue: range [0, 1]
        // @param saturation: range [0, 1]
        // @param lightness: range [0, 1]
        HSL(float hue, float saturation, float lightness);

        // @brief ctor from vector
        HSL(Vector<float, 3>);

        // @brief assign from vector
        HSL& operator=(Vector<float, 3>);

        // @brief expose hue (x) component
        float& hue();
        float hue() const;

        // @brief expose saturation (y) component
        float& saturation();
        float saturation() const;

        // @brief expose lightness (z) component
        float& lightness();
        float lightness() const;

        // @override c.f. ColorRepresentation
        RGB to_rgb() const override;

        // @override c.f. ColorRepresentation
        HSV to_hsv() const override;

        // @override c.f. ColorRepresentation
        HSL to_hsl() const override;

        // @override c.f. ColorRepresentation
        GrayScale to_grayscale() const override;
    };

    struct GrayScale : public ColorRepresentation<1>
    {
        GrayScale() = default;

        // @brief ctor from value
        GrayScale(float);

        // @brief ctor from vector
        GrayScale(Vector<float, 1>);

        // @brief assign from value
        GrayScale& operator=(float);

        // @brief assign from vector
        GrayScale& operator=(Vector<float, 1>);

        // @brief decay to value
        operator float() const;

        // @param expose intensity (value) component
        float& intensity();
        float intensity() const;

        // @override c.f. ColorRepresentation
        RGB to_rgb() const override;

        // @override c.f. ColorRepresentation
        HSV to_hsv() const override;

        // @override c.f. ColorRepresentation
        HSL to_hsl() const override;

        // @override c.f. ColorRepresentation
        GrayScale to_grayscale() const override;
    };
}

#include ".src/color.inl"