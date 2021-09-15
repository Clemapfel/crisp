// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <image.hpp>
#include <color.hpp>
#include <system/image_io.hpp>

using namespace crisp;
void grayscale_intensity()
{
    // vary grayscale intensity
    GrayScale intensity = 0.f;

    GrayScaleImage grayscale_spectrum;
    grayscale_spectrum.create(500, 100);

    float step = 1.f / (grayscale_spectrum.get_size().x() * grayscale_spectrum.get_size().y());

    for (size_t x = 0; x < grayscale_spectrum.get_size().x(); ++x)
    for (size_t y = 0; y < grayscale_spectrum.get_size().y(); ++y)
    {
        grayscale_spectrum.at(x, y) = intensity;
        intensity += step;
    }

    save_to_disk(grayscale_spectrum, "/home/clem/Workspace/crisp/docs/color/grayscale_spectrum.png");
}

void hue_spectrum()
{
    auto color = HSL{0, 1, 0};

    ColorImage specturm;
    specturm.create(500, 100);

    float step = 1.f / (specturm.get_size().x() * specturm.get_size().y());

    for (size_t x = 0; x < specturm.get_size().x(); ++x)
    for (size_t y = 0; y < specturm.get_size().y(); ++y)
    {
        specturm.at(x, y) = color.to_rgb();
        color.lightness() += step;
    }

    save_to_disk(specturm, "/home/clem/Workspace/crisp/docs/color/lightness_spectrum.png");
}

int main()
{
    hue_spectrum();
}

