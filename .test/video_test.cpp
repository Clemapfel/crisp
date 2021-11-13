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
    video.load("/home/clem/Workspace/crisp/.test/horse.mp4");

    auto window = RenderWindow();
    window.create(video.get_size().x(), video.get_size().y());
    window.set_active();

    auto color = PseudoColor<GPU_SIDE>();
    color.add_value_range_to_hue_range(0, 0.3, 0, 1);

    size_t frame_i = 0;
    auto img = video.get_frame_as_image(0);

    float i = 0, step_size = 1.0 / (img.get_size().x() * img.get_size().y());
    for (auto& rgb : img)
    {
        auto as_hsv = rgb.to_hsv();
        as_hsv.hue() = i += step_size;
        as_hsv.saturation() = 1;
        rgb = as_hsv.to_rgb();
    }

    video.set_frame(0, img);

    auto tex = video.get_frame(0);
    /*
    color.apply_in_place(tex);
    video.set_frame(0, tex);
     */

    std::set<size_t> already_filtered;

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
                color.apply_in_place(tex);
                already_filtered.insert(frame_i);
            }
            video.set_frame(frame_i, tex);
        }
        else if (InputHandler::is_key_down(LEFT) and frame_i != 0)
        {
            frame_i -= 1;
            tex = video.get_frame(frame_i);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}

