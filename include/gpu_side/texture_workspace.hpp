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
            void initialize(const Image<T, N>&);

            void display();
            GLNativeHandle yield();

        private:
            GLNativeHandle _texture_a, _texture_b;
            GLNativeHandle _framebuffer;

            Vector2ui _size;
            size_t _n_displays = 0;

            std::set<GLNativeHandle> _textures_to_keep;
    };
}

#include ".src/gpu_side/texure_workspace.inl"