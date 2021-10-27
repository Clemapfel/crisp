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

    auto image = ColorImage();//crisp::load_color_image("/home/clem/Workspace/crisp/docs/segmentation/.resources/opal_non_uniform.png");
    image.create(400, 500, RGB{1, 0, 1});

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

    auto shader = State::register_shader("ideal_bandpass.glsl");
    auto program = State::register_program(shader);

    auto pass_factor = State::register_float(1);
    auto reject_factor = State::register_float(0);
    auto cutoff_a = State::register_float(0.4);
    auto cutoff_b = State::register_float(0.25);

    auto size = State::register_vec2(texture.get_size());
    auto offset = State::register_vec2(Vector2f{0.05, 0.1});

    State::bind_shader_program(program);
    State::bind_float(program, "_cutoff_frequency", cutoff_a);
    State::bind_float(program, "_cutoff_a", cutoff_a);
    State::bind_float(program, "_cutoff_b", cutoff_b);
    State::bind_float(program, "_pass_factor", pass_factor);
    State::bind_float(program, "_reject_factor", reject_factor);
    State::bind_vec2(program, "_texture_size", size);
    State::bind_vec2(program, "_offset", offset);

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

