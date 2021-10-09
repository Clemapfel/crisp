// 
// Copyright 2021 Clemens Cords
// Created on 09.10.21 by clem (mail@clemens-cords.com)
//

#include <texture/texture.hpp>

namespace crisp
{
    template<typename T, size_t N>
    Texture<T, N>::Texture(size_t width, size_t height)
        : _size{width, height}
    {
        _data.resize(N * width * height, T(0));
    }

    template<typename T, size_t N>
    void Texture<T, N>::create(size_t width, size_t height)
    {
        _data.clear();
        _data.resize(N * width * height, T(0));
        _size = Vector2ui{weidth, height};
    }
}