// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <vector.hpp>
#include <image.hpp>
#include <color.hpp>
#include <image.hpp>
#include <system/image_io.hpp>
#include <system/sprite.hpp>
#include <system/render_window.hpp>
#include <spatial_filter.hpp>
#include <morphological_transform.hpp>
#include <pseudocolor_mapping.hpp>
#include <fourier_transform.hpp>
#include <edge_detection.hpp>
#include <segmentation.hpp>

#include <iostream>

using namespace crisp;

int main()
{
    auto image = load_grayscale_image("/home/clem/Workspace/crisp/.test/opal_color.png");
    //image = superpixel_clustering(image, 200);

    auto sobel = SpatialFilter();
    sobel.set_evaluation_function(SpatialFilter::NORMALIZED_CONVOLUTION);
    sobel.set_kernel(sobel.prewitt_gradient_x());
    sobel.apply_to(image);
    //sobel.set_kernel(sobel.sobel_gradient_y());
    //sobel.apply_to(image);

    float min = 192819281, max = -192839128;

    for (auto& px : image)
    {
        min = std::min(float(px), min);
        max = std::max(float(px), max);
    }

    std::cout << min << " " << max << std::endl;

    auto sprite = Sprite();
    sprite.create_from(image);
    sprite.set_scale(2);

    auto size = sprite.get_size();
    auto window = RenderWindow(sprite.get_size().x(), sprite.get_size().y());

    while (window.is_open())
    {
        auto time = window.update();

        window.clear();
        window.draw(sprite);
        window.display();
    }
}

