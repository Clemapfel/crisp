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

namespace crisp::gpu
{
    using ID = unsigned int;

    template<typename Context_t = sf::RenderWindow>
    struct State
    {
        template<typename Value_t, size_t N>
        upload_texture(std::vector<Value_t> data);

        void enable_context();
        void disable_context();

        Context_t _context;
    };
}

#include ".src/state.inl"