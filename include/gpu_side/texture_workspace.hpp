// 
// Copyright 2021 Clemens Cords
// Created on 19.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gpu_side/state.hpp>
#include <gpu_side/texture.hpp>

namespace crisp
{
    class Workspace
    {
        public:
            template<typename T, size_t N>
            Texture& create(const crisp::Image<T, N>& image)
            {
                auto texture_id = State::register_texture<T, N>(image);
                auto buffer_id = State::register_framebuffer(Vector2ui size);
            }
    };
}