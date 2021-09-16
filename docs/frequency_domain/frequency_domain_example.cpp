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
        if (x > cube_img.get_size().x() * 0.45 and x < cube_img.get_size().x() * 0.255 and
            y > cube_img.get_size().y() * 0.3 and y < cube_img.get_size().y() * 0.6)
        {
            cube_img(x, y) = 1.f;
        }
        else
            cube_img(x, y) = 0.f;
    }

    save_to_disk(cube_img, "/home/clem/Workspace/crisp/docs/frequency_domain/cube.png");

    auto spectrum = FourierTransform<BALANCED>();
    spectrum.transform_from(cube_img);

    auto filter = FrequencyDomainFilter(spectrum);

    /*
    auto magnitude_img = spectrum.as_image();
    save_to_disk(magnitude_img, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_spectrum.png");

    auto angle_img = spectrum.phase_angle_as_image();
    save_to_disk(angle_img, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_angle.png");

    auto dc = spectrum.get_dc_component();
    for (size_t x = 0; x < spectrum.get_size().x(); ++x)
    for (size_t y = 0; y < spectrum.get_size().y(); ++y)
    {
        if (x == spectrum.get_size().x() * 0.25 or y == spectrum.get_size().y() * 0.25)
            spectrum.get_component(x, y) = dc;
    }

    auto modified_spectrum = spectrum.as_image();
    save_to_disk(modified_spectrum, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_spectrum_modified.png");

    auto modified_cube = spectrum.transform_to<GrayScaleImage>();
    save_to_disk(modified_cube, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_modified.png");
     */

    filter.as_identity();
    auto filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/identity.png");

    size_t x_size = spectrum.get_size().x(),
           y_size = spectrum.get_size().y();
    /*
    // low pass
    filter.as_ideal_lowpass(0.25 * x_size);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/ideal_lowpass.png");

    filter.as_gaussian_lowpass(0.25 * x_size);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/gaussian_lowpass.png");

    filter.as_butterworth_lowpass(0.25 * x_size, 1);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_1_lowpass.png");

    filter.as_butterworth_lowpass(0.25 * x_size, 2);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_2_lowpass.png");

    filter.as_butterworth_lowpass(0.25 * x_size, 3);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_3_lowpass.png");

    // high pass
    filter.as_ideal_highpass(0.25 * x_size);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/ideal_highpass.png");

    filter.as_gaussian_highpass(0.25 * x_size);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/gaussian_highpass.png");

    filter.as_butterworth_highpass(0.25 * x_size, 1);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_1_highpass.png");

    filter.as_butterworth_highpass(0.25 * x_size, 2);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_2_highpass.png");

    filter.as_butterworth_highpass(0.25 * x_size, 3);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_3_highpass.png");


    // bandpass
    filter.as_ideal_bandpass(0.15 * x_size, 0.3 * x_size);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/ideal_bandpass.png");

    filter.as_gaussian_bandpass(0.15 * x_size, 0.3 * x_size);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/gaussian_bandpass.png");

    filter.as_butterworth_bandpass(0.15 * x_size, 0.3 * x_size, 1);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_1_bandpass.png");

    filter.as_butterworth_bandpass(0.15 * x_size, 0.3 * x_size, 2);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_2_bandpass.png");

    filter.as_butterworth_bandpass(0.15 * x_size, 0.3 * x_size, 3);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_3_bandpass.png");

    // bandreject
    filter.as_ideal_bandreject(0.15 * x_size, 0.3 * x_size);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/ideal_bandreject.png");

    filter.as_gaussian_bandreject(0.15 * x_size, 0.3 * x_size);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/gaussian_bandreject.png");

    filter.as_butterworth_bandreject(0.15 * x_size, 0.3 * x_size, 1);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_1_bandreject.png");

    filter.as_butterworth_bandreject(0.15 * x_size, 0.3 * x_size, 2);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_2_bandreject.png");

    filter.as_butterworth_bandreject(0.15 * x_size, 0.3 * x_size, 3);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/butterworth_3_bandreject.png");
    */

    // offset
    filter.as_butterworth_bandpass(
        0.25 * spectrum.get_size().x(), // lower cutoff
        0.3 * spectrum.get_size().x(),  // upper cutoff
        4,  // order
        1,  // passing factor
        0   // attenuating factor
    );
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/no_offset.png");


    filter.set_offset(-0.2 * spectrum.get_size().x(), -0.1 * spectrum.get_size().y(), true);
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/with_symmetry.png");


}

