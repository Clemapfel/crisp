// 
// Copyright 2021 Clemens Cords
// Created on 22.09.21 by clem (mail@clemens-cords.com)
//

#include <noise_generator.hpp>
#include <system/image_io.hpp>
#include <image/grayscale_image.hpp>
#include <histogram.hpp>
using namespace crisp;

int main()
{
    const auto original = load_grayscale_image("/home/clem/Workspace/crisp/docs/noise/.resources/noise_base.png");

    auto histogram = Histogram<256>(original);
    save_to_disk(histogram.as_image(), "/home/clem/Workspace/crisp/docs/noise/.resources/clean_hist.png");
    
    auto image = original;
    auto uniform = UniformNoise();
    uniform.set_interval(0, 0.2);
    for (auto& px : image)
        px = clamp<float>(0, 1, px + uniform());

    save_to_disk(image, "/home/clem/Workspace/crisp/docs/noise/.resources/uniform.png");
    histogram = Histogram<256>(image);
    save_to_disk(histogram.as_image(), "/home/clem/Workspace/crisp/docs/noise/.resources/uniform_hist.png");

    image = original;
    auto gaussian = GaussianNoise();
    gaussian.set_interval(0, 0.2);
    for (auto& px : image)
        px = clamp<float>(0, 1, px + gaussian());

    save_to_disk(image, "/home/clem/Workspace/crisp/docs/noise/.resources/gaussian.png");
    histogram = Histogram<256>(image);
    save_to_disk(histogram.as_image(), "/home/clem/Workspace/crisp/docs/noise/.resources/gaussian_hist.png");

    image = original;
    auto gamma = GammaNoise();
    gamma.set_interval(0, 0.2);
    for (auto& px : image)
        px = clamp<float>(0, 1, px + gamma());

    save_to_disk(image, "/home/clem/Workspace/crisp/docs/noise/.resources/gamma.png");
    histogram = Histogram<256>(image);
    save_to_disk(histogram.as_image(), "/home/clem/Workspace/crisp/docs/noise/.resources/gamma_hist.png");

    image = original;
    auto salt_and_pepper = SaltAndPepperNoise(0.1, 0.1);
    salt_and_pepper.set_interval(0, 0.2);
    for (auto& px : image)
        px = clamp<float>(0, 1, px + salt_and_pepper());

    save_to_disk(image, "/home/clem/Workspace/crisp/docs/noise/.resources/salt_and_pepper.png");
    histogram = Histogram<256>(image);
    save_to_disk(histogram.as_image(), "/home/clem/Workspace/crisp/docs/noise/.resources/salt_and_pepper_hist.png");
}

