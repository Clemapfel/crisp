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
#include <whole_image_transform.hpp>

#include <iostream>

using namespace crisp;

int main()
{
    auto image = load_grayscale_image("/home/clem/Workspace/crisp/.test/opal_color.png");
    //image = superpixel_clustering(image, 200);

    /*
    auto x_grad = image,
         y_grad = image;

    auto sobel = SpatialFilter();
    sobel.set_evaluation_function(SpatialFilter::CONVOLUTION);
    sobel.set_kernel(sobel.sobel_gradient_x());
    sobel.apply_to(x_grad);
    sobel.set_kernel(sobel.sobel_gradient_y());
    sobel.apply_to(y_grad);

    float min = 21312312312, max = 0;

    for (long x = 0; x < image.get_size().x(); ++x)
        for (long y = 0; y < image.get_size().y(); ++y)
        {
            float xg = x_grad(x, y);
            float yg = y_grad(x, y);

            image(x, y) = sqrt(xg * xg + yg * yg);

            min = std::min<float>(image(x, y), min);
            max = std::max<float>(image(x, y), max);
        }

    for (auto& px : image)
        px = (px - min) / (max - min);

    std::cout << min << " " << max << std::endl;
     */

    image.set_padding_type(PaddingType::ZERO);
    GrayScaleImage gradient = crisp::compute_gradient_magnitude(image);

    for (size_t x = 0; x < gradient.get_size().x(); ++x)
        std::cout << float(gradient(x, 0)) << std::endl;

    auto sprite = Sprite();
    sprite.create_from(gradient);
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

