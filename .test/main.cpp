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

    /*

    // shaders
    auto fragment_id = state.add_shader("/home/clem/Workspace/crisp/include/texture/.shaders/noop.frag");
    state.bind_shader(fragment_id);

    // texture
    auto tex = Texture<float, 3>(image.get_size().x(), image.get_size().y());
    tex.create_from(image);
    auto texture = state.add_texture(tex);
    state.bind_texture(texture);

    // render
    state.display();
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
     */
}

