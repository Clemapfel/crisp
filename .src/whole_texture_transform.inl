// 
// Copyright 2021 Clemens Cords
// Created on 15.11.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gpu_side/whole_texture_transform.hpp>

namespace crisp
{
    template<size_t N>
    void compute_gradient_magnitude(Texture<float, N>& texture)
    {
        auto& workspace = texture.get_workspace();

        auto shader = gl::State::register_shader("sobel_gradient_magnitude.glsl");
        auto program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
        gl::State::bind_shader_program(program);
        gl::State::bind_texture(program, "_texture", texture);

        workspace.display();
        workspace.yield();

        gl::State::free_program(program);
        return;
    }

    template<size_t N>
    void clamp(Texture<float, N>& texture)
    {
        auto& workspace = texture.get_workspace();

        auto shader = gl::State::register_shader("normalize_clamp.glsl");
        auto program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
        gl::State::bind_shader_program(program);
        gl::State::bind_texture(program, "_texture", texture);

        workspace.display();
        workspace.yield();

        gl::State::free_program(program);
        return;
    }

    template<size_t N>
    void smoothstep(Texture<float, N>& texture)
    {
        auto& workspace = texture.get_workspace();

        auto shader = gl::State::register_shader("normalize_smoothstep.glsl");
        auto program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
        gl::State::bind_shader_program(program);
        gl::State::bind_texture(program, "_texture", texture);

        workspace.display();
        workspace.yield();

        gl::State::free_program(program);
        return;
    }

    template<size_t N>
    void normalize(Texture<float, N>& texture)
    {
        auto data = gl::State::get_texture_data(texture.get_handle());

        auto max = Vector<float, 4>(0);
        auto min = Vector<float, 4>(1);
        for (size_t i = 0; i < data.size();)
        {
            for (size_t n = 0; n < N; ++n, ++i)
            {
                max.at(n) = std::max(max.at(n), data.at(i));
                min.at(n) = std::min(min.at(n), data.at(i));
            }
        }

        auto& workspace = texture.get_workspace();

        auto min_id = gl::State::register_vec<4>(min);
        auto max_id = gl::State::register_vec<4>(max);

        auto shader = gl::State::register_shader("normalize_minmax.glsl");
        auto program = gl::State::register_program(shader);

        gl::State::free_shader(shader);
        gl::State::bind_shader_program(program);
        gl::State::bind_texture(program, "_texture", texture);
        gl::State::bind_vec(program, "_min", min_id);
        gl::State::bind_vec(program, "_max", max_id);

        workspace.display();
        workspace.yield();

        gl::State::free_program(program);
        gl::State::free_vec(min_id);
        gl::State::free_vec(max_id);
        return;
    }


}