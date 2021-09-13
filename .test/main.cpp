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
    auto image = load_color_image("/home/clem/Workspace/crisp/.test/opal_color.png");

    image = superpixel_clustering(image, 200);

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

