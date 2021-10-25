// 
// Copyright 2021 Clemens Cords
// Created on 20.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gpu_side/native_handle.hpp>
#include <image/multi_plane_image.hpp>

/// @brief handle for a texture that lives on the graphics card
namespace crisp
{
    template<typename T, size_t N>
    class Texture
    {
        static_assert(std::is_same_v<T, float> or std::is_same_v<T, bool>, "Only bool and 32-bit float are supported for GPU-side processing");
        static_assert(1 <= N and N <= 4, "Only 1 to 4 plane textures are supported for GPU-side processing");

        public:
            Texture() = default;
            Texture(GLNativeHandle already_allocated_handle);
            Texture(const Image<T, N>&);
            Texture(size_t width, size_t height);
            ~Texture();

            void create_from(const Image<T, N>&);
            void create(size_t width, size_t height);

            [[nodiscard]] Image<T, N> to_image() const;

        private:
            size_t _width, _height;
            GLNativeHandle _handle = NONE;
    };
}

#include ".src/texture.inl"