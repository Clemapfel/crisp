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

    auto median_filter = State::register_shader("mean_filter.glsl");
    auto median_program = State::register_program(median_filter);
    State::bind_shader_program(median_program);

    auto texture = State::register_texture(image);
    State::bind_texture(median_program, "_texture", texture);

    auto texture_size = State::register_vec2(image.get_size());
    State::bind_vec2(median_program, "_texture_size", texture_size);

    auto n = State::register_int(5);
    State::bind_int(median_program, "_neighborhood_size", n);

    auto workspace = Workspace();
    workspace.initialize<float, 3>(texture);

    for (size_t i : {1, 2})
    {
        workspace.draw_to_buffer();
        workspace.swap_buffers();
        texture = workspace.yield();
    }

    /*
    GLNativeHandle buffer;
    glGenFramebuffers(1, &buffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer);

    GLNativeHandle buffer_tex = State::register_texture<float, 3>(image.get_size().x(), image.get_size().y());

    auto tex_a = texture,
         tex_b = buffer_tex;

    size_t i = 0;
    for (i; i < 2; ++i)
    {
        // prevent generation loss by rounding error

        glBindTexture(GL_TEXTURE_2D, tex_b);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_b, 0);
        //assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindTexture(GL_TEXTURE_2D, tex_a);
        State::bind_texture(median_program, "_texture", tex_a);
        glViewport(0, 0, image.get_size().x(), image.get_size().y());
        State::display();

        auto temp = tex_b;
        tex_b = tex_a;
        tex_a = temp;
    }

    // make both textures equal
    State::bind_shader_program(-1);
    glBindTexture(GL_TEXTURE_2D, tex_b);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_b, 0);
    glBindTexture(GL_TEXTURE_2D, tex_a);
    State::bind_texture(median_program, "_texture", tex_a);
    State::display();
     */

    // render to screen
    State::bind_shader_program(-1);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    State::bind_texture(median_program, "_texture", texture);
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
                workspace.draw_to_buffer();
                workspace.swap_buffers();
                workspace.yield();

                State::display();
                window.display();

                State::bind_shader_program(-1);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
                State::bind_texture(median_program, "_texture", texture);
                glBindTexture(GL_TEXTURE_2D, texture);

                State::display();
                window.display();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

