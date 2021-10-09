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

    template<typename Context_t = sf::RenderWindow>
    struct State
    {
        template<typename Value_t, size_t N>
        ID add_texture(Texture<Value_t, N>&);

        void bind_texture(ID);





        void enable_context();
        void disable_context();

        Context_t _context;

        struct TextureInfo
        {
            bool bool_or_float;
            size_t n;
            PaddingType padding_type;
        };

        std::map<ID, TextureInfo> _info;
    };
}

#include ".src/state.inl"