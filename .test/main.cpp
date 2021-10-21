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

    if (false) {
        GLNativeHandle buffer;
        glGenFramebuffers(1, &buffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer);

        GLNativeHandle buffer_tex = State::register_texture(
                image); //State::register_texture<float, 3>(image.get_size().x(), image.get_size().y());

        glBindTexture(GL_TEXTURE_2D, buffer_tex);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.get_size().x(), image.get_size().y(), 0, GL_RGB, GL_FLOAT,
                     nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer_tex, 0);
        //assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindTexture(GL_TEXTURE_2D, texture);
        State::display();

        // render to screen
        State::bind_shader_program(-1);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        State::bind_texture(median_program, "_texture", buffer_tex);
        glBindTexture(GL_TEXTURE_2D, buffer_tex);

        State::display();
        window.display();
    }

    GLNativeHandle buffer;
    glGenFramebuffers(1, &buffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer);

    GLNativeHandle buffer_tex = State::register_texture(image); //State::register_texture<float, 3>(image.get_size().x(), image.get_size().y());

    auto tex_a = texture,
         tex_b = buffer_tex;

    size_t i = 0;
    for (i; i < 5; ++i)
    {
        float border[] = {1.f, 1.f, 1.f, 1.0f};

        glBindTexture(GL_TEXTURE_2D, tex_b);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_b, 0);
        //assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindTexture(GL_TEXTURE_2D, tex_a);
        State::bind_texture(median_program, "_texture", tex_a);
        State::display();

        auto temp = tex_b;
        tex_b = tex_a;
        tex_a = temp;

    }

    // render to screen
    auto result = i % 2 == 0 ? tex_a : tex_b;
    State::bind_shader_program(-1);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    State::bind_texture(median_program, "_texture", tex_b);
    glBindTexture(GL_TEXTURE_2D, tex_b);

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

