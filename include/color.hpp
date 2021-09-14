// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector.hpp>

namespace crisp
{
    // forward declarations
    struct RGB;
    struct HSV;
    struct HSL;
    struct GrayScale;

    /// @brief interface for colors as n-dimension vector of floats
    template<size_t N>
    struct ColorRepresentation : public Vector<float, N>
    {
        /// @brief default ctor
        ColorRepresentation();

        /// @brief convert to RGB format
        /// @returns RGB after conversion
        /// @note if any initial component is outside of [0, 1] the behavior is undefined
        virtual RGB to_rgb() const = 0;

        /// @brief convert to HSV format
        /// @returns HSV after conversion
        /// @note if any initial component is outside of [0, 1] the behavior is undefined
        virtual HSV to_hsv() const = 0;

        /// @brief convert to HSL format
        /// @returns HSL after conversion
        /// @note if any initial component is outside of [0, 1] the behavior is undefined
        virtual HSL to_hsl() const = 0;

        /// @brief convert to grayscale
        /// @returns equivalent GrayScale
        /// @note if any initial component is outside of [0, 1] the behavior is undefined
        virtual GrayScale to_grayscale() const = 0;
    };

    /// @brief color as RGB, all components in [0, 1]
    struct RGB : public ColorRepresentation<3>
    {
        /// @brief default ctor
        RGB() = default;

        /// @brief construct from 3 components
        /// @param red: x component, range [0, 1]
        /// @param green: y component, range [0, 1]
        /// @param blue: z component, range [0, 1]
        RGB(float red, float green, float blue);

        /// @brief construct with all components set to the same value
        /// @param all: value for x, y, z
        RGB(float all);

        /// @brief ctor from vector
        /// @param vector
        RGB(Vector<float, 3>);

        /// @brief assign from vector
        /// @param vector
        /// @returns reference to self after assignment
        RGB& operator=(Vector<float, 3>);

        /// @brief expose red component
        /// @returns reference to x component
        float& red();

        /// @brief expose red component
        /// @returns copy of x component
        float red() const;

        /// @brief expose green component
        /// @returns reference to y component
        float& green();

        /// @brief expose green component
        /// @returns copy of y component
        float green() const;

        /// @brief expose blue component
        /// @returns reference to z component
        float& blue();

        /// @brief expose blue component
        /// @returns copy of z component
        float blue() const;

        // @overide c.f. ColorRepresentation
        RGB to_rgb() const override;

        // @overide c.f. ColorRepresentation
        HSV to_hsv() const override;

        // @overide c.f. ColorRepresentation
        HSL to_hsl() const override;

        /// @overide c.f. ColorRepresentation
        GrayScale to_grayscale() const override;
    };

    /// @brief color as HSV, all components in [0, 1]
    struct HSV : public ColorRepresentation<3>
    {
        HSV() = default;

        /// @brief ctor
        /// @param hue: x component, range [0, 1]
        /// @param saturation: y component, range [0, 1]
        /// @param value: z  component, range [0, 1]
        /// @note the "value" component is sometimes called "brightness" in the literature
        HSV(float hue, float saturation, float value);

        /// @brief ctor
        /// @param all: x, y, z component, range [0, 1]
        HSV(float all);

        /// @brief ctor from vector
        HSV(Vector<float, 3>);

        /// @brief assign from vector
        HSV& operator=(Vector<float, 3>);

        /// @brief expose hue component
        /// @returns reference to x component
        float& hue();

        /// @brief expose hue component
        /// @returns copy of x component
        float hue() const;

        /// @brief expose saturation component
        /// @returns reference to y component
        float& saturation();

        /// @brief expose saturation component
        /// @returns copy of y component
        float saturation() const;

        /// @brief expose value component
        /// @returns reference to z component
        /// @note the "value" component is sometimes called "brightness" in the literature
        float& value();

        /// @brief expose value component
        /// @returns copy of z component
        /// @note the "value" component is sometimes called "brightness" in the literature
        float value() const;

        /// @overide c.f. ColorRepresentation
        RGB to_rgb() const override;

        /// @overide c.f. ColorRepresentation
        HSV to_hsv() const override;

        /// @overide c.f. ColorRepresentation
        HSL to_hsl() const override;

        /// @overide c.f. ColorRepresentation
        GrayScale to_grayscale() const override;
    };

    /// @brief color as HSL, all components in [0, 1]
    struct HSL : public ColorRepresentation<3>
    {
        HSL() = default;

        /// @brief ctor
        /// @param hue: x component, range [0, 1]
        /// @param saturation: y component, range [0, 1]
        /// @param lightness: z component, range [0, 1]
        HSL(float hue, float saturation, float lightness);

        /// @brief ctor
        /// @param all: x, y, z component, range [0, 1]
        HSL(float all);

        /// @brief ctor from vector
        HSL(Vector<float, 3>);

        /// @brief assign from vector
        HSL& operator=(Vector<float, 3>);

        /// @brief expose hue component
        /// @returns reference to x component
        float& hue();

        /// @brief expose hue component
        /// @returns copy of x component
        float hue() const;

        /// @brief expose saturation component
        /// @returns reference to y component
        float& saturation();

        /// @brief expose saturation component
        /// @returns copy of y component
        float saturation() const;

        /// @brief expose lightness component
        /// @returns reference to z component
        float& lightness();

        /// @brief expose lightness component
        /// @returns copy of z component
        float lightness() const;

        /// @overide c.f. ColorRepresentation
        RGB to_rgb() const override;

        /// @overide c.f. ColorRepresentation
        HSV to_hsv() const override;

        /// @overide c.f. ColorRepresentation
        HSL to_hsl() const override;

        /// @overide c.f. ColorRepresentation
        GrayScale to_grayscale() const override;
    };

    /// @brief color as single intensity
    struct GrayScale : public ColorRepresentation<1>
    {
        GrayScale() = default;

        /// @brief ctor from value
        /// @param intensity
        GrayScale(float);

        /// @brief ctor from vector
        /// @param vector: 1d vector with intensity
        GrayScale(Vector<float, 1>);

        /// @brief assign from value
        /// @param intensity
        /// @returns reference to self after assignment
        GrayScale& operator=(float);

        /// @brief assign from vector
        /// @param vector: 1d vector with intensity
        /// @returns reference to self after assignment
        GrayScale& operator=(Vector<float, 1>);

        /// @brief decay to value
        /// @returns float
        operator float() const;

        /// @brief expose intensity component
        /// @return reference to x component
        float& intensity();

        /// @brief expose intensity component
        /// @returns copy of x component
        float intensity() const;

        /// @overide c.f. ColorRepresentation
        RGB to_rgb() const override;

        /// @overide c.f. ColorRepresentation
        HSV to_hsv() const override;

        /// @overide c.f. ColorRepresentation
        HSL to_hsl() const override;

        /// @overide c.f. ColorRepresentation
        GrayScale to_grayscale() const override;
    };
}

#include ".src/color.inl"