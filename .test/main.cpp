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

    auto image = crisp::load_grayscale_image("/home/clem/Workspace/crisp/.test/opal_color.png");

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x()-1, image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    std::string shader_id = "sobel_gradient_magnitude.glsl";
    auto shader = State::register_shader(shader_id);
    auto program = State::register_program(shader);
    State::bind_shader_program(program);

    auto texture = State::register_texture(image);
    State::bind_texture(program, "_texture", texture);

    auto size = State::register_vec2(image.get_size());
    State::bind_vec2(program, "_texture_size", size);

    auto workspace = Workspace();
    workspace.initialize<float, 3>(texture);

    workspace.display();
    texture = workspace.yield();

    /*
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    State::display();
    window.display();
     */

    auto as_tex = Texture<float, 1>(texture);
    auto as_img = as_tex.to_image();

    save_to_disk(as_img, "/home/clem/Workspace/crisp/.test/download.png");
    return 0;

    while(window.isOpen())
    {
        auto event = sf::Event();
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Space)
            {
                shader = State::register_shader(shader_id);
                program = State::register_program(shader);
                State::bind_shader_program(program);
                //State::bind_array<1>(program, "_gray_to_hue", array);

                State::display();
                window.display();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

