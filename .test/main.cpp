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
#include <benchmark.hpp>
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

    auto transform = FourierTransform<SPEED>();
    transform.transform_from(image);

    auto filter = FrequencyDomainFilter<GPU_SIDE>(transform);
    filter.as_gaussian_bandpass(0.2, 0.3);
    //filter.apply_to(transform);

    auto img = transform.as_image();
    auto tex = Texture<float, 1>(img);
    State::bind_shader_program(NONE);
    //State::bind_texture(NONE, "_texture", tex.get_handle());
    State::display();

    /*
    auto benchmark = Benchmark([&](Texture<float, 1>& tex){tex = transform.as_texture();});

    Texture<float, 1> texture = transform.as_texture();
    std::cout << benchmark.execute<Texture<float, 1>&>(100, texture) << std::endl;

    auto shader = State::register_shader("lowpass_butterworth.glsl");
    auto program = State::register_program(shader);

    State::bind_shader_program(program);
    State::bind_texture(program, "_texture", texture.get_handle());

    auto pass_factor = State::register_float(1);
    auto reject_factor = State::register_float(0);
    auto cutoff_a = State::register_float(0.4);
    auto cutoff_b = State::register_float(0.25);

    auto size = State::register_vec2(texture.get_size());
    auto offset = State::register_vec2(Vector2f{0.1, 0.1});
    auto order = State::register_int(3);

    State::bind_float(program, "_cutoff_frequency", cutoff_a);
    State::bind_float(program, "_cutoff", cutoff_a);
    State::bind_float(program, "_cutoff_a", cutoff_a);
    State::bind_float(program, "_cutoff_b", cutoff_b);
    State::bind_float(program, "_pass_factor", pass_factor);
    State::bind_float(program, "_reject_factor", reject_factor);
    State::bind_vec2(program, "_texture_size", size);
    State::bind_vec2(program, "_offset", offset);
    State::bind_int(program, "_order", order);
    */

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
                filter.apply_to(transform);
                img = transform.as_image();
                tex = Texture<float, 1>(img);
                State::bind_shader_program(NONE);
                State::bind_texture(NONE, "_texture", tex.get_handle());
                State::display();

                State::display();
                window.display();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

