// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <color.hpp>

namespace crisp
{
    template<size_t N>
    inline ColorRepresentation<N>::ColorRepresentation()
    {}

    inline RGB RGB::to_rgb() const
    {
        return RGB(red(), green(), blue());
    }
    
    inline HSV RGB::to_hsv() const
    {
        float h, s, v;

        auto min = red() < green() ? red() : green();
        min = min  < blue() ? min  : blue();

        auto max = red() > green() ? red() : green();
        max = max  > blue() ? max  : blue();

        auto delta = max - min;

        if (delta > 0)
        {
            if (max == red())
                h = 60 * (fmod(((green() - blue()) / delta), 6));

            else if (max == green())
                h = 60 * (((blue() - red()) / delta) + 2);

            else if (max == blue())
                h = 60 * (((red() - green()) / delta) + 4);

            if (max > 0)
                s = delta / max;
            else
                s = 0;

            v = max;
        }
        else
        {
            h = 0;
            s = 0;
            v = max;
        }

        if (h < 0)
            h += 360;

        return HSV(h / 360.f, s, v);
    }

    inline HSL RGB::to_hsl() const
    {
        return to_hsv().to_hsl();
    }

    inline GrayScale RGB::to_grayscale() const
    {
        return GrayScale((red() + green() + blue()) / 3);
    }
    
    inline RGB HSV::to_rgb() const
    {
        auto h = hue();

        h *= 360;
        float c = value() * saturation();
        float h_2 = h / 60;
        float x = c * (1 - std::fabs(std::fmod(h_2, 2) - 1));

        RGB out;

        if (0 <= h_2 and h_2 < 1)
        {
            out = RGB(c, x, 0);
        }
        else if (1 <= h_2 and h_2 < 2)
        {
            out = RGB(x, c, 0);
        }
        else if (2 <= h_2 and h_2 < 3)
        {
            out = RGB(0, c, x);
        }
        else if (3 <= h_2 and h_2 < 4)
        {
            out = RGB(0, x, c);
        }
        else if (4 <= h_2 and h_2 < 5)
        {
            out = RGB(x, 0, c);
        }
        else if (5 <= h_2 and h_2 <= 6)
        {
            out = RGB(c, 0, x);
        }

        auto m = value() - c;
        out.red() += m;
        out.green() += m;
        out.blue() += m;

        return out;
    }
    
    inline HSV HSV::to_hsv() const
    {
        return HSV(hue(), saturation(), value());
    }
    
    inline HSL HSV::to_hsl() const
    {
        float hsv_s = saturation(),
              hsv_v = value();

        float hsl_l = hsv_v * (1 - hsv_s / 2),
              hsl_s;

        if (hsl_l == 0 or hsl_l == 1)
            hsl_s = 0;
        else
            hsv_s = (hsv_v - hsl_l) / std::min(hsl_l, 1.f - hsl_l);

        HSL out;
        out.hue() = hue();
        out.saturation() = hsl_s;
        out.lightness() = hsl_l;

        return out;
    }
    
    inline GrayScale HSV::to_grayscale() const
    {
        return GrayScale(value());
    }
    
    inline RGB HSL::to_rgb() const
    {
        return to_hsv().to_rgb();
    }
    
    inline HSV HSL::to_hsv() const
    {
        auto s = saturation();
        s *= lightness() < 0.5 ? lightness() : 1 - lightness();

        HSV out;
        out.hue() = hue();
        out.saturation() = 2 * s / (lightness() + s);
        out.value() = lightness() + s;

        return out;
    }
    
    inline HSL HSL::to_hsl() const
    {
        return HSL(hue(), saturation(), lightness());
    }
    
    inline GrayScale HSL::to_grayscale() const
    {
        return GrayScale(lightness());
    }
    
    inline RGB GrayScale::to_rgb() const
    {
        return RGB(intensity(), intensity(), intensity());
    }
    
    inline HSV GrayScale::to_hsv() const
    {
        return HSV(0, 0, intensity());
    }
    
    inline HSL GrayScale::to_hsl() const
    {
        return HSL(0, 0, intensity());
    }
    
    inline GrayScale GrayScale::to_grayscale() const
    {
        return GrayScale(intensity());
    }


    inline RGB::RGB(float r, float g, float b)
    {
        x() = r;
        y() = g;
        z() = b;
    }

    inline RGB::RGB(Vector<float, 3> vec)
    {
        for (size_t i = 0; i < 3; ++i)
            Vector<float, 3>::at(i) = vec.at(i);
    }

    inline RGB& RGB::operator=(Vector<float, 3> vec)
    {
        for (size_t i = 0; i < 3; ++i)
            Vector<float, 3>::at(i) = vec.at(i);

        return *this;
    }

    /*
    inline RGB::operator Vector<float, 3>() const
    {
        return Vector<float, 3>({red(), green(), blue()});
    }*/

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

    inline HSV::HSV(Vector<float, 3> vec)
    {
        for (size_t i = 0; i < 3; ++i)
            Vector<float, 3>::at(i) = vec.at(i);
    }

    inline HSV& HSV::operator=(Vector<float, 3> vec)
    {
        for (size_t i = 0; i < 3; ++i)
            Vector<float, 3>::at(i) = vec.at(i);

        return *this;
    }
    
    inline HSV::operator Vector<float, 3>() const
    {
        return Vector<float, 3>({hue(), saturation(), value()});
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

    inline HSL::HSL(Vector<float, 3> vec)
    {
        for (size_t i = 0; i < 3; ++i)
            Vector<float, 3>::at(i) = vec.at(i);
    }

    inline HSL& HSL::operator=(Vector<float, 3> vec)
    {
        for (size_t i = 0; i < 3; ++i)
            Vector<float, 3>::at(i) = vec.at(i);

        return *this;
    }
    
    inline HSL::operator Vector<float, 3>() const
    {
        return Vector<float, 3>({hue(), saturation(), lightness()});
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

    inline float HSL::lightness() const
    {
        return z();
    }

    inline float& HSL::lightness()
    {
        return z();
    }

    inline GrayScale::GrayScale(float i)
    {
        x() = i;
    }

    inline GrayScale::GrayScale(Vector<float, 1> v)
    {
        intensity() = v;
    }

    GrayScale& GrayScale::operator=(Vector<float, 1> v)
    {
        intensity() = v;
        return *this;
    }

    GrayScale& GrayScale::operator=(float v)
    {
        intensity() = v;
        return *this;
    }

    inline GrayScale::operator Vector<float, 1>() const
    {
        return Vector<float, 1>(intensity());
    }
    
    inline GrayScale::operator float() const
    {
        return intensity();
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