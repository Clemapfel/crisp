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

#include <gpu_side/hardware_accelerated_matrix.hpp>
#include <sol.hpp>
#include <histogram.hpp>

#include <audio/audio_file.hpp>
#include <system/sprite.hpp>

using namespace crisp;

int main()
{
    auto window = RenderWindow();
    window.create(500, 500);
    window.set_active();

    auto image = load_color_image("/home/clem/Workspace/crisp/.test/opal_color.png");
    auto texture = Texture<float, 3>(image);

    auto audio = AudioFile();
    audio.load("/home/clem/Workspace/crisp/.test/killdeer.wav");

    float max = std::numeric_limits<short>::min();
    float min = std::numeric_limits<short>::max();

    std::vector<float> data;
    for (size_t i = 0; i < audio.get_n_samples(); ++i)
    {
        data.push_back(audio.get_data()[i]);
        min = std::min(min, data.back());
        max = std::max(max, data.back());
    }

    for (auto& s : data)
        s = ((s - min) / (max - min)) * 2 - 1;  // project into [-1, 1]

    int offset = 0;
    int size = 500;
    auto signal = State::register_1d_signal(size, offset, &data[0]);
    auto shader = State::register_shader("visualize_1d.glsl");
    auto program = State::register_program(shader);
    State::bind_shader_program(program);

    State::bind_1d_signal(State::get_active_program_handle(), "_texture_1d", signal);
    State::display();

    while (window.is_open())
    {
        window.update();
        window.display();

        if (InputHandler::is_key_down(KeyID::RIGHT))
        {
            offset += size / 10;
            offset = std::min<int>(data.size(), offset);

            std::cout << "offset: " << offset << std::endl;
            State::free_1d_signal(signal);
            signal = State::register_1d_signal(size, offset, &data[0]);
            State::bind_1d_signal(State::get_active_program_handle(), "_texture_1d", signal);
            State::display();
        }
        else if (InputHandler::is_key_down(KeyID::LEFT))
        {
            offset -= size / 10;
            offset = std::max(0, offset);

            std::cout << "offset: " << offset << std::endl;
            State::free_1d_signal(signal);
            signal = State::register_1d_signal(size, offset, &data[0]);
            State::bind_1d_signal(State::get_active_program_handle(), "_texture_1d", signal);
            State::display();
        }
        else if (InputHandler::is_key_down(KeyID::UP))
        {
            size += 10;

            std::cout << "size: " << size << std::endl;
            State::free_1d_signal(signal);
            signal = State::register_1d_signal(size, offset, &data[0]);
            State::bind_1d_signal(State::get_active_program_handle(), "_texture_1d", signal);
            State::display();
        }

        else if (InputHandler::is_key_down(KeyID::DOWN))
        {
            size -= 10;
            size = std::max(0, size);

            std::cout << "size: " << size << std::endl;
            State::free_1d_signal(signal);
            signal = State::register_1d_signal(size, offset, &data[0]);
            State::bind_1d_signal(State::get_active_program_handle(), "_texture_1d", signal);
            State::display();
        }
        else if (InputHandler::is_key_down(KeyID::N))
        {
            size *= 2;

            std::cout << "size: " << size << std::endl;
            State::free_1d_signal(signal);
            signal = State::register_1d_signal(size, offset, &data[0]);
            State::bind_1d_signal(State::get_active_program_handle(), "_texture_1d", signal);
            State::display();
        }
        else if (InputHandler::is_key_down(KeyID::M))
        {
            size /= 2;

            std::cout << "size: " << size << std::endl;
            State::free_1d_signal(signal);
            signal = State::register_1d_signal(size, offset, &data[0]);
            State::bind_1d_signal(State::get_active_program_handle(), "_texture_1d", signal);
            State::display();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
    /*
    auto window = RenderWindow();
    window.create(300, 300);
    window.set_active();

    auto audio = AudioFile();
    audio.load("/home/clem/Workspace/crisp/.test/killdeer.wav");

    std::cout << "n samples: " << audio.get_n_samples() << std::endl;
    auto samples = audio.get_samples();

    std::cout << "n channels: " << audio.get_n_channels() << std::endl;
    std::cout << "max texture dim: " << GL_MAX_TEXTURE_SIZE << " | max array depth: " << GL_MAX_ARRAY_TEXTURE_LAYERS << std::endl;

    int16_t min = std::numeric_limits<int16_t>::max();
    int16_t max = std::numeric_limits<int16_t>::min();
    auto unique = std::set<float>();

    for (auto& s : samples)
    {
        min = std::min(min, s);
        max = std::max(max, s);
        unique.insert(s);
    }

    std::cout << "Min: " << min << " | " << "Max: " << max << std::endl;
    std::cout << "Unique: " << unique.size() << std::endl;


    auto shader = State::register_shader("visualize_1d.glsl");
    auto program = State::register_program(shader);
    State::bind_shader_program(program);
    auto image = load_color_image("/home/clem/Workspace/crisp/.test/opal_color.png");
    auto texture = Texture<float, 3>(image);

    auto sprite = Sprite();
    sprite.create_from(image);

    State::bind_texture(State::get_active_program_handle(), "_texture", texture);

    //auto as_1d_tex = State::register_1d_signal(audio.get_n_samples(), &audio.get_samples()[0]);
    //State::bind_1d_signal(program, "_texture_1d", as_1d_tex, 0);
    State::display();
    return 0;

    while (window.is_open())
    {
        auto time = window.update();
        window.clear();
        window.draw(sprite);
        window.display();

        if (InputHandler::was_key_pressed(SPACE))
        {}

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}

/*
 * auto shader = State::register_shader("matrix_operation/set_constant.glsl");//State::register_shader_from_source(source);
    auto program = State::register_program(shader);
    State::free_shader(shader);

    auto tex_out = Texture<float, 1>(State::register_texture<float, 1>(left.rows(), right.cols()));
    auto dummy = Texture<float, 1>(State::register_texture<float, 1>(left.rows(), right.cols()));

    std::vector<float> data;

    auto gpu_side = Benchmark([&]()
    {
        auto tex_left = Texture<float, 1>(State::register_texture<float, 1>(left.rows(), left.cols(), left.data()));
        auto tex_right = Texture<float, 1>(State::register_texture<float, 1>(right.rows(), right.cols(), right.data()));

        State::bind_shader_program(program);
        State::bind_texture(program, "_left", tex_left, 2);
        State::bind_texture(program, "_right", tex_right, 1);
        State::set_float(program, "_scalar", 5);

        GLNativeHandle buffer;
        glGenFramebuffers(1, &buffer);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer);
        glBindTexture(GL_TEXTURE_2D, tex_out);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_out, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, tex_out.get_size().x(), tex_out.get_size().y());

        State::display();

         data = State::get_texture_data(tex_out);
    });


 */

