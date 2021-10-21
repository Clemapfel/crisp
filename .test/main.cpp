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

using namespace crisp;

int main()
{
    auto image = crisp::load_grayscale_image("/home/clem/Workspace/crisp/.test/opal_color.png");

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x()-1, image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    auto median_filter = State::register_shader("mean_filter.glsl");
    auto median_program = State::register_program(median_filter);
    State::bind_shader_program(median_program);

    auto texture = State::register_texture(image);
    State::bind_texture(median_program, "_texture", texture);

    auto texture_size = State::register_vec2(image.get_size());
    State::bind_vec2(median_program, "_texture_size", texture_size);

    auto n = State::register_int(5);
    State::bind_int(median_program, "_neighborhood_size", n);

    for (size_t i = 0; i < 3; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        State::display();

        glBlitFramebuffer(0, 0, 0, 0, 0, 0, 0, 0, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, image.get_size().x(), image.get_size().y());
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //glDeleteFramebuffers(1, &buffer_handle);

    State::bind_shader_program(NONE);
    State::bind_texture(State::get_active_program_handle(), "_texture", texture, 0);

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

