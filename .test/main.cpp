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

#include <iostream>

using namespace crisp;

int main()
{
    auto image = load_grayscale_image("/home/clem/Workspace/crisp/.test/opal_color.png");

    auto morph = MorphologicalTransform();
    auto se = morph.all_foreground(3, 3);
    se(1, 1) = false;
    morph.set_structuring_element(se);
    //morph.dilate(image);

    auto pseudo = PseudoColorTransform();
    pseudo.set_function(pseudo.value_range_to_hue_range(0.3, 1, 0, 1));
    auto as_color = pseudo.transform(image);

    auto sprite = Sprite();
    sprite.create_from(as_color);
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

