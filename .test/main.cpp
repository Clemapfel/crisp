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

#include <fourier_transform.hpp>

using namespace crisp;

int main()
{
    constexpr size_t width = 1500;
    constexpr size_t height = 1000;

    auto window = RenderWindow();
    window.create(width, height);
    window.set_active();

    auto audio = AudioFile();
    audio.load("/home/clem/Workspace/crisp/.test/bell.wav");

    auto data = audio.get_samples();

    int offset = 15000;
    int size = width;
    auto shader = State::register_shader("audio/visualize_fourier.glsl");
    auto program = State::register_program(shader);
    State::free_shader(shader);
    State::bind_shader_program(program);

    auto fourier = FourierTransform1D<SPEED>();
    auto update_signal = [&](){

        fourier.transform_from_real(&data[offset], size);
        auto as_signal = fourier.as_signal();
        auto signal = State::register_1d_signal(as_signal.size(), 0, static_cast<float*>(&as_signal[0]));
        State::bind_1d_signal(State::get_active_program_handle(), "_texture_1d", signal);
        State::set_int(State::get_active_program_handle(), "_n_samples", size);
        State::set_vec<2>(State::get_active_program_handle(), "_resolution", window.get_resolution().cast_to<float>());
        State::display();
    };

    update_signal();

    while (window.is_open())
    {
        window.update();
        window.display();

        if (InputHandler::is_key_down(KeyID::RIGHT))
        {
            offset += 10;
            offset = std::min<int>(data.size(), offset);

            update_signal();
        }
        else if (InputHandler::is_key_down(KeyID::LEFT))
        {
            offset -= 10;
            offset = std::max(0, offset);

            update_signal();
        }
        else if (InputHandler::was_key_pressed(KeyID::SPACE))
        {
            shader = State::register_shader("audio/visualize_fourier.glsl");
            program = State::register_program(shader);
            State::free_shader(shader);
            State::bind_shader_program(program);

            update_signal();
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

