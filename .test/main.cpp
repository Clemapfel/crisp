// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <vector.hpp>
#include <image.hpp>
#include <color.hpp>
#include <system/image_io.hpp>
#include <system/sprite.hpp>
#include <system/render_window.hpp>

#include <iostream>

using namespace crisp;

int main()
{
    auto image = load_color_image("/home/clem/Workspace/crisp/.test/opal_color.png");

    auto sprite = Sprite();
    sprite.create_from(image);
    sprite.set_scale(2);

    auto window = RenderWindow(sprite.get_size().x(), sprite.get_size().y());

    while (window.is_open())
    {
        auto time = window.update();
        window.clear();
        window.draw(sprite);
        window.display();
    }
}

