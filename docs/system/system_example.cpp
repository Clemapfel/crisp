// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <system.hpp>
#include <image/grayscale_image.hpp>
using namespace crisp;

int main()
{
    // load image as backup
    const auto original = load_grayscale_image("/home/clem/Workspace/crisp/docs/system/hidden_secret.png");

    // image that we will modify
    auto image = original;

    // create sprite
    auto sprite = Sprite();

    auto window = RenderWindow();
    window.create(image.get_size().x(), image.get_size().y());

    // modify the image by multiplying each pixel intensity with factor
    float factor = 1;
    auto update = [&]()
    {
        std::cout << "current factor: " << factor << std::endl;

        image = original;
        for (auto& px : image)
            px *= factor;

        // then rebind to sprite to update it
        sprite.create_from(image);
    };

    // render loop
    while (window.is_open())
    {
        // update input handler
        window.update();

        // press up to increase brightness
        if (InputHandler::was_key_pressed(KeyID::UP))
        {
            factor += 0.25;
            update();
        }

        // press down to decrease it
        else if (InputHandler::was_key_pressed(KeyID::DOWN))
        {
            factor -= 0.25;
            update();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}

