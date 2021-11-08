// 
// Copyright 2021 Clemens Cords
// Created on 06.11.21 by clem (mail@clemens-cords.com)
//

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/video.hpp>
#include <opencv4/opencv2/videoio.hpp>

#include <gpu_side/texture.hpp>
#include <system/image_io.hpp>
#include <benchmark.hpp>
#include <spatial_filter.hpp>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>

#include <video/video_file.hpp>

using namespace crisp;

int main()
{
    auto video = VideoFile();
    video.create("/home/clem/Workspace/crisp/include/video/horse.mp4");

    auto size = video.get_size();

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;
    context_settings.minorVersion = 3;
    context_settings.majorVersion = 3;
    context_settings.attributeFlags = context_settings.Core | context_settings.Default;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(size.x(), size.y()), "", style, context_settings);
    window.setActive(true);

    auto shader = State::register_shader("sobel_gradient_magnitude.glsl");
    auto program = State::register_program(shader);

    auto filter = SpatialFilter();
    filter.set_kernel(filter.box(3, 0));

    size_t frame_i = 0;
    auto tex = video.get_frame(0);

    while(window.isOpen())
    {
        tex = video.get_frame(frame_i);
        filter.apply_to(tex);
        video.set_frame(frame_i, tex);

        State::bind_shader_program(NONE);
        State::bind_texture(NONE, "_texture", video.get_frame(frame_i));
        //State::set_vec<2>(program, "_texture_size", video.get_size().cast_to<float>());
        State::display();
        window.display();

        auto event = sf::Event();
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::EventType::KeyPressed and event.key.code == sf::Keyboard::Right)
            {
                frame_i = frame_i < video.get_n_frames() ? frame_i + 1 : frame_i;
                tex = video.get_frame(frame_i);
            }
            else if (event.type == sf::Event::EventType::KeyPressed and event.key.code == sf::Keyboard::Left)
            {
                frame_i = frame_i != 0 ? frame_i - 1 : frame_i;
                tex = video.get_frame(frame_i);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}

