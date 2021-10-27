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

    auto image = crisp::load_grayscale_image("/home/clem/Workspace/crisp/docs/segmentation/.resources/opal_non_uniform.png");

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;
    context_settings.minorVersion = 3;
    context_settings.majorVersion = 3;
    context_settings.attributeFlags = context_settings.Core | context_settings.Default;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(2*image.get_size().x(), 2*image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    TODO: benchmarking object

    auto transform = FourierTransform<SPEED>();
    transform.transform_from(image);

    auto texture = State::register_texture<1>(2 * image.get_size().x(), 2 * image.get_size().y(), transform.get_spectrum());

    auto shader = State::register_shader("visualize_fourier.glsl");
    auto program = State::register_program(shader);
    State::bind_shader_program(program);
    State::bind_texture(program, "_texture", texture);

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
                shader = State::register_shader("visualize_fourier.glsl");
                program = State::register_program(shader);
                State::bind_shader_program(program);
                State::bind_texture(program, "_texture", texture);

                State::display();
                window.display();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

