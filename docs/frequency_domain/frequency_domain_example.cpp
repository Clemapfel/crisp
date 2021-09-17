// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <fourier_transform.hpp>
#include <system.hpp>
#include <image/grayscale_image.hpp>
#include <whole_image_transform.hpp>

using namespace crisp;

int main()
{
    /*
    auto image = load_grayscale_image("/home/clem/Workspace/crisp/docs/frequency_domain/color_opal.png");

    save_to_disk(image, "/home/clem/Workspace/crisp/docs/frequency_domain/grayscale_opal.png");

    auto spectrum = FourierTransform<SPEED>();
    spectrum.transform_from(image);

    auto spectrum_img = spectrum.as_image();
    save_to_disk(spectrum_img, "/home/clem/Workspace/crisp/docs/frequency_domain/opal_spectrum.png");

    auto filter = FrequencyDomainFilter(spectrum);
    filter.as_ideal_highpass(0.4 * spectrum.get_size().x(), 1, 0);

    auto other = FrequencyDomainFilter(spectrum);
    other.as_gaussian_lowpass(0.1 * spectrum.get_size().x(), 1, 0);

    filter += other;
    filter.normalize();

    auto filter_img = filter.as_image<GrayScaleImage>();
    normalize(filter_img);
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/combined_filter.png");

    filter.apply_to(spectrum);

    spectrum_img = spectrum.as_image();
    save_to_disk(spectrum_img, "/home/clem/Workspace/crisp/docs/frequency_domain/opal_spectrum_filtered.png");

    image = spectrum.transform_to<GrayScaleImage>();
    normalize(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/frequency_domain/opal_filtered.png");

    auto cube_img = GrayScaleImage(300, 300);

    for (size_t x = 0; x < cube_img.get_size().x(); ++x)
    for (size_t y = 0; y < cube_img.get_size().y(); ++y)
    {
        if (x > 0.35 * cube_img.get_size().x() and x < 0.45 * cube_img.get_size().x() and
            y > 0.45 * cube_img.get_size().y() and y < 0.75 * cube_img.get_size().y())
            cube_img(x, y) = 1.f;
        else
            cube_img(x, y) = 0.f;
    }

    save_to_disk(cube_img, "/home/clem/Workspace/crisp/docs/frequency_domain/cube.png");

    auto spectrum = FourierTransform<BALANCED>();
    spectrum.transform_from(cube_img);

    auto filter = FrequencyDomainFilter(spectrum);

    auto magnitude_img = spectrum.as_image();
    save_to_disk(magnitude_img, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_spectrum.png");

    auto angle_img = spectrum.phase_angle_as_image();
    save_to_disk(angle_img, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_angle.png");

    auto dc = spectrum.get_dc_component();

    auto center_x = spectrum.get_size().x() * 0.5;
    auto center_y = spectrum.get_size().y() * 0.5;

    for (size_t x = 0; x < spectrum.get_size().x(); ++x)
    for (size_t y = 0; y < spectrum.get_size().y(); ++y)
    {
        if (x == center_x or y == center_y)
        {
            auto dist = sqrt((center_x - x) * (center_x - x) + (center_y - y) * (center_y - y));
            if (dist > 0.1 * center_x);
                spectrum.get_component(x, y) *= dist;
        }
    }

    auto modified_spectrum = spectrum.as_image();
    save_to_disk(modified_spectrum, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_spectrum_modified.png");

    auto modified_cube = spectrum.transform_to<GrayScaleImage>();
    save_to_disk(modified_cube, "/home/clem/Workspace/crisp/docs/frequency_domain/cube_modified.png");
    */

    filter.as_identity();
    auto filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/identity.png");

    size_t x_size = spectrum.get_size().x() * 0.5,
           y_size = spectrum.get_size().y() * 0.5;

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
    filter.normalize();
    filter_img = filter.as_image<GrayScaleImage>();
    save_to_disk(filter_img, "/home/clem/Workspace/crisp/docs/frequency_domain/with_symmetry.png");

}

