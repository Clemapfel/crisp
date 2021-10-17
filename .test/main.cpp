// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <classification/fully_connected_neural_network.hpp>
#include <classification/bayes_classifier.hpp>
#include <thread>
#include <iostream>
#include <fstream>

#include <SFML/OpenGL.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <system.hpp>
#include <GLES3/gl3.h>

#include <texture/state.hpp>
#include <texture/texture.hpp>
#include <texture/shader.hpp>
#include <segmentation.hpp>

using namespace crisp;

int main()
{
    auto image = crisp::load_color_image("/home/clem/Workspace/crisp/.test/opal_color.png");
    //auto image = image_in.convert_to_color();

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x()-1, image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    auto shader = Shader("/home/clem/Workspace/crisp/include/texture/.shaders/noop.frag");

    auto texture_ram = Texture<float, 3>(image.get_size().x(), image.get_size().y());
    texture_ram.create_from(image);

    save_to_disk(image, "/home/clem/Workspace/crisp/.test/texture_test_before.png");

    for (size_t y = 0, i = 0; y < image.get_size().y(); ++y)
        for (size_t x = 0; x < image.get_size().x(); ++x)
        {
            image(x, y) = RGB {
                texture_ram.expose_data()[i],
                texture_ram.expose_data()[i + 1],
                texture_ram.expose_data()[i + 2]
            };

            i = i + 3;
        }

    save_to_disk(image, "/home/clem/Workspace/crisp/.test/texture_test_after.png");

    shader.set_texture("_texture", texture_ram);

    shader.set_active();
    shader.bind_uniforms();

    // render
    State::display();
    window.display();

    while(window.isOpen())
    {
        auto event = sf::Event();
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed)
                window.close();
        }
    }
}

