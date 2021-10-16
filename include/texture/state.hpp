// 
// Copyright 2021 Clemens Cords
// Created on 09.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SFML/OpenGL.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <system.hpp>
#include <GLES3/gl3.h>

#include <texture/texture.hpp>
#include <texture/native_handle.hpp>
#include <texture/shader.hpp>

namespace crisp
{
    constexpr size_t MAXIMUM_NUMBER_OF_TEXTURES = 64;
    constexpr size_t MAXIMUM_NUMBER_OF_SHADERS = 256;

    template<typename Context_t = sf::RenderWindow>
    struct State
    {
        template<typename Value_t, size_t N>
        ID add_texture(Texture<Value_t, N>&);
        void bind_texture(ID);

        ID add_shader(const std::string& path);
        void bind_shader(ID);

        void display();

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

        // TEXTURES
        static inline std::array<TextureProxy, MAXIMUM_NUMBER_OF_TEXTURES> _proxies = {};
        static inline std::array<bool, MAXIMUM_NUMBER_OF_TEXTURES> _proxies_allocated = {false};
        static inline std::map<ID, size_t> _texture_native_to_index = {};

        TextureProxy& get_texture_proxy(ID id)
        {
            return _proxies.at(_texture_native_to_index.at(id));
        }

        // SHADERS
        static inline std::array<Shader, MAXIMUM_NUMBER_OF_SHADERS> _shaders = {};
        static inline std::array<bool, MAXIMUM_NUMBER_OF_TEXTURES> _shaders_allocated = {false};
        static inline std::map<ID, size_t> _shader_native_to_index = {};

        Shader& get_shader(ID id)
        {
            return _shaders.at(_shader_native_to_index.at(id));
        }

        static inline bool _vertex_shader_initialized = false;

        // CURRENTLY BOUND
        static inline ID _current_texture;
        static inline ID _fragment_shader;
        static inline sf::Shader _vertex_shader = sf::Shader();
    };
}

#include ".src/state.inl"