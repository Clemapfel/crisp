// 
// Copyright 2021 Clemens Cords
// Created on 09.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SFML/OpenGL.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <system.hpp>
#include <GLES3/gl3.h>

#include <texture/texture.hpp>

namespace crisp
{
    using ID = unsigned int;

    constexpr size_t MAXIMUM_NUMBER_OF_TEXTURES = 64;

    template<typename Context_t = sf::RenderWindow>
    struct State
    {
        template<typename Value_t, size_t N>
        ID add_texture(Texture<Value_t, N>&);

        void bind_texture(ID);

        static inline const sf::ContextSettings settings = sf::ContextSettings(0, 0, 0, 3, 3);

        static inline const float vertices[] = {
             //pos       //colors   //tex
             1,  1, 0,   1, 1, 1,   1, 1,
             1, -1, 0,   1, 1, 1,   1, 0,
            -1, -1, 0,   1, 1, 1,   0, 0,
            -1,  1, 0,   1, 1, 1,   0, 1
        };

        static inline const unsigned int element_indices[] =
        {
            0, 1, 3,
            1, 2, 3
        };

        struct TextureProxy
        {
            unsigned int native_handle = 0;
            bool bool_or_float;
            size_t n = 0;
            PaddingType padding_type;
            sf::RenderTexture context = sf::RenderTexture();

            unsigned int vertex_array_id = 0,
                         vertex_buffer_id = 0,
                         element_buffer_id = 0;
        };

        static inline std::array<TextureProxy, MAXIMUM_NUMBER_OF_TEXTURES> _proxies = {};
        static inline std::array<bool, MAXIMUM_NUMBER_OF_TEXTURES> _proxies_allocated = {false};
        static inline std::map<ID, size_t> _native_to_index = {};

        TextureProxy& get_proxy(ID id)
        {
            return _proxies.at(_native_to_index.at(id));
        }
    };
}

#include ".src/state.inl"