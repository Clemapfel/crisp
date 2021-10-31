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

#include <gpu_side/texture.hpp>

using namespace crisp;

int main()
{
    auto image = crisp::load_color_image(get_resource_path() + "docs/hardware_acceleration/.resources/color_opal.png");

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;
    context_settings.minorVersion = 3;
    context_settings.majorVersion = 3;
    context_settings.attributeFlags = context_settings.Core | context_settings.Default;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x(), image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    auto texture = Texture<float, 3>(image);

    auto transform = MorphologicalTransform();
    transform.set_structuring_element(transform.all_foreground(4, 4));

    auto filter_texture = Benchmark([&](){
        transform.open(texture);
    });

    std::cout << "texture: " << filter_texture.execute(100) << "ms" << std::endl;

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
}

