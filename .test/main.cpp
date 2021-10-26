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
#include <SFML/Graphics.hpp>

#include <system.hpp>
#include <GLES3/gl32.h>

#include <sol.hpp>

#include <gpu_side/state.hpp>

#include <segmentation.hpp>
#include <gpu_side/texture_workspace.hpp>
#include <gpu_side/texture.hpp>

#include <pseudocolor_mapping.hpp>
//#include <resource_path.hpp>

using namespace crisp;

int main()
{
    sol::state state;

    auto image = crisp::load_color_image("/home/clem/Workspace/crisp/docs/segmentation/.resources/opal_non_uniform.png");
    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;
    context_settings.minorVersion = 3;
    context_settings.majorVersion = 3;
    context_settings.attributeFlags = context_settings.Core | context_settings.Default;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x()-1, image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    auto texture = Texture<float, 3>(image);

    auto shader = State::register_shader("segmentation.glsl");
    auto program = State::register_program(shader);

    auto size = State::register_vec2(texture.get_size());
    auto neighborhood = State::register_int(3);

    State::bind_shader_program(program);
    State::bind_texture(program, "_texture", texture.get_handle());
    State::bind_vec2(program, "_texture_size", size);
    State::bind_int(program, "_neighborhood_size", neighborhood);

    State::display();
    window.display();

    while(window.isOpen())
    {
        auto event = sf::Event();
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Space)
            {
                State::display();
                window.display();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

