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

        auto shader = State::register_shader("sobel_gradient_magnitude.glsl");
        auto program = State::register_program(shader);
        State::free_shader(shader);
        State::bind_shader_program(program);
        State::bind_texture(program, "_texture", texture);

        workspace.display();
        workspace.yield();

        State::free_program(program);
        return;
    }
}