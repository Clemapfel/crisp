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

#include <iostream>

using namespace crisp;

int main()
{
    auto image = load_binary_image("/home/clem/Workspace/crisp/.test/opal_color.png");

    auto morph = MorphologicalTransform();
    morph.set_structuring_element(morph.all_foreground(5, 5));
    //morph.dilate(image);

    auto sprite = Sprite();
    sprite.create_from(image);
    sprite.set_scale(2);

    auto size = sprite.get_size();
    auto window = RenderWindow(sprite.get_size().x(), sprite.get_size().y());

    while (window.is_open())
    {
        auto time = window.update();

        if (InputHandler::was_key_pressed(KeyID::SPACE))
        {
            morph.erode(image);
            sprite.create_from(image);
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}

