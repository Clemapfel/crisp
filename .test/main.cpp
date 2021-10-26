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

    auto image = crisp::load_color_image("/home/clem/Workspace/crisp/docs/morphological_transform/.resources/grayscale_template.png");
    auto mask = crisp::load_color_image("/home/clem/Workspace/crisp/docs/morphological_transform/.resources/mask.png");
    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;
    context_settings.minorVersion = 3;
    context_settings.majorVersion = 3;
    context_settings.attributeFlags = context_settings.Core | context_settings.Default;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x()-1, image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    auto as_tex = Texture<float, 3>(image);
    auto mask_tex = Texture<float, 3>(mask);

    /*
    auto filter = MorphologicalTransform();
    filter.set_structuring_element(filter.all_foreground(3, 3));
    filter.erode(as_tex, mask_tex);
     */

    auto shader = State::register_shader("geodesic_compare_erode.glsl");
    auto program = State::register_program(shader);
    State::free_shader(shader);

    //State::bind_shader_program(program);
    State::bind_texture(program, "_texture", as_tex, 0);
    State::bind_texture(program, "_mask", mask_tex, 1);

    /*
    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "_texture"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, as_tex);

    glUniform1i(glGetUniformLocation(program, "_mask"), 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, mask_tex);
     */

    /*
    State::bind_shader_program(program);
    State::bind_texture(program, "_texture", as_tex.get_handle(), 0);
    State::bind_texture(program, "_mask", mask_tex.get_handle(), 1);
    */
    //State::bind_shader_program(NONE);
    //State::bind_texture(State::get_active_program_handle(), "_texture", as_tex);
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

