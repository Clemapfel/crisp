// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <fourier_transform.hpp>
#include <system.hpp>
#include <image/grayscale_image.hpp>

using namespace crisp;

int main()
{
    auto cube_img = GrayScaleImage(300, 300);

    for (size_t x = 0; x < cube_img.get_size().x(); ++x)
    for (size_t y = 0; y < cube_img.get_size().y(); ++y)
    {
        if (x > cube_img.get_size().x() * 0.45 and x < cube_img.get_size().x() * 0.55 and
            y > cube_img.get_size().y() * 0.35 and y < cube_img.get_size().y() * 0.6)
        {
            cube_img(x, y) = 1.f;
        }
        else
            cube_img(x, y) = 0.f;
    }

    save_to_disk(cube_img, "/home/clem/Workspace/crisp/docs/frequency_domain/cube.png");

    auto spectrum = FourierTransform<BALANCED>();
    spectrum.transform_from(cube_img);

    /*
    auto magnitude_img = spectrum.as_image();
    save_to_disk(magnitude_img, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_spectrum.png");

    auto angle_img = spectrum.phase_angle_as_image();
    save_to_disk(angle_img, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_angle.png");
    */

    auto dc = spectrum.get_dc_component();
    for (size_t x = 0; x < spectrum.get_size().x(); ++x)
    for (size_t y = 0; y < spectrum.get_size().y(); ++y)
    {
        if (x == spectrum.get_size().x() * 0.5 or y == spectrum.get_size().y() * 0.5)
            spectrum.get_component(x, y) = dc;
    }

    auto modified_spectrum = spectrum.as_image();
    save_to_disk(modified_spectrum, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_spectrum_modified.png");

    auto modified_cube = spectrum.transform_to<GrayScaleImage>();
    save_to_disk(modified_cube, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_modified.png");
}

