// 
// Copyright 2021 Clemens Cords
// Created on 20.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gpu_side/native_handle.hpp>
#include <image/multi_plane_image.hpp>
#include <gpu_side/texture_workspace.hpp>

namespace crisp
{
    /// @brief object representing an image living on the graphics card
    template<typename T, size_t N>
    class Texture
    {
        static_assert(std::is_same_v<T, float> or std::is_same_v<T, bool>, "Only bool and 32-bit float are supported for GPU-side processing");
        static_assert(1 <= N and N <= 4, "Only 1 to 4 plane textures are supported for GPU-side processing");

        friend class SpatialFilter;
        friend class MorphologicalTransform;

        public:
            /// @brief default ctor, not memory is allocated
            Texture() = default;

            /// @brief copy constructor, hardware accelerated and extremely fast
            /// @param other
            Texture(const Texture<T, N>& other);

            /// @brief move ctor defaulted
            /// @param other
            Texture(Texture<T, N>&& other) = default;

            /// @brief bind an already existing texture to this object
            /// @param already_allocated_handle: gl native handle of already allocated texture
            Texture(GLNativeHandle already_allocated_handle);

            /// @brief create texture from raw data
            /// @param data: row-major, y-inverted array
            Texture(const std::vector<float>&);

            /// @brief create a texture from am image
            /// @param image: of the same type and dimensionality
            Texture(const Image<T, N>&);

            /// @brief create an empty texture
            /// @param width: x-dimension
            /// @param height: y-dimensions
            Texture(size_t width, size_t height);

            /// @brief dtor, deallocates managed gpu-side texture
            ~Texture();

            /// @brief copy assignment, hardware accelerated and extremely fast
            /// @param other
            /// @returns reference to self after assignment
            Texture<T, N>& operator=(const Texture<T, N>& other);

            /// @brief move assignment operator defaulted
            /// @param other
            /// @returns reference to self after assignment
            Texture<T, N>& operator=(Texture<T, N>&& other) noexcept = default;

            /// @brief decay to native handle for easier syntax
            operator GLNativeHandle();

            /// @brief create from image, deallocates currently managed texture, if it exists
            /// @param image
            void create_from(const Image<T, N>&);

            /// @brief create empty, deallocates currently managed texture, if it exists
            /// @param width: x-dimension
            /// @param height: y-dimension
            void create(size_t width, size_t height);

            /// @brief download the data from the graphics card and transform it into an cpu-side image
            /// @returns image
            [[nodiscard]] Image<T, N> to_image() const;

            /// @brief get size of texture
            /// @returns size
            Vector2ui get_size() const;

            /// @brief access the native OpenGL handle of the texture
            /// @returns handle
            GLNativeHandle get_handle() const;

        private:
            size_t _width, _height;
            GLNativeHandle _handle = NONE;

            Workspace _workspace;
    };
}

#include ".src/texture.inl"