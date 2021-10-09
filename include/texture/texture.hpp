// 
// Copyright 2021 Clemens Cords
// Created on 09.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SFML/Window/Context.hpp>

#include <GLES3/gl3.h>
#include <GL/gl.h>

#include <type_traits>
#include <vector>

#include <padding_type.hpp>
#include <vector.hpp>

namespace crisp
{

    /*
     * N == 1
     * bool -> GL_RGBA2
     * float -> GL_R32F
     *
     * N == 2
     * bool -> GL_RGBA2
     * float -> GLRG32F
     *
     * N == 3
     * bool -> GL_RGBA2
     * float -> GL_RGB32F
     *
     * N == 4
     * bool -> GL_RGBA2
     * float -> GL_RGBA32F
     */

    template<typename InnerValue_t, size_t N>
    class Texture
    {
        static_assert(N <= 4, "crisp::Texture only supports up to 4 planes. For a higher dimensional image, use crisp::Image");
        static_asssert(std::is_same_v<InnerValue_t, bool> or std::is_same_v<InnerValue_t, float>, "crisp::Texture only support bool and 32-bit float as value type, for other types use crisp::Image");

        public:
            using Value_t = typename std::conditional<N == 3 and std::is_same_v<InnerValue_t, float>, RGB, Vector<InnerValue_t, N>>::type;

            Texture(size_t width, size_t height);
            void create(size_t width, size_t height);

            void set_padding_type(PaddingType);
            PaddingType get_padding_type() const;

            InnerValue_t* expose_data();

        private:

            Vector2ui _size;
            PaddingType _padding_type;
            std::vector<InnerValue_t> _data;
    };
}

#include ".src/texture.inl"