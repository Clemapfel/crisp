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

#include <gpu_side/state.hpp>

#include <segmentation.hpp>
#include <gpu_side/texture_workspace.hpp>
using namespace crisp;

int main()
{
    auto image = crisp::load_color_image("/home/clem/Workspace/crisp/.test/opal_color.png");

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x()-1, image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    auto shader = State::register_shader("dilate_mat3x3.glsl");
    auto program = State::register_program(shader);
    State::bind_shader_program(program);

    auto texture = State::register_texture(image);
    State::bind_texture(program, "_texture", texture);

    auto texture_size = State::register_vec2(image.get_size());
    State::bind_vec2(program, "_texture_size", texture_size);

    auto se = State::register_structuring_element(MorphologicalTransform::all_foreground(3, 3));
    State::bind_matrix(program, "_structuring_element", se);

    auto workspace = Workspace();
    workspace.initialize<float, 3>(texture);
    workspace.display();
    workspace.display();
    workspace.display();
    workspace.display();
    texture = workspace.yield();

    // render to screen
    State::bind_shader_program(-1);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, texture);

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
                //workspace.draw_to_buffer();
                //workspace.swap_buffers();
                //workspace.yield();

                State::display();
                window.display();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

