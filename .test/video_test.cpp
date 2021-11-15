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
#include <system/render_window.hpp>
#include <morphological_transform.hpp>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>

#include <video/video_file.hpp>
#include <pseudocolor_mapping.hpp>

using namespace crisp;

int main()
{
    auto video = VideoFile();
    video.load("/home/clem/Workspace/crisp/.test/horse.mp4");//goose.gif");

    auto window = RenderWindow();
    window.create(video.get_size().x(), video.get_size().y());
    window.set_active();

    size_t frame_i = 0;
    auto tex = video.get_frame(0);
    video.set_frame(0, tex);

    std::set<size_t> already_filtered;

    std::cout << video.get_n_frames() << std::endl;

    while (window.is_open())
    {
        auto time = window.update();

        State::bind_shader_program(NONE);
        State::bind_texture(NONE, "_texture", tex);
        State::display();
        window.display();

        if (InputHandler::is_key_down(RIGHT) and frame_i < video.get_n_frames())
        {
            frame_i += 1;
            tex = video.get_frame(frame_i);

            if (already_filtered.find(frame_i) == already_filtered.end())
            {
                already_filtered.insert(frame_i);
            }
            //video.set_frame(frame_i, tex);
        }
        else if (InputHandler::is_key_down(LEFT) and frame_i != 0)
        {
            frame_i -= 1;
            tex = video.get_frame(frame_i);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 / video.get_fps())));
    }

    return 0;
}

