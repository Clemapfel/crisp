// 
// Copyright 2021 Clemens Cords
// Created on 09.10.21 by clem (mail@clemens-cords.com)
//

#include <texture/state.hpp>

namespace crisp::gpu
{
    template<typename CT>
    void State<CT>::enable_context()
    {
        _context.setActive(true);
    }

    template<typename CT>
    void State<CT>::disable_context()
    {
        _context.setActive(false);
    }

    template<typename CT>
    template<typename Value_t, size_t N>
    ID State<CT>::upload_texture(std::vector<Value_t> data)
    {
        enable_context();

        ID texture;
        glGenTextures(1, &texture);

    }
}