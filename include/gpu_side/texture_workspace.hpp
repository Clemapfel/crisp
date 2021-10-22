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
            Workspace() = default;
            ~Workspace();

            template<typename T, size_t N>
            void initialize(GLNativeHandle texture);
            void draw_to_buffer();
            void swap_buffers();
            GLNativeHandle yield();

        private:
            GLNativeHandle _original;
            GLNativeHandle _texture_a, _texture_b;
            GLNativeHandle _framebuffer;
            size_t _n_displays = 0;
            Vector2ui _size;

            std::set<GLNativeHandle> _textures_to_keep;
    };
}

#include ".src/texture_workspace.inl"