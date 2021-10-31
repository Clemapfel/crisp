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

#include <gpu_side/texture.hpp>

using namespace crisp;

int main()
{
    auto image = crisp::load_grayscale_image(get_resource_path() + "docs/segmentation/.resources/non_uniform.png");

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;
    context_settings.minorVersion = 3;
    context_settings.majorVersion = 3;
    context_settings.attributeFlags = context_settings.Core | context_settings.Default;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x(), image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    auto texture = Texture<float, 1>(image);

    for (size_t n = 1; n < 5; ++n)
    {
        auto res = Segmentation::threshold(texture, n, 0);
        save_to_disk(res.to_image(), get_resource_path() + "docs/hardware_acceleration/.resources/threshold_n" + std::to_string(n) + ".png");
    }

    for (size_t c = 0; c < 5; ++c)
    {
        auto res = Segmentation::threshold(texture, 7, c);
        save_to_disk(res.to_image(), get_resource_path() + "docs/hardware_acceleration/.resources/threshold_c" + std::to_string(c) + ".png");
    }

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

