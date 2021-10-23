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

#include <pseudocolor_mapping.hpp>
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

    std::string shader_id = "pseudocolor.glsl";
    auto shader = State::register_shader(shader_id);
    auto program = State::register_program(shader);
    State::bind_shader_program(program);

    auto texture = State::register_texture(image);
    State::bind_texture(program, "_texture", texture);

    auto mapping = PseudoColor::RangeMapping();
    mapping.add_value_range_to_hue_range(0, 0.1, 0, 1);
    auto mapping_as_array = PseudoColor::range_mapping_to_array(mapping);
    auto array = State::register_array(mapping_as_array);

    for (size_t i = 0; i < mapping_as_array.size(); ++i)
        std::cout << i << " -> " << mapping_as_array[i] << std::endl;

    State::bind_array<1>(program, "_gray_to_hue", array);

    /*
    auto workspace = Workspace();
    workspace.initialize<float, 3>(texture);
    workspace.display();
    texture = workspace.yield();
     */

    // render to screen
    //State::bind_shader_program(-1);

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
                shader = State::register_shader(shader_id);
                program = State::register_program(shader);
                State::bind_shader_program(program);
                State::bind_array<1>(program, "_gray_to_hue", array);

                State::display();
                window.display();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

