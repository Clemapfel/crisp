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

using namespace crisp;

int main()
{
    auto window = RenderWindow();
    window.create(300, 300);
    window.set_active();

    Eigen::MatrixXf left, right, control_out;

    size_t rows = 1000;
    size_t cols = 1000;

    left.resize(rows, cols);
    left.setRandom();

    right.resize(rows, cols);
    right.setRandom();

    control_out = left;
    auto gpu_matrix = HardwareAcceleratedMatrix(control_out);
    auto gpu_left = HardwareAcceleratedMatrix(left);

    size_t run = 0;
    auto cpu_side = Benchmark([&](){
        control_out.transposeInPlace();
    });

    std::cout << "CPU: " << cpu_side.execute(3) << std::endl;

    run = 0;
    auto gpu_side = Benchmark([&](){
        gpu_matrix.transpose_in_place();
    });

    std::cout << "GPU: " << gpu_side.execute(3) << std::endl;

    control_out.transposeInPlace();
    auto temp = gpu_matrix.transpose();
    gpu_matrix = temp;

    auto data = gpu_matrix.get_data();
    float error = 0;
    size_t n = 0;
    for (size_t i = 0; i < data.size(); ++i)
    {
        error += abs(data[i] - control_out.data()[i]);;
        n++;
    }

    std::cout << "Mean Error: " << error / n << std::endl;
    return 0;

    while (window.is_open())
    {
        auto time = window.update();

        if (InputHandler::was_key_pressed(RIGHT))
        {
        }
        else if (InputHandler::was_key_pressed(LEFT))
        {
        }

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

