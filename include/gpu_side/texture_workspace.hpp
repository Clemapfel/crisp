// 
// Copyright 2021 Clemens Cords
// Created on 19.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gpu_side/state.hpp>
#include <gpu_side/texture.hpp>

namespace crisp
{
    /// @brief allocates two textures and a framebuffer to allow for best-performance texture modification via shaders
    class Workspace
    {
        public:
            /// @brief default ctor
            Workspace() = default;

            /// @brief dtor, does not deallocate texture used for initialization
            ~Workspace();

            /// @brief initialize the workspace by creating a framebuffer and buffer texture
            /// @tparam T: value type of texture in {bool, float}
            /// @tparam N: dimensionality of value type in {1, 2, 3, 4}
            /// @param texture: native handle of texture to be modified
            template<typename T, size_t N>
            void initialize(GLNativeHandle texture);

            /// @brief apply the current fragment shader and past it's contents into the framebuffer
            void draw_to_buffer();

            /// @brief swap the current buffer texture with the texture currently being used for the fragment shader
            void swap_buffers();

            /// @brief applies both draw_to_buffer() and swap_buffer()
            void display();

            /// @brief get the handle of the original, now modified texture
            /// @returns handle of the texture used for initialize
            GLNativeHandle yield();

            /// @brief get the handle of the framebuffer
            /// @returns handle
            GLNativeHandle get_framebuffer() const;

            /// @brief get the handle of the buffer texture
            /// @returns handle
            GLNativeHandle get_buffer_texture() const;

            /// @brief get the handle of texture
            /// @returns handle
            GLNativeHandle get_texture() const;

        private:
            GLNativeHandle _original, _last_updated;
            GLNativeHandle _texture_a, _texture_b;
            GLNativeHandle _framebuffer;
            size_t _n_displays = 0;
            Vector2ui _size;
    };
}

#include ".src/texture_workspace.inl"