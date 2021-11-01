// 
// Copyright 2021 Clemens Cords
// Created on 30.10.21 by clem (mail@clemens-cords.com)
//

#include <chrono>
#include <thread>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <system/image_io.hpp>
#include <benchmark.hpp>
#include <spatial_filter.hpp>
#include <morphological_transform.hpp>
#include <segmentation.hpp>
#include <fourier_transform.hpp>
#include <frequency_domain_filter.hpp>

#include <gpu_side/texture.hpp>

using namespace crisp;

int main()
{
    auto image = crisp::load_color_image(get_resource_path() + "docs/segmentation/.resources/non_uniform.png");

    /*
    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;
    context_settings.minorVersion = 3;
    context_settings.majorVersion = 3;
    context_settings.attributeFlags = context_settings.Core | context_settings.Default;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto context = sf::Context(context_settings, image.get_size().x(), image.get_size().y());
    context.setActive(true);
     */

    auto texture = Texture<float, 3>(image);

    auto shader = State::register_shader("sobel_gradient_magnitude.glsl");
    auto program = State::register_program(shader);
    State::bind_shader_program(program);
    State::bind_texture(program, "_texture", texture.get_handle());
    auto size = State::register_vec<2>(texture.get_size());
    State::bind_vec(program, "_texture_size", size);

    auto workspace = texture.get_workspace();
    workspace.display();
    workspace.yield();

    auto as_image = texture.to_image();
    save_to_disk(as_image, "/home/clem/Workspace/crisp/.test/out.png");
    return 0;

    /*
    State::bind_shader_program(NONE);
    State::bind_texture(NONE, "_texture", texture.get_handle());
    State::display();
    window.display();

    while(window.isOpen())
    {
        auto event = sf::Event();
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed)
                window.close();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
     */
}

