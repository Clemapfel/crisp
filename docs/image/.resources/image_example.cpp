// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <system.hpp>
#include <image/grayscale_image.hpp>
#include <whole_image_transform.hpp>
using namespace crisp;

int main()
{

    auto image = load_grayscale_image("/home/clem/Workspace/crisp/docs/color/.resources/infrared_deer.png");

    float min = 1.5, max = 0;
    for (auto& px : image)
    {
        min = std::min<float>(px, min);
        max = std::max<float>(px, max);
    }

    std::cout << min << " " << max << std::endl;

    auto before_hist = Histogram<256>(image);
    save_to_disk(before_hist.as_image(), "/home/clem/Workspace/crisp/docs/image/.resources/deer_hist_before.png");

    normalize(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/image/.resources/deer_normalized.png");

    auto after_hist = Histogram<256>(image);
    save_to_disk(after_hist.as_image(), "/home/clem/Workspace/crisp/docs/image/.resources/deer_hist_after.png");
    return 0;

    auto gradient = compute_gradient_magnitude(image);
    normalize(gradient);
    save_to_disk(gradient, "/home/clem/Workspace/crisp/docs/image/.resources/deer_gradient.png");

    return 0;
}

