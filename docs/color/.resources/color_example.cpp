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

#include <pseudocolor_mapping.hpp>
#include <whole_image_transform.hpp>

int main()
{
    auto deer = load_grayscale_image("/home/clem/Workspace/crisp/docs/color/infrared_deer.png");
    auto color_deer = ColorImage();

    // for intro
    color_deer = PseudoColor::value_range_to_hue_range(0.601, 1, 0, 1, deer);
    save_to_disk(color_deer, "/home/clem/Workspace/crisp/docs/color/final_deer.png");

    // one to one (needs to be range because image is so noisy)
    color_deer = PseudoColor::value_range_to_hue(0.595, 0.605, 0, deer);
    save_to_disk(color_deer, "/home/clem/Workspace/crisp/docs/color/gray_value_to_hue_value.png");
    // many to one
    color_deer = PseudoColor::value_range_to_hue(0.6, 1, 0, deer);
    save_to_disk(color_deer, "/home/clem/Workspace/crisp/docs/color/gray_range_to_hue_value.png");

    // many to many
    color_deer = PseudoColor::value_range_to_hue_range(0, 0.8, 0.5, 1, deer);
    save_to_disk(color_deer, "/home/clem/Workspace/crisp/docs/color/gray_range_to_hue_range.png");

    // many to many inverse
    color_deer = PseudoColor::value_range_to_inverse_hue_range(0, 0.8, 0.5, 1, deer);
    save_to_disk(color_deer, "/home/clem/Workspace/crisp/docs/color/gray_range_to_inverse_hue_range.png");

    // multiple ranges
    auto ranges = PseudoColor::RangeMapping();
    ranges.add_value_range_to_hue(0.601, 1, 0);
    ranges.add_value_range_to_hue_range(0, 0.6, 0.2, 0.75);
    color_deer = PseudoColor::value_ranges_to_hue_ranges(ranges, deer);
    save_to_disk(color_deer, "/home/clem/Workspace/crisp/docs/color/gray_ranges_to_hue_ranges.png");

}

