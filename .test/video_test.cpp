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

using namespace crisp;

int main()
{
    auto video = VideoFile();
    video.load("/home/clem/Workspace/crisp/include/video/horse.mp4");

    auto window = RenderWindow();
    window.create(video.get_size().x(), video.get_size().y());
    window.set_active();

    auto shader = State::register_shader("median_filter_5x5.glsl");
    auto program = State::register_program(shader);

    auto morph = MorphologicalTransform();
    morph.set_structuring_element(morph.all_foreground(3, 3));

    auto filter = SpatialFilter();
    filter.set_kernel(filter.box(3, 0));

    size_t frame_i = 0;
    auto tex = video.get_frame(0);

    while (window.is_open())
    {
        auto time = window.update();

        State::bind_shader_program(program);
        State::bind_texture(program, "_texture", tex);
        State::display();
        window.display();

        if (InputHandler::is_key_down(RIGHT) and frame_i < video.get_n_frames())
        {
            frame_i += 1;
            tex = video.get_frame(frame_i);
            morph.erode(tex);
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

